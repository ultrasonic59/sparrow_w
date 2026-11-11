#include "SignalModel.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <random>

namespace {

constexpr double kPi = 3.14159265358979323846;

double frequencyHz(const SignalParams& params)
{
    return params.frequencyMhz * 1.0e6;
}

double sigmaSec(const SignalParams& params)
{
    return static_cast<double>(params.cycles) / (6.0 * frequencyHz(params));
}

double effectiveVelocity(const SignalParams& params)
{
    return params.velocityMps * (1.0 + params.tempCoeffPerC * (params.temperatureC - 20.0));
}

double echoTimeSec(const SignalParams& params, int echoIndex)
{
    const double velocity = effectiveVelocity(params);
    const double t0 = params.zeroUs * 1.0e-6;
    const double depthM = static_cast<double>(echoIndex) * params.thicknessMm * 1.0e-3;
    const double offsetM = params.elementSpacingMm * 0.5e-3;
    const double metal = 2.0 * std::sqrt(depthM * depthM + offsetM * offsetM) / velocity;
    const double coating = 2.0 * static_cast<double>(echoIndex) * params.coatingMm * 1.0e-3
        / params.coatingVelocityMps;
    return t0 + coating + metal;
}

void addWavelet(std::vector<double>& acc, const SignalParams& params, double centerSec, double amplitude)
{
    if (acc.empty() || amplitude == 0.0 || !std::isfinite(amplitude))
        return;

    const double sigma = sigmaSec(params);
    const double f0 = frequencyHz(params);
    const int radius = std::max(1, static_cast<int>(std::ceil(4.0 * sigma * SignalModel::kSampleRateHz)));
    const int center = static_cast<int>(std::llround(centerSec * SignalModel::kSampleRateHz));
    const int begin = std::max(0, center - radius);
    const int end = std::min(static_cast<int>(acc.size()), center + radius + 1);

    for (int i = begin; i < end; ++i) {
        const double dt = static_cast<double>(i) / SignalModel::kSampleRateHz - centerSec;
        const double env = std::exp(-0.5 * (dt * dt) / (sigma * sigma));
        const double carrier = std::sin(2.0 * kPi * f0 * dt);
        acc[static_cast<size_t>(i)] += amplitude * env * carrier;
    }
}

std::int16_t quantize(double value)
{
    if (!std::isfinite(value))
        return 0;
    const long rounded = std::lround(value);
    if (rounded > 32767)
        return 32767;
    if (rounded < -32768)
        return -32768;
    return static_cast<std::int16_t>(rounded);
}

} // namespace

bool SignalModel::paramsValid(const SignalParams& params)
{
    return std::isfinite(params.thicknessMm) && params.thicknessMm > 0.0
        && std::isfinite(params.velocityMps) && params.velocityMps > 0.0
        && std::isfinite(params.frequencyMhz) && params.frequencyMhz > 0.0
        && params.cycles >= 1
        && params.echoCount >= 1
        && std::isfinite(params.attenuationDb)
        && std::isfinite(params.zeroUs)
        && std::isfinite(params.amplitudePercent)
        && std::isfinite(params.crosstalkPercent)
        && std::isfinite(params.noisePercent) && params.noisePercent >= 0.0
        && std::isfinite(params.outputDivider) && params.outputDivider >= 1.0
        && std::isfinite(params.elementSpacingMm) && params.elementSpacingMm >= 0.0
        && std::isfinite(params.coatingMm) && params.coatingMm >= 0.0
        && std::isfinite(params.coatingVelocityMps) && params.coatingVelocityMps > 0.0
        && std::isfinite(params.temperatureC)
        && std::isfinite(params.tempCoeffPerC)
        && effectiveVelocity(params) > 0.0;
}

double SignalModel::echoTimeUs(const SignalParams& params, int echoIndex)
{
    return echoTimeSec(params, echoIndex) * 1.0e6;
}

int SignalModel::echoSample(const SignalParams& params, int echoIndex)
{
    if (!paramsValid(params) || echoIndex < 1)
        return -1;

    const double sample = echoTimeSec(params, echoIndex) * kSampleRateHz;
    if (!std::isfinite(sample) || sample < 0.0
        || sample > static_cast<double>(std::numeric_limits<int>::max())) {
        return -1;
    }
    return static_cast<int>(std::llround(sample));
}

bool SignalModel::fitsDac(int length)
{
    return length > 0 && length <= kDacSamples;
}

