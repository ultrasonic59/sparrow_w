#include "MainWindow.h"
#include "SignalModel.h"
#include "ThkLink.h"
#include "WaveformPlot.h"

#include "thk_bank.h"
#include "thk_proto.h"

#include <QAction>
#include <QApplication>
#include <QSettings>
#include <QCheckBox>
#include <QComboBox>
#include <QDial>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollBar>
#include <QSlider>
#include <QSpinBox>
#include <QToolButton>
#include <QtTest>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>

namespace {

int peakAbs(const std::vector<std::int16_t>& samples, int begin, int end)
{
    begin = std::max(0, begin);
    end = std::min(end, static_cast<int>(samples.size()));
    int best = 0;
    for (int i = begin; i < end; ++i)
        best = std::max(best, std::abs(static_cast<int>(samples[static_cast<size_t>(i)])));
    return best;
}

SignalParams referenceParams()
{
    SignalParams params;
    params.thicknessMm = 10.0;
    params.velocityMps = 5920.0;
    params.frequencyMhz = 5.0;
    params.cycles = 3;
    params.echoCount = 1;
    params.attenuationDb = 0.0;
    params.zeroUs = 0.0;
    params.amplitudePercent = 100.0;
    params.crosstalkPercent = 0.0;
    params.noisePercent = 0.0;
    return params;
}

} // namespace

class SignalTests : public QObject {
    Q_OBJECT

private slots:
    void init();
    void echoSamplesMatchReference();
    void bufferLengthFollowsThreeSigma();
    void waveformEnergySitsOnFirstEcho();
    void attenuationScalesSecondEcho();
    void overflowIsRejected();
    void noiseIsRepeatable();
    void windowShowsReferenceSample();
    void windowWarnsWhenBufferDoesNotFit();
    void plotPaintsTrace();
    void plotKeepsContinuousTraceAt20mm();
    void plotZoomAndScroll();
    void udpReassemblesAfterLostPacket();
    void statusCarriesLengthAndChecksum();
    void uartStreamFeedsTheSameBank();
    void dacRamLimitIsRejected();
    void vPathCoatingAndTemperatureMoveTheEcho();
    void probePresetSetsDualElement();
    void gaugeSetMatchesWithinOneSample();
    void dividerScalesTheBuffer();
    void windowSelectsGaugePoint();
    void windowSendsWaveToStub();
    void windowSendsTextToStub();
    void windowStartsContinuousSine();
    void continuousSineMatchesPeriod();
    void windowSetsOutputDacs();
    void windowSelectsPlayback();
    void windowRestoresSettings();
};

void SignalTests::init()
{
    QCoreApplication::setOrganizationName("sparrow5");
    QCoreApplication::setApplicationName("sparrow5_tests");
    QSettings settings;
    settings.clear();
    settings.sync();
}

void SignalTests::echoSamplesMatchReference()
{
    SignalParams params = referenceParams();
    QCOMPARE(SignalModel::echoSample(params, 1), 338);
    QVERIFY(std::abs(SignalModel::echoTimeUs(params, 1) - 3.378378378) < 1e-6);

    params.thicknessMm = 25.0;
    QCOMPARE(SignalModel::echoSample(params, 1), 845);
    QCOMPARE(SignalModel::echoSample(params, 2), 1689);
    QVERIFY(std::abs(SignalModel::echoTimeUs(params, 1) - 8.445945946) < 1e-6);
}

void SignalTests::bufferLengthFollowsThreeSigma()
{
    const SignalParams params = referenceParams();
    QCOMPARE(SignalModel::requiredLength(params), 369);

    SignalParams two = params;
    two.thicknessMm = 25.0;
    two.echoCount = 2;
    QCOMPARE(SignalModel::requiredLength(two), 1721);
}

void SignalTests::waveformEnergySitsOnFirstEcho()
{
    const Waveform wave = SignalModel::generate(referenceParams());
    QVERIFY(wave.valid);
    QCOMPARE(static_cast<int>(wave.samples.size()), 369);
    QCOMPARE(wave.samples.front(), static_cast<std::int16_t>(0));
    QVERIFY(peakAbs(wave.samples, 338 - 8, 338 + 9) > 20000);
    QVERIFY(peakAbs(wave.samples, 0, 200) == 0);
}

