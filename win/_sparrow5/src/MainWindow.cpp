#include "MainWindow.h"

#include "ThkLink.h"
#include "WaveformPlot.h"

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QDial>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMouseEvent>
#include <QWidgetAction>
#include <QFormLayout>
#include <QGridLayout>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QSettings>
#include <QSizePolicy>
#include <QSpinBox>
#include <QToolButton>
#include <QTimer>
#include <QVBoxLayout>

#include <cmath>

namespace {

struct MaterialPreset {
    const char* name;
    double velocityMps;
    bool custom;
};

const MaterialPreset kPresets[] = {
    {"Steel", 5920.0, false},
    {"Aluminum", 6320.0, false},
    {"Titanium", 6100.0, false},
    {"Copper", 4700.0, false},
    {"Custom", 0.0, true},
};

class FormMenu : public QMenu {
public:
    FormMenu(const QString& title, QWidget* parent)
        : QMenu(title, parent)
    {
    }

    void setForm(QWidget* form)
    {
        auto* action = new QWidgetAction(this);
        action->setDefaultWidget(form);
        addAction(action);
    }

protected:
    void mouseReleaseEvent(QMouseEvent* event) override
    {
        event->accept();
    }
};

struct ProbePreset {
    const char* name;
    double frequencyMhz;
    int cycles;
    double spacingMm;
    bool custom;
};

const ProbePreset kProbes[] = {
    {"Straight 5 MHz", 5.0, 3, 0.0, false},
    {"Straight 2.5 MHz", 2.5, 3, 0.0, false},
    {"Dual 5 MHz", 5.0, 2, 8.0, false},
    {"Custom", 0.0, 0, 0.0, true},
};

QDoubleSpinBox* addDouble(QFormLayout* form, const QString& label, double min, double max, double step,
                           int decimals, double value, const char* name, const QString& tip)
{
    auto* box = new QDoubleSpinBox;
    box->setRange(min, max);
    box->setSingleStep(step);
    box->setDecimals(decimals);
    box->setValue(value);
    box->setObjectName(name);
    box->setToolTip(tip);
    box->setAlignment(Qt::AlignRight);
    form->addRow(label, box);
    return box;
}

} // namespace

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    buildUi();
    connectSignals();
    restoreSettings();
    rebuild();
    if (m_sineMode)
        refreshSinePlot();
    m_linkTimer = new QTimer(this);
    m_linkTimer->setInterval(1000);
    connect(m_linkTimer, &QTimer::timeout, this, &MainWindow::pollLink);
    m_linkTimer->start();
    QTimer::singleShot(200, this, &MainWindow::pollLink);
}