double SignalModel::readingMm(const SignalParams& params, int echoSample)
{
    if (!paramsValid(params) || echoSample < 0)
        return -1.0;
    const double velocity = params.velocityMps * (1.0 + params.tempCoeffPerC * (params.temperatureC - 20.0));
    double travelSec = static_cast<double>(echoSample) / kSampleRateHz - params.zeroUs * 1.0e-6;
    travelSec -= 2.0 * params.coatingMm * 1.0e-3 / params.coatingVelocityMps;
    const double halfPathM = velocity * travelSec * 0.5;
    const double offsetM = params.elementSpacingMm * 0.5e-3;
    if (!(halfPathM >= offsetM))
        return 0.0;
    return std::sqrt(halfPathM * halfPathM - offsetM * offsetM) * 1.0e3;
}

int SignalModel::requiredLength(const SignalParams& params)
{
    if (!paramsValid(params))
        return -1;

    const double end = echoTimeSec(params, params.echoCount) + 3.0 * sigmaSec(params);
    if (!std::isfinite(end))
        return -1;

    const double samples = std::ceil(end * kSampleRateHz) + 1.0;
    if (!std::isfinite(samples) || samples < 1.0 || samples > 1.0e9)
        return -1;
    return static_cast<int>(samples);
}

Waveform SignalModel::generate(const SignalParams& params)
{
    Waveform wave;
    if (paramsValid(params)) {
        wave.firstEchoUs = echoTimeUs(params, 1);
        wave.firstEchoSample = echoSample(params, 1);
        wave.requiredLength = requiredLength(params);
    }

    wave.valid = wave.requiredLength > 0
        && wave.requiredLength <= kMaxSamples
        && wave.firstEchoSample >= 0
        && wave.firstEchoSample < wave.requiredLength;
    if (!wave.valid)
        return wave;

    std::vector<double> acc(static_cast<size_t>(wave.requiredLength), 0.0);
    const double fullScale = 32767.0;
    const double base = params.amplitudePercent / 100.0 * fullScale;

    addWavelet(acc, params, params.zeroUs * 1.0e-6, params.crosstalkPercent / 100.0 * fullScale);
    for (int echo = 1; echo <= params.echoCount; ++echo) {
        const double decay = std::pow(10.0, -static_cast<double>(echo - 1) * params.attenuationDb / 20.0);
        addWavelet(acc, params, echoTimeSec(params, echo), base * decay);
    }

    if (params.noisePercent > 0.0) {
        std::mt19937 rng(params.noiseSeed);
        std::normal_distribution<double> noise(0.0, params.noisePercent / 100.0 * fullScale);
        for (double& sample : acc)
            sample += noise(rng);
    }

    const double divider = params.outputDivider;
    if (divider > 1.0) {
        for (double& sample : acc)
            sample /= divider;
    }

    wave.samples.resize(acc.size());
    for (size_t i = 0; i < acc.size(); ++i)
        wave.samples[i] = quantize(acc[i]);
    return wave;
}

Waveform SignalModel::continuousSine(double frequencyMhz, double amplitudePercent)
{
    Waveform wave;
    if (!std::isfinite(frequencyMhz) || frequencyMhz <= 0.0 || !std::isfinite(amplitudePercent))
        return wave;
    const double frequencyHz = frequencyMhz * 1.0e6;
    const double periodSamples = kSampleRateHz / frequencyHz;
    int cycles = 4;
    if (periodSamples < 500.0)
        cycles = std::max(4, static_cast<int>(2000.0 / periodSamples));
    int count = static_cast<int>(std::ceil(periodSamples * static_cast<double>(cycles))) + 1;
    if (count < 32)
        count = 32;
    if (count > 4096)
        count = 4096;
    const double scale = 32767.0 * std::clamp(amplitudePercent, 0.0, 100.0) / 100.0;
    wave.samples.resize(static_cast<size_t>(count));
    for (int i = 0; i < count; ++i) {
        const double phase = 2.0 * kPi * frequencyHz * static_cast<double>(i) / kSampleRateHz;
        wave.samples[static_cast<size_t>(i)] = quantize(std::sin(phase) * scale);
    }
    wave.requiredLength = count;
    wave.valid = true;
    wave.firstEchoUs = 1.0e6 / frequencyHz;
    wave.firstEchoSample = -1;
    return wave;
}