void SignalTests::attenuationScalesSecondEcho()
{
    SignalParams params = referenceParams();
    params.thicknessMm = 25.0;
    params.echoCount = 2;
    params.attenuationDb = 20.0;
    const Waveform wave = SignalModel::generate(params);
    QVERIFY(wave.valid);
    const int first = peakAbs(wave.samples, 845 - 8, 845 + 9);
    const int second = peakAbs(wave.samples, 1689 - 8, 1689 + 9);
    QVERIFY(first > 20000);
    const double ratio = static_cast<double>(second) / static_cast<double>(first);
    QVERIFY(ratio > 0.06);
    QVERIFY(ratio < 0.16);
}

void SignalTests::overflowIsRejected()
{
    SignalParams params = referenceParams();
    params.thicknessMm = 500.0;
    params.echoCount = 4;
    const Waveform wave = SignalModel::generate(params);
    QVERIFY(wave.requiredLength > SignalModel::kMaxSamples);
    QVERIFY(!wave.valid);
    QVERIFY(wave.samples.empty());
}

void SignalTests::noiseIsRepeatable()
{
    SignalParams params = referenceParams();
    params.noisePercent = 1.0;
    params.noiseSeed = 7;
    const Waveform first = SignalModel::generate(params);
    const Waveform second = SignalModel::generate(params);
    QCOMPARE(first.samples, second.samples);
    QVERIFY(peakAbs(first.samples, 0, 50) > 0);
}

void SignalTests::windowShowsReferenceSample()
{
    MainWindow window;
    auto* summary = window.findChild<QLabel*>("summaryLabel");
    auto* thickness = window.findChild<QDoubleSpinBox*>("thicknessSpin");
    QVERIFY(summary);
    QVERIFY(thickness);
    QVERIFY(window.findChild<QPushButton*>("exportButton") == nullptr);
    const auto actions = window.findChildren<QAction*>();
    for (QAction* action : actions)
        QVERIFY(action->text() != QString("Settings..."));
    QCOMPARE(thickness->value(), 10.0);
    QVERIFY(summary->text().contains("338"));
    QVERIFY(summary->text().contains("fits"));
}

void SignalTests::windowWarnsWhenBufferDoesNotFit()
{
    MainWindow window;
    auto* thickness = window.findChild<QDoubleSpinBox*>("thicknessSpin");
    auto* echoes = window.findChild<QSpinBox*>("echoCountSpin");
    auto* summary = window.findChild<QLabel*>("summaryLabel");
    echoes->setValue(4);
    thickness->setValue(500.0);
    QVERIFY(summary->text().contains("not formed"));
}

void SignalTests::plotPaintsTrace()
{
    const Waveform wave = SignalModel::generate(referenceParams());
    WaveformPlot plot;
    plot.resize(800, 420);
    plot.setWaveform(wave.samples, wave.firstEchoSample);
    plot.show();
    plot.repaint();
    const QPixmap pixmap = plot.grab();
    QVERIFY(!pixmap.isNull());
    QVERIFY(pixmap.width() >= plot.width());

    const QImage image = pixmap.toImage();
    int greenPixels = 0;
    for (int y = 0; y < image.height(); y += 2) {
        for (int x = 0; x < image.width(); x += 2) {
            const QColor color(image.pixel(x, y));
            if (color.green() > 180 && color.green() > color.red() + 40 && color.green() > color.blue() + 40)
                ++greenPixels;
        }
    }
    QVERIFY(greenPixels > 20);
}