void MainWindow::buildUi()
{
    setWindowTitle("UT sensor signal emulator");
    resize(1180, 720);
    setMinimumSize(980, 560);

    auto* central = new QWidget;
    auto* row = new QHBoxLayout(central);
    row->setContentsMargins(12, 12, 12, 12);
    row->setSpacing(12);

    auto* column = new QWidget;
    column->setObjectName("controlColumn");
    column->setFixedWidth(360);
    auto* columnLayout = new QVBoxLayout(column);
    columnLayout->setContentsMargins(0, 0, 0, 0);
    columnLayout->setSpacing(8);

    auto* linkRow = new QHBoxLayout;
    linkRow->setContentsMargins(2, 2, 2, 0);
    linkRow->setSpacing(8);
    m_linkLamp = new QLabel;
    m_linkLamp->setObjectName("linkLamp");
    m_linkLamp->setFixedSize(14, 14);
    m_linkState = new QLabel("No link");
    m_linkState->setObjectName("linkState");
    linkRow->addWidget(m_linkLamp, 0, Qt::AlignVCenter);
    linkRow->addWidget(m_linkState, 1, Qt::AlignVCenter);
    columnLayout->addLayout(linkRow);

    auto* scroll = new QScrollArea;
    scroll->setObjectName("controlScroll");
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto* panel = new QFrame;
    panel->setObjectName("controlPanel");
    auto* panelLayout = new QVBoxLayout(panel);
    panelLayout->setContentsMargins(0, 0, 0, 0);
    panelLayout->setSpacing(10);

    auto* modeRow = new QHBoxLayout;
    m_modePulses = new QRadioButton("Pulses");
    m_modePulses->setObjectName("modePulses");
    m_modeSine = new QRadioButton("Continuous sine");
    m_modeSine->setObjectName("modeSine");
    m_modePulses->setChecked(true);
    modeRow->addWidget(m_modePulses);
    modeRow->addWidget(m_modeSine, 1);
    panelLayout->addLayout(modeRow);

    auto* analogBox = new QGroupBox("Output");
    analogBox->setObjectName("analogBox");
    auto* analogOuter = new QVBoxLayout(analogBox);
    analogOuter->setContentsMargins(0, 4, 0, 0);
    analogOuter->setSpacing(6);
    auto* analogRow = new QHBoxLayout;
    analogRow->setSpacing(8);
    auto addKnob = [this](const QString& caption, const QString& objectName, const QString& tip, QDial** dialOut,
                          QLabel** valueOut) {
        auto* column = new QWidget;
        auto* layout = new QVBoxLayout(column);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(2);
        auto* title = new QLabel(caption);
        title->setAlignment(Qt::AlignHCenter);
        auto* dial = new QDial;
        dial->setRange(0, 4095);
        dial->setValue(2048);
        dial->setNotchesVisible(true);
        dial->setNotchTarget(16.0);
        dial->setWrapping(false);
        dial->setSingleStep(1);
        dial->setPageStep(256);
        dial->setFixedSize(96, 96);
        dial->setObjectName(objectName);
        dial->setToolTip(tip);
        auto* value = new QLabel("2048");
        value->setAlignment(Qt::AlignHCenter);
        value->setObjectName(objectName + "Value");
        auto makeStep = [this, dial](int direction, bool page, const QString& name, const QString& stepTip) {
            auto* button = new QToolButton;
            button->setObjectName(name);
            button->setAutoRaise(true);
            button->setAutoRepeat(true);
            button->setToolButtonStyle(Qt::ToolButtonTextOnly);
            button->setText(page ? (direction > 0 ? QStringLiteral("▲▲") : QStringLiteral("▼▼"))
                                 : (direction > 0 ? QStringLiteral("▲") : QStringLiteral("▼")));
            button->setToolTip(stepTip);
            button->setFixedSize(32, 18);
            connect(button, &QToolButton::clicked, dial, [dial, direction, page] {
                dial->triggerAction(page ? (direction > 0 ? QAbstractSlider::SliderPageStepAdd
                                                          : QAbstractSlider::SliderPageStepSub)
                                          : (direction > 0 ? QAbstractSlider::SliderSingleStepAdd
                                                           : QAbstractSlider::SliderSingleStepSub));
            });
            return button;
        };
        auto* arrows = new QVBoxLayout;
        arrows->setSpacing(1);
        arrows->addStretch();
        arrows->addWidget(makeStep(1, true, objectName + "UpPage", "Increase by 256"));
        arrows->addWidget(makeStep(1, false, objectName + "Up", "Increase by 1"));
        arrows->addWidget(makeStep(-1, false, objectName + "Down", "Decrease by 1"));
        arrows->addWidget(makeStep(-1, true, objectName + "DownPage", "Decrease by 256"));
        arrows->addStretch();
        auto* row = new QHBoxLayout;
        row->setSpacing(2);
        row->addWidget(dial);
        row->addLayout(arrows);
        layout->addWidget(title);
        layout->addLayout(row);
        layout->addWidget(value);
        column->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        *dialOut = dial;
        *valueOut = value;
        return column;
    };
    analogRow->addWidget(addKnob("Amplitude", "analogAmp", "Output amplitude, MCP4725 address 0x60",
                                 &m_analogAmp, &m_analogAmpValue));
    analogRow->addWidget(addKnob("Offset", "analogOffset",
                                 "Zero offset, MCP4725 address 0x61. 2048 is midscale", &m_analogOffset,
                                 &m_analogOffsetValue));
    analogRow->addStretch();
    analogOuter->addLayout(analogRow);
    panelLayout->addWidget(analogBox);

    m_pulsePanel = new QWidget;
    m_pulsePanel->setObjectName("pulsePanel");
    auto* pulseLayout = new QVBoxLayout(m_pulsePanel);
    pulseLayout->setContentsMargins(0, 0, 0, 0);
    pulseLayout->setSpacing(10);

    auto* objectBox = new QGroupBox("Object");
    auto* objectForm = new QFormLayout(objectBox);
    m_preset = new QComboBox;
    m_preset->setObjectName("presetCombo");
    for (const MaterialPreset& preset : kPresets)
        m_preset->addItem(QString::fromUtf8(preset.name));
    objectForm->addRow("Material", m_preset);
    m_thickness = addDouble(objectForm, "Thickness, mm", 0.001, 2000.0, 0.1, 3, 10.0, "thicknessSpin",
                            "Thickness along the ray");
    m_velocity = addDouble(objectForm, "Velocity, m/s", 100.0, 20000.0, 10.0, 0, 5920.0, "velocitySpin",
                           "Longitudinal wave velocity");
    auto* gaugeGrid = new QGridLayout;
    const double gaugeMm[] = {2.0, 5.0, 10.0, 20.0, 50.0, 100.0};
    for (int i = 0; i < 6; ++i) {
        const double mm = gaugeMm[i];
        auto* button = new QPushButton(QString::number(mm, 'f', 0));
        button->setObjectName(QString("gauge%1").arg(static_cast<int>(mm)));
        button->setToolTip("Reference thickness");
        gaugeGrid->addWidget(button, i / 3, i % 3);
        connect(button, &QPushButton::clicked, this, [this, mm] { m_thickness->setValue(mm); });
    }
    objectForm->addRow("Set, mm", gaugeGrid);
    pulseLayout->addWidget(objectBox);

    auto* probePage = new QWidget(this);
    probePage->setMinimumWidth(280);
    auto* probeForm = new QFormLayout(probePage);
    probeForm->setContentsMargins(10, 8, 10, 8);
    m_probe = new QComboBox;
    m_probe->setObjectName("probeCombo");
    for (const ProbePreset& probe : kProbes)
        m_probe->addItem(QString::fromUtf8(probe.name));
    probeForm->addRow("Type", m_probe);
    m_frequency = addDouble(probeForm, "Frequency, MHz", 0.5, 20.0, 0.5, 2, 5.0, "frequencySpin",
                            "Transducer center frequency");
    m_cycles = new QSpinBox;
    m_cycles->setRange(1, 30);
    m_cycles->setValue(3);
    m_cycles->setObjectName("cyclesSpin");
    m_cycles->setToolTip("Number of periods between the +/-3 sigma edges of the Gaussian envelope");
    probeForm->addRow("Cycles", m_cycles);
    m_spacing = addDouble(probeForm, "Spacing, mm", 0.0, 40.0, 0.1, 2, 0.0, "spacingSpin",
                          "Distance between element centers. Zero is a straight path");
    m_zero = addDouble(probeForm, "Zero, us", 0.0, 200.0, 0.1, 3, 0.0, "zeroSpin",
                       "Fixed delay: wear plate, cable, comparator");
    m_crosstalk = addDouble(probeForm, "Crosstalk, %", 0.0, 100.0, 1.0, 1, 0.0, "crosstalkSpin",
                            "Short pulse at time zero");

    auto* signalBox = new QGroupBox("Signal");
    auto* signalForm = new QFormLayout(signalBox);
    m_echoCount = new QSpinBox;
    m_echoCount->setRange(1, 12);
    m_echoCount->setValue(2);
    m_echoCount->setObjectName("echoCountSpin");
    signalForm->addRow("Echo count", m_echoCount);
    m_attenuation = addDouble(signalForm, "Attenuation, dB", 0.0, 60.0, 1.0, 1, 6.0, "attenuationSpin",
                              "Amplitude drop from echo to echo");
    m_amplitude = addDouble(signalForm, "Amplitude, %", 0.0, 100.0, 5.0, 1, 80.0, "amplitudeSpin",
                            "Percent of full-scale int16");
    m_noise = addDouble(signalForm, "Noise, %", 0.0, 30.0, 0.5, 2, 0.0, "noiseSpin",
                        "Noise RMS, percent of full-scale int16");
    m_divider = new QSpinBox;
    m_divider->setRange(1, 100);
    m_divider->setValue(1);
    m_divider->setObjectName("dividerSpin");
    m_divider->setToolTip("How many times to attenuate the buffer for the RX input");
    signalForm->addRow("Divider", m_divider);
    m_coating = addDouble(signalForm, "Coating, mm", 0.0, 20.0, 0.1, 2, 0.0, "coatingSpin",
                          "Layer thickness at the entry surface");
    m_coatingVelocity = addDouble(signalForm, "Layer velocity, m/s", 500.0, 10000.0, 10.0, 0, 2400.0,
                                  "coatingVelocitySpin", "Sound velocity in the coating");
    m_temperature = addDouble(signalForm, "Temperature, C", -40.0, 300.0, 1.0, 1, 20.0, "temperatureSpin",
                              "Object temperature. The coefficient reference is 20 C");
    m_tempCoeff = addDouble(signalForm, "Coeff. alpha, 1/C", -0.001, 0.001, 0.00001, 6, 0.0, "tempCoeffSpin",
                            "c(T) = c * (1 + alpha * (T - 20))");
    pulseLayout->addWidget(signalBox);

    m_summary = new QLabel;
    m_summary->setObjectName("summaryLabel");
    m_summary->setWordWrap(true);
    m_summary->setMinimumHeight(72);
    m_summary->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    pulseLayout->addWidget(m_summary);

    auto* rate = new QLabel("Sample rate 100 MHz, int16, window up to 32768 samples");
    rate->setObjectName("rateLabel");
    rate->setWordWrap(true);
    pulseLayout->addWidget(rate);

    auto* playBox = new QGroupBox("Playback");
    auto* playForm = new QFormLayout(playBox);
    m_pause = new QSpinBox;
    m_pause->setRange(0, 5000);
    m_pause->setValue(200);
    m_pause->setSuffix(" us");
    m_pause->setObjectName("pauseSpin");
    playForm->addRow("Pause", m_pause);
    m_playMode = new QComboBox;
    m_playMode->setObjectName("playMode");
    m_playMode->addItem("Free run", THK_CTRL_FREERUN);
    m_playMode->addItem("On trigger", THK_CTRL_ARM);
    m_playMode->addItem("Single shot", THK_CTRL_ONCE);
    m_playMode->setItemData(0, "Repeat the buffer by itself. Pause is the gap between repeats", Qt::ToolTipRole);
    m_playMode->setItemData(1, "Play the buffer on every TX pulse", Qt::ToolTipRole);
    m_playMode->setItemData(2, "Play the buffer once, on the next TX pulse", Qt::ToolTipRole);
    m_playMode->setCurrentIndex(0);
    playForm->addRow("Mode", m_playMode);
    pulseLayout->addWidget(playBox);
    panelLayout->addWidget(m_pulsePanel);

    m_sinePanel = new QWidget;
    m_sinePanel->setObjectName("sinePanel");
    auto* sineLayout = new QVBoxLayout(m_sinePanel);
    sineLayout->setContentsMargins(0, 0, 0, 0);
    sineLayout->setSpacing(10);
    auto* sineBox = new QGroupBox("Sine");
    auto* sineForm = new QFormLayout(sineBox);
    m_sineFreq = addDouble(sineForm, "Frequency, MHz", 0.01, 20.0, 0.01, 3, 5.0, "sineFreq",
                           "Continuous sine frequency");
    m_sineAmplitude = addDouble(sineForm, "Amplitude, %", 0.0, 100.0, 5.0, 1, 80.0, "sineAmplitude",
                                "Percent of full-scale int16");
    m_sineSummary = new QLabel;
    m_sineSummary->setObjectName("sineSummary");
    m_sineSummary->setWordWrap(true);
    sineForm->addRow(m_sineSummary);
    sineLayout->addWidget(sineBox);
    m_sinePanel->setVisible(false);
    panelLayout->addWidget(m_sinePanel);

    auto* networkPage = new QWidget(this);
    networkPage->setMinimumWidth(280);
    auto* netForm = new QFormLayout(networkPage);
    netForm->setContentsMargins(10, 8, 10, 8);
    m_host = new QLineEdit("192.168.1.50");
    m_host->setObjectName("hostEdit");
    netForm->addRow("Address", m_host);
    m_port = new QSpinBox;
    m_port->setRange(1, 65535);
    m_port->setValue(45000);
    m_port->setObjectName("portSpin");
    netForm->addRow("Port", m_port);
    m_stubCheck = new QCheckBox("Stub receiver");
    m_stubCheck->setObjectName("stubCheck");
    netForm->addRow(QString(), m_stubCheck);

    auto* comPage = new QWidget(this);
    comPage->setMinimumWidth(220);
    auto* comForm = new QFormLayout(comPage);
    comForm->setContentsMargins(10, 8, 10, 8);
    m_comBaud = new QSpinBox;
    m_comBaud->setRange(1200, 3000000);
    m_comBaud->setValue(115200);
    m_comBaud->setObjectName("comBaud");
    comForm->addRow("Baud", m_comBaud);

    auto* linkBox = new QGroupBox("Link");
    auto* linkForm = new QFormLayout(linkBox);
    m_stop = new QPushButton("Start");
    m_stop->setObjectName("stopButton");
    linkForm->addRow(QString(), m_stop);
    m_comText = new QLineEdit;
    m_comText->setObjectName("comText");
    m_comText->setPlaceholderText("Line is sent to USB-COM");
    linkForm->addRow("Text", m_comText);
    m_comSend = new QPushButton("Send to COM");
    m_comSend->setObjectName("comButton");
    linkForm->addRow(QString(), m_comSend);
    m_link = new QLabel("Stub is off");
    m_link->setObjectName("linkLabel");
    m_link->setWordWrap(true);
    linkForm->addRow(m_link);
    panelLayout->addWidget(linkBox);
    panelLayout->addStretch();

    m_stub = new ThkStubReceiver(this);
    m_sender = new ThkSender(this);

    m_plot = new WaveformPlot;

    scroll->setWidget(panel);
    columnLayout->addWidget(scroll, 1);
    row->addWidget(column);
    row->addWidget(m_plot, 1);
    setCentralWidget(central);

    auto* probeMenu = new FormMenu("Probe", this);
    probeMenu->setForm(probePage);
    menuBar()->addMenu(probeMenu);
    auto* comMenu = new FormMenu("COM", this);
    comMenu->setForm(comPage);
    menuBar()->addMenu(comMenu);
    auto* networkMenu = new FormMenu("Network", this);
    networkMenu->setForm(networkPage);
    menuBar()->addMenu(networkMenu);
    setLinkLamp(0);

    setStyleSheet(
        "QWidget { background: #f4f6f5; color: #1c2420; font-family: 'Segoe UI'; font-size: 10pt; }"
        "QFrame#controlPanel, QScrollArea#controlScroll { background: #f4f6f5; }"
        "QScrollArea#controlScroll > QWidget > QWidget { background: #f4f6f5; }"
        "QGroupBox { font-weight: 600; border: 1px solid #d5ddd8; border-radius: 6px; margin-top: 12px; padding: 10px 8px 8px 8px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 4px; }"
        "QComboBox, QSpinBox, QDoubleSpinBox, QLineEdit { background: white; border: 1px solid #c5d0ca; border-radius: 3px; padding: 2px 4px; min-height: 22px; }"
        "QPushButton { background: #1f6b4a; color: white; border: none; border-radius: 4px; padding: 8px 12px; font-weight: 600; }"
        "QPushButton:disabled { background: #b7c4bc; color: #f4f6f5; }"
        "QLabel#rateLabel { color: #5c6b64; font-size: 9pt; }"
        "QMenuBar { background: #f4f6f5; }"
        "QMenuBar::item:selected { background: #d5ddd8; }"
        "QMenu { background: white; }"
        "WaveformPlot { border-radius: 6px; }");
}

