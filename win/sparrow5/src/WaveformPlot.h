#pragma once

#include <QWidget>

#include <cstdint>
#include <vector>

class QLabel;
class QScrollBar;
class QSlider;
class WaveformView;

class WaveformPlot : public QWidget {
    friend class WaveformView;

public:
    explicit WaveformPlot(QWidget* parent = nullptr);

    void setWaveform(const std::vector<std::int16_t>& samples, int echoSample);
    const std::vector<std::int16_t>& samples() const { return m_samples; }

private:
    void applyZoom(int zoom, double anchorSample, double anchorFraction);
    void clampOrigin();
    void syncScrollBar();
    void zoomAt(int steps, int cursorX);
    int visibleCount() const;

    WaveformView* m_view = nullptr;
    QScrollBar* m_scroll = nullptr;
    QSlider* m_zoomSlider = nullptr;
    QLabel* m_zoomLabel = nullptr;
    std::vector<std::int16_t> m_samples;
    int m_echoSample = -1;
    int m_origin = 0;
    int m_zoom = 1;
};