void SignalTests::plotKeepsContinuousTraceAt20mm()
{
    SignalParams params = referenceParams();
    params.thicknessMm = 20.0;
    params.echoCount = 2;
    params.amplitudePercent = 80.0;
    const Waveform wave = SignalModel::generate(params);
    QVERIFY(wave.valid);
    QVERIFY(static_cast<int>(wave.samples.size()) > 500);

    WaveformPlot plot;
    plot.resize(480, 420);
    plot.setWaveform(wave.samples, wave.firstEchoSample);
    plot.show();
    plot.repaint();
    const QImage image = plot.grab().toImage();
    const double scale = static_cast<double>(image.width()) / plot.width();

    const int left = 64;
    const int plotWidth = plot.width() - left - 16;
    const double echoX = (left + (static_cast<double>(wave.firstEchoSample) + 0.5) * plotWidth / wave.samples.size()) * scale;
    const int x0 = static_cast<int>(echoX) - static_cast<int>(30 * scale);
    const int x1 = static_cast<int>(echoX) + static_cast<int>(30 * scale);
    int minY = image.height();
    int maxY = 0;
    int columnsWithTrace = 0;
    for (int x = std::max(0, x0); x < std::min(image.width(), x1); ++x) {
        bool column = false;
        for (int y = 0; y < image.height(); ++y) {
            const QColor color(image.pixel(x, y));
            if (color.green() > 180 && color.green() > color.red() + 40 && color.green() > color.blue() + 40) {
                column = true;
                minY = std::min(minY, y);
                maxY = std::max(maxY, y);
            }
        }
        if (column)
            ++columnsWithTrace;
    }
    QVERIFY(columnsWithTrace > static_cast<int>(40 * scale));
    QVERIFY(maxY - minY > image.height() / 3);
}

int greenSpan(const QImage& image)
{
    int minY = image.height();
    int maxY = -1;
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            const QColor color(image.pixel(x, y));
            if (color.green() > 180 && color.green() > color.red() + 40 && color.green() > color.blue() + 40) {
                minY = std::min(minY, y);
                maxY = std::max(maxY, y);
            }
        }
    }
    return maxY < 0 ? 0 : maxY - minY;
}

void SignalTests::plotZoomAndScroll()
{
    SignalParams params = referenceParams();
    params.thicknessMm = 20.0;
    params.echoCount = 2;
    params.amplitudePercent = 80.0;
    const Waveform wave = SignalModel::generate(params);

    WaveformPlot plot;
    plot.resize(640, 480);
    plot.setWaveform(wave.samples, wave.firstEchoSample);
    plot.show();

    auto* zoom = plot.findChild<QSlider*>("zoomSlider");
    auto* scroll = plot.findChild<QScrollBar*>("timeScroll");
    auto* label = plot.findChild<QLabel*>("zoomLabel");
    QVERIFY(zoom);
    QVERIFY(scroll);
    QVERIFY(label);
    QCOMPARE(zoom->value(), 1);
    QCOMPARE(scroll->maximum(), 0);
    QCOMPARE(label->text(), QString::fromUtf8("×1"));

    zoom->setValue(16);
    QCOMPARE(label->text(), QString::fromUtf8("×16"));
    QVERIFY(scroll->maximum() > 0);
    scroll->setValue(0);
    plot.repaint();
    const int baselineSpan = greenSpan(plot.grab().toImage());
    QVERIFY(baselineSpan < plot.height() / 10);

    const int echoOrigin = std::max(0, wave.firstEchoSample - scroll->pageStep() / 2);
    scroll->setValue(echoOrigin);
    plot.repaint();
    const int echoSpan = greenSpan(plot.grab().toImage());
    QVERIFY(echoSpan > plot.grab().toImage().height() / 3);
}

void SignalTests::udpReassemblesAfterLostPacket()
{
    std::vector<std::int16_t> samples(THK_WAVE_SAMPLES_PER_CHUNK + 8);
    for (int i = 0; i < static_cast<int>(samples.size()); ++i)
        samples[static_cast<size_t>(i)] = static_cast<std::int16_t>(i * 3 - 20);

    ThkStubReceiver stub;
    QVERIFY(stub.listen(0));
    stub.dropNextWave();

    ThkSender sender;
    const ThkUploadResult result = sender.upload(QHostAddress::LocalHost, stub.port(), samples, 100000000);
    QVERIFY2(result.ok, qUtf8Printable(result.error));
    QVERIFY(result.retries >= 1);
    QVERIFY(stub.complete());
    QVERIFY(stub.samples() == samples);
    QCOMPARE(stub.sampleRateHz(), 100000000u);

    ThkStatusBody status = {};
    QVERIFY(sender.ping(QHostAddress::LocalHost, stub.port(), &status));
    QCOMPARE(int(status.link), 1);
    QCOMPARE(status.sample_count, quint32(samples.size()));
    QCOMPARE(status.sample_crc, thk_samples_crc(samples.data(), quint32(samples.size())));
}