void MainWindow::connectSignals()
{
    auto refresh = [this] {
        if (!m_updating)
            rebuild();
    };

    connect(m_thickness, qOverload<double>(&QDoubleSpinBox::valueChanged), this, refresh);
    connect(m_frequency, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double) {
        if (m_updating)
            return;
        syncProbePreset();
        rebuild();
    });
    connect(m_spacing, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double) {
        if (m_updating)
            return;
        syncProbePreset();
        rebuild();
    });
    connect(m_coating, qOverload<double>(&QDoubleSpinBox::valueChanged), this, refresh);
    connect(m_coatingVelocity, qOverload<double>(&QDoubleSpinBox::valueChanged), this, refresh);
    connect(m_temperature, qOverload<double>(&QDoubleSpinBox::valueChanged), this, refresh);
    connect(m_tempCoeff, qOverload<double>(&QDoubleSpinBox::valueChanged), this, refresh);
    connect(m_zero, qOverload<double>(&QDoubleSpinBox::valueChanged), this, refresh);
    connect(m_crosstalk, qOverload<double>(&QDoubleSpinBox::valueChanged), this, refresh);
    connect(m_attenuation, qOverload<double>(&QDoubleSpinBox::valueChanged), this, refresh);
    connect(m_amplitude, qOverload<double>(&QDoubleSpinBox::valueChanged), this, refresh);
    connect(m_noise, qOverload<double>(&QDoubleSpinBox::valueChanged), this, refresh);
    connect(m_cycles, qOverload<int>(&QSpinBox::valueChanged), this, [this](int) {
        if (m_updating)
            return;
        syncProbePreset();
        rebuild();
    });
    connect(m_echoCount, qOverload<int>(&QSpinBox::valueChanged), this, refresh);
    connect(m_divider, qOverload<int>(&QSpinBox::valueChanged), this, refresh);

    connect(m_velocity, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double) {
        if (m_updating)
            return;
        syncPresetFromVelocity();
        rebuild();
    });

    connect(m_preset, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int index) {
        if (m_updating)
            return;
        if (!kPresets[index].custom) {
            m_updating = true;
            m_velocity->setValue(kPresets[index].velocityMps);
            m_updating = false;
        }
        rebuild();
    });

    connect(m_probe, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int index) {
        if (m_updating)
            return;
        if (!kProbes[index].custom) {
            m_updating = true;
            m_frequency->setValue(kProbes[index].frequencyMhz);
            m_cycles->setValue(kProbes[index].cycles);
            m_spacing->setValue(kProbes[index].spacingMm);
            m_updating = false;
        }
        rebuild();
    });

    connect(m_stubCheck, &QCheckBox::toggled, this, &MainWindow::toggleStub);
    connect(m_playMode, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int) {
        saveSettings();
        startPulses();
    });
    connect(m_pause, qOverload<int>(&QSpinBox::valueChanged), this, [this](int) {
        saveSettings();
        if (m_playing && m_playMode->currentData().toInt() == THK_CTRL_FREERUN)
            sendCommand(THK_CTRL_FREERUN);
    });
    connect(m_stop, &QPushButton::clicked, this, [this] {
        if (m_playing || m_sineOn)
            sendCommand(THK_CTRL_STOP);
        else if (m_sineMode)
            sendSine();
        else
            startPulses();
    });
    connect(m_modeSine, &QRadioButton::toggled, this, [this](bool on) {
        if (on)
            applyMode(true);
    });
    connect(m_modePulses, &QRadioButton::toggled, this, [this](bool on) {
        if (on)
            applyMode(false);
    });
    connect(m_sineFreq, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double) {
        if (m_updating)
            return;
        if (m_sineMode)
            refreshSinePlot();
        if (m_sineOn)
            sendSine();
        saveSettings();
    });
    connect(m_sineAmplitude, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double) {
        if (m_updating)
            return;
        if (m_sineMode)
            refreshSinePlot();
        if (m_sineOn)
            sendSine();
        saveSettings();
    });
    connect(m_analogAmp, &QDial::valueChanged, this, [this](int value) {
        m_analogAmpValue->setText(QString::number(value));
        saveSettings();
        sendAnalog();
    });
    connect(m_analogOffset, &QDial::valueChanged, this, [this](int value) {
        m_analogOffsetValue->setText(QString::number(value));
        saveSettings();
        sendAnalog();
    });
    connect(m_comSend, &QPushButton::clicked, this, &MainWindow::sendCom);
    connect(m_comText, &QLineEdit::textChanged, this, [this](const QString&) { saveSettings(); });
    connect(m_comText, &QLineEdit::returnPressed, this, &MainWindow::sendCom);
    connect(m_comBaud, qOverload<int>(&QSpinBox::valueChanged), this, [this](int) { saveSettings(); });
    connect(m_port, qOverload<int>(&QSpinBox::valueChanged), this, [this](int) {
        saveSettings();
        if (m_stubCheck->isChecked())
            toggleStub(true);
        else
            pollLink();
    });
    connect(m_host, &QLineEdit::textChanged, this, [this](const QString&) { saveSettings(); });
    connect(m_host, &QLineEdit::editingFinished, this, [this] {
        if (m_stubCheck->isChecked())
            toggleStub(true);
        else
            pollLink();
    });
}

