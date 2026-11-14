#pragma once

#include <cstdint>
#include <vector>

// Thickness-gauge echo model at 100 MHz, int16 codes.
// Center of back-wall echo k:
//   t0 + 2*k*d_coat/c_coat + 2*sqrt((k*d)^2 + (spacing/2)^2) / c(T)
// With zero spacing, zero coating and zero temperature coefficient
// this is t0 + k * 2 * d / c. The envelope is a Gaussian, sigma = N / (6 f0).
// Buffer length runs to t_last + 3 sigma. c(T) = c * (1 + alpha*(T - 20)).

struct SignalParams {
    double thicknessMm = 10.0;
    double velocityMps = 5920.0;
    double frequencyMhz = 5.0;
    int cycles = 3;
    int echoCount = 1;
    double attenuationDb = 0.0;
    double zeroUs = 0.0;
    double amplitudePercent = 80.0;
    double crosstalkPercent = 0.0;
    double noisePercent = 0.0;
    std::uint32_t noiseSeed = 1;
    double outputDivider = 1.0;
    double elementSpacingMm = 0.0;
    double coatingMm = 0.0;
    double coatingVelocityMps = 2400.0;
    double temperatureC = 20.0;
    double tempCoeffPerC = 0.0;
};

struct Waveform {
    std::vector<std::int16_t> samples;
    int requiredLength = 0;
    bool valid = false;
    double firstEchoUs = 0.0;
    int firstEchoSample = -1;
};

class SignalModel {
public:
    static constexpr double kSampleRateHz = 100000000.0;
    static constexpr int kMaxSamples = 32768;
    static constexpr int kDacSamples = 8191;

    static bool paramsValid(const SignalParams& params);
    static double echoTimeUs(const SignalParams& params, int echoIndex);
    static int echoSample(const SignalParams& params, int echoIndex);
    static int requiredLength(const SignalParams& params);
    static bool fitsDac(int length);
    static double readingMm(const SignalParams& params, int echoSample);
    static Waveform generate(const SignalParams& params);
    static Waveform continuousSine(double frequencyMhz, double amplitudePercent);
};