QByteArray thkFrame(quint16 type, quint32 sequence, const QByteArray& payload)
{
    QByteArray frame(int(sizeof(ThkHeader) + payload.size()), 0);
    ThkHeader header = {};
    header.signature = THK_SIGNATURE;
    header.version = THK_VERSION;
    header.type = type;
    header.sequence = sequence;
    header.length = quint16(payload.size());
    std::memcpy(frame.data(), &header, sizeof(header));
    if (!payload.isEmpty())
        std::memcpy(frame.data() + sizeof(header), payload.constData(), size_t(payload.size()));
    thk_seal(reinterpret_cast<uint8_t*>(frame.data()), size_t(frame.size()));
    return frame;
}

void SignalTests::statusCarriesLengthAndChecksum()
{
    SignalParams params = referenceParams();
    const Waveform wave = SignalModel::generate(params);
    QVERIFY(wave.valid);
    QVERIFY(wave.samples.size() <= size_t(THK_DAC_SAMPLES));

    ThkStubReceiver stub;
    QVERIFY(stub.listen(0));
    ThkSender sender;
    const ThkUploadResult result = sender.upload(QHostAddress::LocalHost, stub.port(), wave.samples, 100000000);
    QVERIFY2(result.ok, qUtf8Printable(result.error));

    ThkStatusBody status = {};
    QVERIFY(sender.ping(QHostAddress::LocalHost, stub.port(), &status));
    QCOMPARE(status.sample_count, quint32(wave.samples.size()));
    QCOMPARE(status.sample_crc, thk_samples_crc(wave.samples.data(), quint32(wave.samples.size())));
    QCOMPARE(int(status.flags), 1);
    QCOMPARE(int(status.last_error), 0);
}

void SignalTests::uartStreamFeedsTheSameBank()
{
    std::vector<std::int16_t> samples(700);
    for (int i = 0; i < 700; ++i)
        samples[size_t(i)] = std::int16_t(1000 - i);

    const int chunkSamples = THK_WAVE_SAMPLES_PER_CHUNK;
    const int chunkCount = 2;
    ThkStream stream;
    thk_stream_reset(&stream);
    ThkBank bank;
    thk_bank_reset(&bank);
    QVERIFY(thk_stream_byte(&stream, 0x11) == 0);

    for (int chunk = 0; chunk < chunkCount; ++chunk) {
        const int begin = chunk * chunkSamples;
        const int count = std::min(chunkSamples, 700 - begin);
        ThkWaveInfo info = {};
        info.sample_rate_hz = 100000000;
        info.sample_format = THK_SAMPLE_INT16;
        info.total_samples = 700;
        info.chunk_index = quint16(chunk);
        info.chunk_count = quint16(chunkCount);
        QByteArray payload(int(sizeof(info) + size_t(count) * sizeof(std::int16_t)), 0);
        std::memcpy(payload.data(), &info, sizeof(info));
        std::memcpy(payload.data() + sizeof(info), samples.data() + begin, size_t(count) * sizeof(std::int16_t));
        const QByteArray frame = thkFrame(THK_WAVE, quint32(chunk + 1), payload);
        int frames = 0;
        for (char byte : frame) {
            if (!thk_stream_byte(&stream, static_cast<uint8_t>(byte)))
                continue;
            ThkHeader header = {};
            std::memcpy(&header, stream.raw, sizeof(header));
            QCOMPARE(int(header.type), int(THK_WAVE));
            QCOMPARE(int(thk_bank_wave(&bank, stream.raw + sizeof(header), header.length)), int(THK_ACK_OK));
            thk_stream_consume(&stream);
            ++frames;
        }
        QCOMPARE(frames, 1);
    }

    QVERIFY(bank.complete);
    ThkStatusBody status = {};
    thk_bank_status(&bank, &status);
    QCOMPARE(status.sample_count, 700u);
    QCOMPARE(status.sample_crc, thk_samples_crc(samples.data(), 700));
    QVERIFY(std::equal(samples.begin(), samples.end(), bank.samples));
}