void MainWindow::syncPresetFromVelocity()
{
    const double velocity = m_velocity->value();
    int index = 0;
    for (int i = 0; i < static_cast<int>(sizeof(kPresets) / sizeof(kPresets[0])); ++i) {
        if (kPresets[i].custom)
            index = i;
    }
    for (int i = 0; i < static_cast<int>(sizeof(kPresets) / sizeof(kPresets[0])); ++i) {
        if (!kPresets[i].custom && std::abs(kPresets[i].velocityMps - velocity) < 0.5)
            index = i;
    }
    m_updating = true;
    m_preset->setCurrentIndex(index);
    m_updating = false;
}

void MainWindow::syncProbePreset()
{
    const double frequency = m_frequency->value();
    const int cycles = m_cycles->value();
    const double spacing = m_spacing->value();
    int index = 0;
    for (int i = 0; i < static_cast<int>(sizeof(kProbes) / sizeof(kProbes[0])); ++i) {
        if (kProbes[i].custom)
            index = i;
    }
    for (int i = 0; i < static_cast<int>(sizeof(kProbes) / sizeof(kProbes[0])); ++i) {
        if (!kProbes[i].custom && kProbes[i].cycles == cycles && std::abs(kProbes[i].frequencyMhz - frequency) < 0.01
            && std::abs(kProbes[i].spacingMm - spacing) < 0.05)
            index = i;
    }
    m_updating = true;
    m_probe->setCurrentIndex(index);
    m_updating = false;
}

