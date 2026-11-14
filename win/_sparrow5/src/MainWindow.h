#pragma once

#include "SignalModel.h"

#include <QMainWindow>

class QCheckBox;
class QDial;
class QRadioButton;
class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QTimer;
class ThkSender;
class ThkStubReceiver;
class WaveformPlot;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void buildUi();
    void connectSignals();
    void syncPresetFromVelocity();
    void syncProbePreset();
    void rebuild();
    void toggleStub(bool enabled);
    void sendWave();
    bool uploadWave();
    void sendCommand(quint16 command);
    void sendSine();
    void sendAnalog();
    void sendCom();
    void applyMode(bool sine);
    void startPulses();
    void refreshSinePlot();
    void setPlaybackEnabled(bool enabled);
    void updateRunButton();
    void pollLink();
    void setLinkLamp(int state);
    void saveSettings() const;
    void restoreSettings();
    SignalParams params() const;

    QComboBox* m_preset = nullptr;
    QComboBox* m_probe = nullptr;
    QDoubleSpinBox* m_spacing = nullptr;
    QDoubleSpinBox* m_coating = nullptr;
    QDoubleSpinBox* m_coatingVelocity = nullptr;
    QDoubleSpinBox* m_temperature = nullptr;
    QDoubleSpinBox* m_tempCoeff = nullptr;
    QDoubleSpinBox* m_thickness = nullptr;
    QDoubleSpinBox* m_velocity = nullptr;
    QDoubleSpinBox* m_frequency = nullptr;
    QSpinBox* m_cycles = nullptr;
    QSpinBox* m_echoCount = nullptr;
    QDoubleSpinBox* m_attenuation = nullptr;
    QDoubleSpinBox* m_zero = nullptr;
    QDoubleSpinBox* m_amplitude = nullptr;
    QDoubleSpinBox* m_crosstalk = nullptr;
    QDoubleSpinBox* m_noise = nullptr;
    QSpinBox* m_divider = nullptr;
    QLabel* m_summary = nullptr;
    QWidget* m_pulsePanel = nullptr;
    QWidget* m_sinePanel = nullptr;
    QRadioButton* m_modePulses = nullptr;
    QRadioButton* m_modeSine = nullptr;
    QDoubleSpinBox* m_sineAmplitude = nullptr;
    QLabel* m_sineSummary = nullptr;
    QDial* m_analogAmp = nullptr;
    QDial* m_analogOffset = nullptr;
    QLabel* m_analogAmpValue = nullptr;
    QLabel* m_analogOffsetValue = nullptr;
    QLineEdit* m_host = nullptr;
    QSpinBox* m_port = nullptr;
    QCheckBox* m_stubCheck = nullptr;
    QComboBox* m_playMode = nullptr;
    QPushButton* m_stop = nullptr;
    QDoubleSpinBox* m_sineFreq = nullptr;
    QSpinBox* m_pause = nullptr;
    QLineEdit* m_comText = nullptr;
    QSpinBox* m_comBaud = nullptr;
    QPushButton* m_comSend = nullptr;
    QLabel* m_link = nullptr;
    QLabel* m_linkLamp = nullptr;
    QLabel* m_linkState = nullptr;
    QTimer* m_linkTimer = nullptr;
    WaveformPlot* m_plot = nullptr;
    ThkStubReceiver* m_stub = nullptr;
    ThkSender* m_sender = nullptr;
    bool m_updating = false;
    bool m_loading = false;
    bool m_sending = false;
    bool m_polling = false;
    bool m_analogPending = false;
    bool m_sinePending = false;
    quint16 m_playPending = 0;
    bool m_pulseStartPending = false;
    bool m_wavePending = false;
    bool m_playing = false;
    bool m_sineOn = false;
    bool m_sineMode = false;
    SignalParams m_params;
    Waveform m_wave;
};