void SignalTests::dacRamLimitIsRejected()
{
    std::vector<std::int16_t> samples(size_t(THK_DAC_SAMPLES) + 1, 1);
    ThkStubReceiver stub;
    QVERIFY(stub.listen(0));
    ThkSender sender;
    const ThkUploadResult result = sender.upload(QHostAddress::LocalHost, stub.port(), samples, 100000000);
    QVERIFY(!result.ok);
    QVERIFY(!stub.complete());
}

void SignalTests::vPathCoatingAndTemperatureMoveTheEcho()
{
    SignalParams straight = referenceParams();
    QCOMPARE(SignalModel::echoSample(straight, 1), 338);

    SignalParams vpath = straight;
    vpath.elementSpacingMm = 8.0;
    QCOMPARE(SignalModel::echoSample(vpath, 1), 364);
    QVERIFY(std::abs(SignalModel::readingMm(vpath, 364) - 10.0) < 0.05);

    SignalParams coated = straight;
    coated.coatingMm = 1.0;
    coated.coatingVelocityMps = 2500.0;
    QCOMPARE(SignalModel::echoSample(coated, 1), 418);

    SignalParams hot = straight;
    hot.temperatureC = 120.0;
    hot.tempCoeffPerC = -0.0001;
    QCOMPARE(SignalModel::echoSample(hot, 1), 341);
}

void SignalTests::probePresetSetsDualElement()
{
    MainWindow window;
    auto* probe = window.findChild<QComboBox*>("probeCombo");
    auto* spacing = window.findChild<QDoubleSpinBox*>("spacingSpin");
    auto* frequency = window.findChild<QDoubleSpinBox*>("frequencySpin");
    auto* cycles = window.findChild<QSpinBox*>("cyclesSpin");
    QVERIFY(probe);
    probe->setCurrentText("Dual 5 MHz");
    QCOMPARE(spacing->value(), 8.0);
    QCOMPARE(frequency->value(), 5.0);
    QCOMPARE(cycles->value(), 2);
    QCOMPARE(probe->currentText(), QString("Dual 5 MHz"));
}

void SignalTests::gaugeSetMatchesWithinOneSample()
{
    QCOMPARE(SignalModel::kDacSamples, THK_DAC_SAMPLES);
    const double points[] = {2.0, 5.0, 10.0, 20.0, 50.0, 100.0};
    SignalParams params = referenceParams();
    params.echoCount = 2;
    const double stepMm = params.velocityMps / (2.0 * SignalModel::kSampleRateHz) * 1.0e3;
    for (double thickness : points) {
        params.thicknessMm = thickness;
        const int sample = SignalModel::echoSample(params, 1);
        const double reading = SignalModel::readingMm(params, sample);
        QVERIFY2(std::abs(reading - thickness) <= stepMm + 1e-9, qPrintable(QString::number(thickness)));
        const Waveform wave = SignalModel::generate(params);
        QVERIFY2(wave.valid, qPrintable(QString::number(thickness)));
        QVERIFY2(SignalModel::fitsDac(wave.requiredLength),
                 qPrintable(QString("%1 needs %2").arg(thickness).arg(wave.requiredLength)));
    }
}

void SignalTests::dividerScalesTheBuffer()
{
    SignalParams params = referenceParams();
    params.amplitudePercent = 100.0;
    const Waveform full = SignalModel::generate(params);
    params.outputDivider = 10.0;
    const Waveform divided = SignalModel::generate(params);
    QVERIFY(full.valid);
    QVERIFY(divided.valid);
    QCOMPARE(full.samples.size(), divided.samples.size());
    const int fullPeak = peakAbs(full.samples, 0, static_cast<int>(full.samples.size()));
    const int dividedPeak = peakAbs(divided.samples, 0, static_cast<int>(divided.samples.size()));
    QVERIFY(fullPeak > 20000);
    QVERIFY(dividedPeak * 9 < fullPeak);
    QVERIFY(dividedPeak * 11 > fullPeak);
}

void SignalTests::windowSelectsGaugePoint()
{
    MainWindow window;
    auto* point = window.findChild<QPushButton*>("gauge20");
    auto* thickness = window.findChild<QDoubleSpinBox*>("thicknessSpin");
    auto* summary = window.findChild<QLabel*>("summaryLabel");
    QVERIFY(point);
    QVERIFY(thickness);
    point->click();
    QCOMPARE(thickness->value(), 20.0);
    QVERIFY(summary->text().contains("Gauge"));
    QVERIFY(summary->text().contains("fits"));
}