SignalParams MainWindow::params() const
{
    SignalParams result;
    result.thicknessMm = m_thickness->value();
    result.velocityMps = m_velocity->value();
    result.frequencyMhz = m_frequency->value();
    result.cycles = m_cycles->value();
    result.echoCount = m_echoCount->value();
    result.attenuationDb = m_attenuation->value();
    result.zeroUs = m_zero->value();
    result.amplitudePercent = m_amplitude->value();
    result.crosstalkPercent = m_crosstalk->value();
    result.noisePercent = m_noise->value();
    result.outputDivider = m_divider->value();
    result.elementSpacingMm = m_spacing->value();
    result.coatingMm = m_coating->value();
    result.coatingVelocityMps = m_coatingVelocity->value();
    result.temperatureC = m_temperature->value();
    result.tempCoeffPerC = m_tempCoeff->value();
    return result;
}

void MainWindow::rebuild()
{
    m_params = params();
    m_wave = SignalModel::generate(m_params);
    const bool inDac = SignalModel::fitsDac(m_wave.requiredLength);
    if (!m_sineMode)
        m_plot->setWaveform(m_wave.samples, m_wave.valid ? m_wave.firstEchoSample : -1);

    QString text;
    if (!SignalModel::paramsValid(m_params) || m_wave.requiredLength < 0) {
        text = "Transmit buffer is not formed: the parameters do not give a finite length.";
    } else if (!m_wave.valid) {
        text = QString("First echo: %1 us, sample %2. Need %3 samples - transmit buffer is not formed (limit %4).")
                   .arg(m_wave.firstEchoUs, 0, 'f', 6)
                   .arg(m_wave.firstEchoSample)
                   .arg(m_wave.requiredLength)
                   .arg(SignalModel::kMaxSamples);
    } else {
        const double shownMm = SignalModel::readingMm(m_params, m_wave.firstEchoSample);
        const double stepMm = m_params.velocityMps / (2.0 * SignalModel::kSampleRateHz) * 1.0e3;
        const QString dac = inDac ? QString("fits in %1").arg(SignalModel::kDacSamples)
                                  : QString("need %1, limit %2").arg(m_wave.requiredLength).arg(SignalModel::kDacSamples);
        text = QString("First echo: %1 us, sample %2. Buffer: %3 samples, fits in %4. "
                       "Gauge: %5 mm, step %6 mm. DAC: %7.")
                   .arg(m_wave.firstEchoUs, 0, 'f', 6)
                   .arg(m_wave.firstEchoSample)
                   .arg(m_wave.requiredLength)
                   .arg(SignalModel::kMaxSamples)
                   .arg(shownMm, 0, 'f', 3)
                   .arg(stepMm, 0, 'f', 3)
                   .arg(dac);
    }
    m_summary->setText(text);
    m_summary->setStyleSheet(m_wave.valid && inDac ? "color: #163028;" : "color: #9a3412; font-weight: 600;");
    saveSettings();
    if (inDac)
        sendWave();
}