void SignalTests::windowSendsWaveToStub()
{
    MainWindow window;
    auto* link = window.findChild<QLabel*>("linkLabel");
    auto* stub = window.findChild<QCheckBox*>("stubCheck");
    auto* host = window.findChild<QLineEdit*>("hostEdit");
    auto* thickness = window.findChild<QDoubleSpinBox*>("thicknessSpin");
    QVERIFY(window.findChild<QPushButton*>("sendButton") == nullptr);
    QVERIFY(link);
    QVERIFY(stub);
    QVERIFY(host);
    QVERIFY(thickness);
    auto* port = window.findChild<QSpinBox*>("portSpin");
    QVERIFY(port);
    QVERIFY(!stub->isChecked());
    host->setText("127.0.0.1");
    port->blockSignals(true);
    port->setValue(45112);
    port->blockSignals(false);
    stub->setChecked(true);
    thickness->setValue(20.0);
    QVERIFY2(link->text().contains("Received"), qUtf8Printable(link->text()));
}

void SignalTests::windowSendsTextToStub()
{
    ThkStubReceiver stub;
    QVERIFY(stub.listen(0));
    ThkSender sender;
    quint16 status = 99;
    const QByteArray bytes("hi\r\n");
    QVERIFY(sender.sendText(QHostAddress(QHostAddress::LocalHost), stub.port(), 115200, bytes, &status));
    QCOMPARE(int(status), 0);
    QCOMPARE(stub.lastText(), bytes);

    MainWindow window;
    auto* text = window.findChild<QLineEdit*>("comText");
    auto* baud = window.findChild<QSpinBox*>("comBaud");
    auto* button = window.findChild<QPushButton*>("comButton");
    auto* link = window.findChild<QLabel*>("linkLabel");
    auto* host = window.findChild<QLineEdit*>("hostEdit");
    auto* stubCheck = window.findChild<QCheckBox*>("stubCheck");
    QVERIFY(text);
    QVERIFY(baud);
    QVERIFY(button);
    QVERIFY(link);
    auto* port = window.findChild<QSpinBox*>("portSpin");
    QVERIFY(host);
    QVERIFY(stubCheck);
    QVERIFY(port);
    host->setText("127.0.0.1");
    port->blockSignals(true);
    port->setValue(45113);
    port->blockSignals(false);
    stubCheck->setChecked(true);
    QCOMPARE(baud->value(), 115200);
    text->setText("abc");
    button->click();
    QVERIFY2(link->text().contains("Stub accepted text, 5 bytes"), qUtf8Printable(link->text()));
}

void SignalTests::windowStartsContinuousSine()
{
    MainWindow window;
    auto* host = window.findChild<QLineEdit*>("hostEdit");
    auto* stub = window.findChild<QCheckBox*>("stubCheck");
    auto* port = window.findChild<QSpinBox*>("portSpin");
    QVERIFY(host);
    QVERIFY(stub);
    QVERIFY(port);
    host->setText("127.0.0.1");
    port->blockSignals(true);
    port->setValue(45114);
    port->blockSignals(false);
    stub->setChecked(true);
    window.show();
    auto* pulses = window.findChild<QRadioButton*>("modePulses");
    auto* sine = window.findChild<QRadioButton*>("modeSine");
    auto* thickness = window.findChild<QDoubleSpinBox*>("thicknessSpin");
    auto* freq = window.findChild<QDoubleSpinBox*>("sineFreq");
    auto* amplitude = window.findChild<QDoubleSpinBox*>("sineAmplitude");
    auto* plot = window.findChild<WaveformPlot*>("waveformPlot");
    auto* link = window.findChild<QLabel*>("linkLabel");
    QVERIFY(pulses && pulses->isChecked());
    QVERIFY(thickness->isVisible());
    QVERIFY(!freq->isVisible());
    auto* analogAmp = window.findChild<QDial*>("analogAmp");
    auto* analogOffset = window.findChild<QDial*>("analogOffset");
    QVERIFY(analogAmp && analogAmp->isVisible());
    QVERIFY(analogOffset && analogOffset->isVisible());

    sine->click();
    QVERIFY(window.findChild<QPushButton*>("sineButton") == nullptr);
    QVERIFY2(link->text().contains("sine 5.000 MHz"), qUtf8Printable(link->text()));
    QVERIFY(analogAmp->isVisible());
    QVERIFY(analogOffset->isVisible());
    QVERIFY(!thickness->isVisible());
    QVERIFY(freq->isVisible());
    QVERIFY(amplitude->isVisible());
    amplitude->setValue(100.0);
    freq->setValue(5.0);
    QCOMPARE(int(plot->samples().at(5)), 32767);
    QCOMPARE(int(plot->samples().at(15)), -32767);

    freq->setValue(2.5);
    QVERIFY2(link->text().contains("sine 2.500 MHz"), qUtf8Printable(link->text()));
    amplitude->setValue(40.0);
    QVERIFY2(link->text().contains("40%"), qUtf8Printable(link->text()));

    pulses->click();
    QVERIFY(thickness->isVisible());
    QVERIFY(!freq->isVisible());
    auto* summary = window.findChild<QLabel*>("summaryLabel");
    QVERIFY(summary->text().contains("338"));
}

void SignalTests::continuousSineMatchesPeriod()
{
    const Waveform sine = SignalModel::continuousSine(5.0, 100.0);
    QVERIFY(sine.valid);
    QCOMPARE(sine.samples.front(), static_cast<std::int16_t>(0));
    QCOMPARE(sine.samples.at(5), static_cast<std::int16_t>(32767));
    QCOMPARE(sine.samples.at(10), static_cast<std::int16_t>(0));
    QCOMPARE(sine.samples.at(15), static_cast<std::int16_t>(-32767));
    QCOMPARE(sine.firstEchoUs, 0.2);
}

void SignalTests::windowSetsOutputDacs()
{
    MainWindow window;
    auto* amplitude = window.findChild<QDial*>("analogAmp");
    auto* offset = window.findChild<QDial*>("analogOffset");
    auto* link = window.findChild<QLabel*>("linkLabel");
    auto* port = window.findChild<QSpinBox*>("portSpin");
    auto* stub = window.findChild<QCheckBox*>("stubCheck");
    auto* host = window.findChild<QLineEdit*>("hostEdit");
    QVERIFY(amplitude);
    QVERIFY(offset);
    QVERIFY(window.findChild<QPushButton*>("analogButton") == nullptr);
    QVERIFY(port);
    QVERIFY(stub);
    QVERIFY(host);
    QCOMPARE(amplitude->value(), 2048);
    QCOMPARE(offset->value(), 2048);
    host->setText("127.0.0.1");
    port->blockSignals(true);
    port->setValue(45111);
    port->blockSignals(false);
    stub->setChecked(true);
    auto* ampUp = window.findChild<QToolButton*>("analogAmpUp");
    auto* ampDown = window.findChild<QToolButton*>("analogAmpDown");
    QVERIFY(ampUp && ampDown);
    QVERIFY(window.findChild<QToolButton*>("analogOffsetUp"));
    QVERIFY(window.findChild<QToolButton*>("analogOffsetDown"));
    ampUp->click();
    QCOMPARE(amplitude->value(), 2049);
    ampDown->click();
    QCOMPARE(amplitude->value(), 2048);
    auto* ampUpPage = window.findChild<QToolButton*>("analogAmpUpPage");
    auto* ampDownPage = window.findChild<QToolButton*>("analogAmpDownPage");
    QVERIFY(ampUpPage && ampDownPage);
    QVERIFY(window.findChild<QToolButton*>("analogOffsetUpPage"));
    QVERIFY(window.findChild<QToolButton*>("analogOffsetDownPage"));
    ampUpPage->click();
    QCOMPARE(amplitude->value(), 2048 + amplitude->pageStep());
    ampDownPage->click();
    QCOMPARE(amplitude->value(), 2048);
    amplitude->setValue(1000);
    offset->setValue(3000);
    QVERIFY2(link->text().contains("amplitude 1000"), qUtf8Printable(link->text()));
    QVERIFY(link->text().contains("offset 3000"));
}