void MainWindow::toggleStub(bool enabled)
{
    saveSettings();
    if (!enabled) {
        m_stub->close();
        m_link->setText("Stub is off");
        pollLink();
        return;
    }
    if (!m_stub->listen(quint16(m_port->value()))) {
        m_updating = true;
        m_stubCheck->setChecked(false);
        m_updating = false;
        m_link->setText("Could not bind the port");
        return;
    }
    m_link->setText(QString("Stub is listening on %1").arg(m_stub->port()));
    pollLink();
}

void MainWindow::setLinkLamp(int state)
{
    const char* color = "#9aa39c";
    const char* text = "No link";
    if (state > 0) {
        color = "#1f8a4c";
        text = "Connected";
    } else if (state < 0) {
        color = "#c44536";
        text = "No link";
    }
    m_linkLamp->setStyleSheet(QString("background: %1; border-radius: 7px;").arg(color));
    m_linkState->setStyleSheet(QString("color: %1; font-weight: 600; background: transparent;").arg(color));
    const QString where = m_host && m_port ? QString("%1:%2").arg(m_host->text().trimmed()).arg(m_port->value()) : QString();
    m_linkState->setText(where.isEmpty() ? QString(text) : QString("%1    %2").arg(text, where));
}

void MainWindow::pollLink()
{
    if (m_sending || m_polling || !m_sender)
        return;
    const QHostAddress address(m_host->text().trimmed());
    if (address.isNull()) {
        setLinkLamp(-1);
        return;
    }
    m_polling = true;
    const bool up = m_sender->ping(address, quint16(m_port->value()), nullptr);
    m_polling = false;
    setLinkLamp(up ? 1 : -1);
    if (m_analogPending)
        sendAnalog();
    if (m_sinePending)
        sendSine();
    if (m_pulseStartPending) {
        m_pulseStartPending = false;
        m_wavePending = false;
        startPulses();
    } else if (m_playPending)
        sendCommand(m_playPending);
    else if (m_wavePending) {
        m_wavePending = false;
        sendWave();
    }
}

bool MainWindow::uploadWave()
{
    if (!m_wave.valid || !SignalModel::fitsDac(m_wave.requiredLength) || m_sending)
        return false;
    const QHostAddress address(m_host->text().trimmed());
    if (address.isNull()) {
        m_link->setText("Invalid address");
        return false;
    }

    m_sending = true;
    const ThkUploadResult result = m_sender->upload(address, quint16(m_port->value()), m_wave.samples,
                                                    quint32(SignalModel::kSampleRateHz));
    m_sending = false;

    if (!result.ok) {
        m_link->setText(result.error);
        return false;
    }
    if (m_stub->isListening() && address.isLoopback() && m_stub->port() == quint16(m_port->value())) {
        const bool same = m_stub->complete() && m_stub->samples() == m_wave.samples;
        m_link->setText(same ? QString("Received %1 samples").arg(m_stub->samples().size())
                             : QString("Sent, the receiver assembled a different buffer"));
        return same;
    }
    m_link->setText(QString("Sent %1 samples, retries %2").arg(m_wave.samples.size()).arg(result.retries));
    return true;
}

void MainWindow::sendWave()
{
    if (m_sineMode || m_loading)
        return;
    if (m_sending || m_polling) {
        m_wavePending = true;
        return;
    }
    if (!m_wave.valid || !SignalModel::fitsDac(m_wave.requiredLength))
        return;
    do {
        m_wavePending = false;
        if (m_sineOn) {
            sendCommand(THK_CTRL_STOP);
            if (m_sineOn)
                return;
        }
        const bool ok = uploadWave();
        if (ok && m_playing) {
            const int id = m_playMode->currentData().toInt();
            if (id > 0)
                sendCommand(quint16(id));
        }
        if (!ok && !m_wavePending)
            return;
    } while (m_wavePending);
}

void MainWindow::setPlaybackEnabled(bool enabled)
{
    m_playMode->setEnabled(enabled);
    m_stop->setEnabled(enabled);
}

void MainWindow::updateRunButton()
{
    m_stop->setText((m_playing || m_sineOn) ? QString("Stop") : QString("Start"));
}

void MainWindow::startPulses()
{
    if (m_sending || m_polling) {
        m_pulseStartPending = true;
        return;
    }
    const int id = m_playMode->currentData().toInt();
    if (id < 0) {
        m_link->setText("Select a playback mode");
        return;
    }
    if (!m_wave.valid || !SignalModel::fitsDac(m_wave.requiredLength)) {
        m_link->setText("Transmit buffer does not fit the DAC");
        return;
    }
    if (m_sineOn || m_playing) {
        sendCommand(THK_CTRL_STOP);
        if (m_sineOn || m_playing)
            return;
    }
    if (!uploadWave())
        return;
    sendCommand(quint16(id));
}

void MainWindow::sendCommand(quint16 command)
{
    if (m_sending || m_polling) {
        m_playPending = command;
        return;
    }
    const QHostAddress address(m_host->text().trimmed());
    if (address.isNull()) {
        m_playPending = 0;
        m_link->setText("Invalid address");
        return;
    }
    m_sending = true;
    setPlaybackEnabled(false);
    m_comSend->setEnabled(false);
    bool ok = false;
    ThkStatusBody status = {};
    do {
        if (m_playPending) {
            command = m_playPending;
            m_playPending = 0;
        }
        status = {};
        ok = m_sender->command(address, quint16(m_port->value()), command, quint32(m_pause->value()), &status);
    } while (ok && m_playPending);
    m_sending = false;
    setPlaybackEnabled(true);
    m_comSend->setEnabled(true);
    const bool stub = m_stub->isListening() && address.isLoopback() && m_stub->port() == quint16(m_port->value());
    const QString echo = m_wave.valid ? QString(" Model first echo %1 us.").arg(m_wave.firstEchoUs, 0, 'f', 3) : QString();
    if (!ok) {
        m_link->setText("No reply to the command");
        updateRunButton();
        return;
    }
    m_sineOn = false;
    if (command == THK_CTRL_STOP)
        m_playing = false;
    else if (command == THK_CTRL_FREERUN || command == THK_CTRL_ARM || command == THK_CTRL_ONCE)
        m_playing = true;
    updateRunButton();
    if (command == THK_CTRL_FREERUN)
        m_link->setText((stub ? QString("Stub accepted free run")
                              : QString("Free run, pause %1 us").arg(m_pause->value()))
                        + echo);
    else if (command == THK_CTRL_ARM)
        m_link->setText((stub ? QString("Stub accepted trigger mode")
                              : QString("Waiting for the TX trigger, count %1").arg(status.trigger_count))
                        + echo);
    else if (command == THK_CTRL_ONCE)
        m_link->setText((stub ? QString("Stub accepted a single shot")
                              : QString("Single shot on the TX trigger, count %1").arg(status.trigger_count))
                        + echo);
    else
        m_link->setText(stub ? QString("Stub accepted stop") : QString("Playback stopped"));
}

void MainWindow::sendSine()
{
    if (m_sending || m_polling) {
        m_sinePending = true;
        return;
    }
    const QHostAddress address(m_host->text().trimmed());
    if (address.isNull()) {
        m_sinePending = false;
        m_link->setText("Invalid address");
        return;
    }
    m_sending = true;
    setPlaybackEnabled(false);
    m_comSend->setEnabled(false);
    bool ok = false;
    quint32 hz = 0;
    int percent = 0;
    do {
        m_sinePending = false;
        hz = quint32(qRound64(m_sineFreq->value() * 1000000.0));
        percent = qRound(m_sineAmplitude->value());
        if (percent < 0)
            percent = 0;
        if (percent > 100)
            percent = 100;
        ThkStatusBody status = {};
        ok = m_sender->command(address, quint16(m_port->value()), THK_CTRL_SINE, hz, &status, quint16(percent));
    } while (ok && m_sinePending);
    m_sending = false;
    setPlaybackEnabled(true);
    m_comSend->setEnabled(true);
    m_sineOn = ok;
    if (ok)
        m_playing = false;
    updateRunButton();
    const bool stub = m_stub->isListening() && address.isLoopback() && m_stub->port() == quint16(m_port->value());
    if (!ok) {
        m_link->setText("No reply to the sine");
        return;
    }
    m_link->setText((stub ? QString("Stub accepted sine %1 MHz, %2%") : QString("Continuous sine %1 MHz, %2%"))
                        .arg(m_sineFreq->value(), 0, 'f', 3)
                        .arg(percent));
}

void MainWindow::applyMode(bool sine)
{
    m_sineMode = sine;
    m_pulsePanel->setVisible(!sine);
    m_sinePanel->setVisible(sine);
    saveSettings();
    if (sine) {
        refreshSinePlot();
        sendSine();
        return;
    }
    rebuild();
    startPulses();
}

void MainWindow::saveSettings() const
{
    if (m_loading)
        return;
    QSettings settings;
    settings.setValue("sineMode", m_sineMode);
    settings.setValue("preset", m_preset->currentIndex());
    settings.setValue("thickness", m_thickness->value());
    settings.setValue("velocity", m_velocity->value());
    settings.setValue("probe", m_probe->currentIndex());
    settings.setValue("frequency", m_frequency->value());
    settings.setValue("cycles", m_cycles->value());
    settings.setValue("spacing", m_spacing->value());
    settings.setValue("zero", m_zero->value());
    settings.setValue("crosstalk", m_crosstalk->value());
    settings.setValue("echoCount", m_echoCount->value());
    settings.setValue("attenuation", m_attenuation->value());
    settings.setValue("amplitude", m_amplitude->value());
    settings.setValue("noise", m_noise->value());
    settings.setValue("divider", m_divider->value());
    settings.setValue("coating", m_coating->value());
    settings.setValue("coatingVelocity", m_coatingVelocity->value());
    settings.setValue("temperature", m_temperature->value());
    settings.setValue("tempCoeff", m_tempCoeff->value());
    settings.setValue("pause", m_pause->value());
    settings.setValue("playMode", m_playMode->currentData().toInt());
    settings.setValue("sineFreq", m_sineFreq->value());
    settings.setValue("sineAmplitude", m_sineAmplitude->value());
    settings.setValue("analogAmp", m_analogAmp->value());
    settings.setValue("analogOffset", m_analogOffset->value());
    settings.setValue("host", m_host->text());
    settings.setValue("port", m_port->value());
    settings.setValue("stub", m_stubCheck->isChecked());
    settings.setValue("comText", m_comText->text());
    settings.setValue("comBaud", m_comBaud->value());
}