void SignalTests::windowSelectsPlayback()
{
    MainWindow window;
    auto* mode = window.findChild<QComboBox*>("playMode");
    auto* stop = window.findChild<QPushButton*>("stopButton");
    auto* pause = window.findChild<QSpinBox*>("pauseSpin");
    auto* link = window.findChild<QLabel*>("linkLabel");
    auto* host = window.findChild<QLineEdit*>("hostEdit");
    auto* stub = window.findChild<QCheckBox*>("stubCheck");
    auto* port = window.findChild<QSpinBox*>("portSpin");
    QVERIFY(mode && stop && pause && link && host && stub && port);
    QVERIFY(window.findChild<QRadioButton*>("runButton") == nullptr);
    QCOMPARE(mode->currentText(), QString("Free run"));
    QCOMPARE(stop->text(), QString("Start"));
    host->setText("127.0.0.1");
    port->blockSignals(true);
    port->setValue(45115);
    port->blockSignals(false);
    stub->setChecked(true);

    stop->click();
    QCOMPARE(mode->currentText(), QString("Free run"));
    QCOMPARE(stop->text(), QString("Stop"));
    QVERIFY2(link->text().contains("Stub accepted free run"), qUtf8Printable(link->text()));
    pause->setValue(500);
    QVERIFY2(link->text().contains("Stub accepted free run"), qUtf8Printable(link->text()));

    mode->setCurrentText("On trigger");
    QCOMPARE(mode->currentText(), QString("On trigger"));
    QVERIFY2(link->text().contains("Stub accepted trigger mode"), qUtf8Printable(link->text()));

    mode->setCurrentText("Single shot");
    QCOMPARE(mode->currentText(), QString("Single shot"));
    QVERIFY2(link->text().contains("Stub accepted a single shot"), qUtf8Printable(link->text()));

    stop->click();
    QCOMPARE(mode->currentText(), QString("Single shot"));
    QCOMPARE(stop->text(), QString("Start"));
    QVERIFY2(link->text().contains("Stub accepted stop"), qUtf8Printable(link->text()));

    stop->click();
    QCOMPARE(mode->currentText(), QString("Single shot"));
    QCOMPARE(stop->text(), QString("Stop"));
    QVERIFY2(link->text().contains("Stub accepted a single shot"), qUtf8Printable(link->text()));
}

void SignalTests::windowRestoresSettings()
{
    {
        MainWindow first;
        auto* host = first.findChild<QLineEdit*>("hostEdit");
        auto* port = first.findChild<QSpinBox*>("portSpin");
        auto* stub = first.findChild<QCheckBox*>("stubCheck");
        auto* thickness = first.findChild<QDoubleSpinBox*>("thicknessSpin");
        auto* mode = first.findChild<QComboBox*>("playMode");
        auto* baud = first.findChild<QSpinBox*>("comBaud");
        auto* text = first.findChild<QLineEdit*>("comText");
        QVERIFY(host && port && stub && thickness && mode && baud && text);
        host->setText("127.0.0.1");
        port->setValue(45116);
        stub->setChecked(true);
        thickness->setValue(12.5);
        mode->setCurrentText("On trigger");
        baud->setValue(9600);
        text->setText("saved");
    }

    MainWindow second;
    QCOMPARE(second.findChild<QDoubleSpinBox*>("thicknessSpin")->value(), 12.5);
    QCOMPARE(second.findChild<QLineEdit*>("hostEdit")->text(), QString("127.0.0.1"));
    QCOMPARE(second.findChild<QSpinBox*>("portSpin")->value(), 45116);
    QVERIFY(second.findChild<QCheckBox*>("stubCheck")->isChecked());
    QCOMPARE(second.findChild<QComboBox*>("playMode")->currentText(), QString("On trigger"));
    QCOMPARE(second.findChild<QSpinBox*>("comBaud")->value(), 9600);
    QCOMPARE(second.findChild<QLineEdit*>("comText")->text(), QString("saved"));
    QCOMPARE(second.findChild<QPushButton*>("stopButton")->text(), QString("Start"));
}

int main(int argc, char* argv[])
{
    qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));
    QApplication app(argc, argv);
    SignalTests tests;
    return QTest::qExec(&tests, argc, argv);
}

#include "test_signal.moc"