void MainWindow::restoreSettings()
{
    QSettings settings;
    if (settings.allKeys().isEmpty())
        return;
    m_loading = true;
    const auto restoreDouble = [&settings](QDoubleSpinBox* box, const char* key) {
        box->blockSignals(true);
        box->setValue(settings.value(key, box->value()).toDouble());
        box->blockSignals(false);
    };
    const auto restoreInt = [&settings](QSpinBox* box, const char* key) {
        box->blockSignals(true);
        box->setValue(settings.value(key, box->value()).toInt());
        box->blockSignals(false);
    };
    const auto restoreIndex = [&settings](QComboBox* box, const char* key) {
        const int index = settings.value(key, box->currentIndex()).toInt();
        if (index < 0 || index >= box->count())
            return;
        box->blockSignals(true);
        box->setCurrentIndex(index);
        box->blockSignals(false);
    };
    restoreIndex(m_preset, "preset");
    restoreDouble(m_thickness, "thickness");
    restoreDouble(m_velocity, "velocity");
    restoreIndex(m_probe, "probe");
    restoreDouble(m_frequency, "frequency");
    restoreInt(m_cycles, "cycles");
    restoreDouble(m_spacing, "spacing");
    restoreDouble(m_zero, "zero");
    restoreDouble(m_crosstalk, "crosstalk");
    restoreInt(m_echoCount, "echoCount");
    restoreDouble(m_attenuation, "attenuation");
    restoreDouble(m_amplitude, "amplitude");
    restoreDouble(m_noise, "noise");
    restoreInt(m_divider, "divider");
    restoreDouble(m_coating, "coating");
    restoreDouble(m_coatingVelocity, "coatingVelocity");
    restoreDouble(m_temperature, "temperature");
    restoreDouble(m_tempCoeff, "tempCoeff");
    restoreInt(m_pause, "pause");
    const int playCommand = settings.value("playMode", THK_CTRL_FREERUN).toInt();
    const int playIndex = m_playMode->findData(playCommand);
    if (playIndex >= 0) {
        m_playMode->blockSignals(true);
        m_playMode->setCurrentIndex(playIndex);
        m_playMode->blockSignals(false);
    }
    restoreDouble(m_sineFreq, "sineFreq");
    restoreDouble(m_sineAmplitude, "sineAmplitude");
    m_analogAmp->blockSignals(true);
    m_analogAmp->setValue(settings.value("analogAmp", m_analogAmp->value()).toInt());
    m_analogAmp->blockSignals(false);
    m_analogOffset->blockSignals(true);
    m_analogOffset->setValue(settings.value("analogOffset", m_analogOffset->value()).toInt());
    m_analogOffset->blockSignals(false);
    m_analogAmpValue->setText(QString::number(m_analogAmp->value()));
    m_analogOffsetValue->setText(QString::number(m_analogOffset->value()));
    m_host->blockSignals(true);
    m_host->setText(settings.value("host", m_host->text()).toString());
    m_host->blockSignals(false);
    restoreInt(m_port, "port");
    m_stubCheck->blockSignals(true);
    m_stubCheck->setChecked(settings.value("stub", false).toBool());
    m_stubCheck->blockSignals(false);
    m_comText->blockSignals(true);
    m_comText->setText(settings.value("comText", m_comText->text()).toString());
    m_comText->blockSignals(false);
    restoreInt(m_comBaud, "comBaud");
    m_sineMode = settings.value("sineMode", false).toBool();
    m_modeSine->blockSignals(true);
    m_modePulses->blockSignals(true);
    if (m_sineMode)
        m_modeSine->setChecked(true);
    else
        m_modePulses->setChecked(true);
    m_modeSine->blockSignals(false);
    m_modePulses->blockSignals(false);
    m_pulsePanel->setVisible(!m_sineMode);
    m_sinePanel->setVisible(m_sineMode);
    m_loading = false;
    if (m_stubCheck->isChecked())
        toggleStub(true);
}

void MainWindow::refreshSinePlot()
{
    const Waveform sine = SignalModel::continuousSine(m_sineFreq->value(), m_sineAmplitude->value());
    m_plot->setWaveform(sine.samples, -1);
    m_sineSummary->setText(QString("%1 MHz, period %2 us, amplitude %3%")
                               .arg(m_sineFreq->value(), 0, 'f', 3)
                               .arg(sine.firstEchoUs, 0, 'f', 3)
                               .arg(m_sineAmplitude->value(), 0, 'f', 0));
}

void MainWindow::sendAnalog()
{
    if (m_sending || m_polling) {
        m_analogPending = true;
        return;
    }
    const QHostAddress address(m_host->text().trimmed());
    if (address.isNull()) {
        m_analogPending = false;
        m_link->setText("Invalid address");
        return;
    }
    m_sending = true;
    quint32 amplitude = 0;
    quint16 offset = 0;
    ThkStatusBody status = {};
    bool ok = false;
    do {
        m_analogPending = false;
        amplitude = quint32(m_analogAmp->value());
        offset = quint16(m_analogOffset->value());
        status = {};
        ok = m_sender->command(address, quint16(m_port->value()), THK_CTRL_ANALOG, amplitude, &status, offset);
    } while (ok && m_analogPending);
    m_sending = false;
    const bool stub = m_stub->isListening() && address.isLoopback() && m_stub->port() == quint16(m_port->value());
    if (!ok) {
        m_link->setText("No reply for the output");
        return;
    }
    if (status.last_error == THK_ERR_I2C) {
        m_link->setText("No MCP4725");
        return;
    }
    m_link->setText((stub ? QString("Stub accepted amplitude %1, offset %2")
                          : QString("Amplitude %1, offset %2"))
                        .arg(amplitude)
                        .arg(offset));
}

void MainWindow::sendCom()
{
    if (m_sending)
        return;
    const QString text = m_comText->text();
    if (text.isEmpty()) {
        m_link->setText("Empty text");
        return;
    }
    const QByteArray bytes = text.toUtf8() + "\r\n";
    if (bytes.size() > int(THK_MAX_PAYLOAD) - int(sizeof(ThkTextInfo))) {
        m_link->setText("Text is too long");
        return;
    }
    const QHostAddress address(m_host->text().trimmed());
    if (address.isNull()) {
        m_link->setText("Invalid address");
        return;
    }
    m_sending = true;
    m_comSend->setEnabled(false);
    quint16 ackStatus = 0;
    const bool ok = m_sender->sendText(address, quint16(m_port->value()), quint32(m_comBaud->value()), bytes, &ackStatus);
    m_sending = false;
    m_comSend->setEnabled(true);
    const bool stub = m_stub->isListening() && address.isLoopback() && m_stub->port() == quint16(m_port->value());
    if (ok) {
        m_link->setText(stub ? QString("Stub accepted text, %1 bytes").arg(bytes.size())
                             : QString("Sent %1 bytes to COM, %2 baud").arg(bytes.size()).arg(m_comBaud->value()));
        return;
    }
    if (ackStatus == THK_ERR_COM)
        m_link->setText("No USB-COM");
    else if (ackStatus == 0xffff)
        m_link->setText("No reply to the text");
    else
        m_link->setText("Text was not accepted");
}
