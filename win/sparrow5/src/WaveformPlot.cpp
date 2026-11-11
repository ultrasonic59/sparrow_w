#include "WaveformPlot.h"

#include "SignalModel.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QScrollBar>
#include <QSlider>
#include <QVBoxLayout>
#include <QWheelEvent>

#include <algorithm>
#include <cmath>

namespace {

constexpr int kLeft = 64;
constexpr int kRight = 16;
constexpr int kTop = 18;
constexpr int kBottom = 36;
constexpr int kMaxZoom = 64;

} // namespace

class WaveformView : public QWidget {
public:
    explicit WaveformView(WaveformPlot* plot)
        : QWidget(plot)
        , m_plot(plot)
    {
        setMinimumHeight(220);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setFocusPolicy(Qt::WheelFocus);
    }

protected:
    void paintEvent(QPaintEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    WaveformPlot* m_plot;
};

WaveformPlot::WaveformPlot(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(480, 320);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setObjectName("waveformPlot");

    m_view = new WaveformView(this);

    auto* zoomCaption = new QLabel("Zoom");
    m_zoomSlider = new QSlider(Qt::Horizontal);
    m_zoomSlider->setObjectName("zoomSlider");
    m_zoomSlider->setRange(1, kMaxZoom);
    m_zoomSlider->setValue(1);
    m_zoomSlider->setSingleStep(1);
    m_zoomSlider->setPageStep(4);
    m_zoomSlider->setToolTip("Stretch the time axis");
    m_zoomLabel = new QLabel("×1");
    m_zoomLabel->setObjectName("zoomLabel");
    m_zoomLabel->setMinimumWidth(40);
    m_zoomLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    auto* zoomRow = new QHBoxLayout;
    zoomRow->setContentsMargins(8, 4, 8, 0);
    zoomRow->addWidget(zoomCaption);
    zoomRow->addWidget(m_zoomSlider, 1);
    zoomRow->addWidget(m_zoomLabel);

    m_scroll = new QScrollBar(Qt::Horizontal);
    m_scroll->setObjectName("timeScroll");
    m_scroll->setToolTip("Scroll along time");
    m_scroll->setRange(0, 0);

    auto* scrollRow = new QHBoxLayout;
    scrollRow->setContentsMargins(kLeft, 0, kRight, 4);
    scrollRow->addWidget(m_scroll);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);
    layout->addLayout(zoomRow);
    layout->addWidget(m_view, 1);
    layout->addLayout(scrollRow);

    connect(m_zoomSlider, &QSlider::valueChanged, this, [this](int value) {
        const int oldVisible = visibleCount();
        const double center = m_origin + oldVisible * 0.5;
        applyZoom(value, center, 0.5);
    });
    connect(m_scroll, &QScrollBar::valueChanged, this, [this](int value) {
        if (value == m_origin)
            return;
        m_origin = value;
        clampOrigin();
        m_view->update();
    });
}

void WaveformPlot::setWaveform(const std::vector<std::int16_t>& samples, int echoSample)
{
    m_samples = samples;
    m_echoSample = echoSample;
    clampOrigin();
    syncScrollBar();
    m_view->update();
}

int WaveformPlot::visibleCount() const
{
    const int count = static_cast<int>(m_samples.size());
    if (count <= 1)
        return std::max(1, count);
    const int visible = std::max(1, static_cast<int>(std::lround(static_cast<double>(count) / m_zoom)));
    return std::min(count, visible);
}

void WaveformPlot::clampOrigin()
{
    const int count = static_cast<int>(m_samples.size());
    const int maximum = std::max(0, count - visibleCount());
    if (m_origin < 0)
        m_origin = 0;
    if (m_origin > maximum)
        m_origin = maximum;
}

void WaveformPlot::syncScrollBar()
{
    const int count = static_cast<int>(m_samples.size());
    const int visible = visibleCount();
    const int maximum = std::max(0, count - visible);
    m_scroll->blockSignals(true);
    m_scroll->setRange(0, maximum);
    m_scroll->setPageStep(std::max(1, visible));
    m_scroll->setSingleStep(std::max(1, visible / 20));
    m_scroll->setValue(m_origin);
    m_scroll->blockSignals(false);
    m_zoomSlider->setEnabled(!m_samples.empty());
    m_zoomLabel->setText(QString("×%1").arg(m_zoom));
}

void WaveformPlot::applyZoom(int zoom, double anchorSample, double anchorFraction)
{
    m_zoom = std::clamp(zoom, 1, kMaxZoom);
    anchorFraction = std::clamp(anchorFraction, 0.0, 1.0);
    const int visible = visibleCount();
    m_origin = static_cast<int>(std::llround(anchorSample - anchorFraction * visible));
    clampOrigin();

    m_zoomSlider->blockSignals(true);
    m_zoomSlider->setValue(m_zoom);
    m_zoomSlider->blockSignals(false);
    syncScrollBar();
    m_view->update();
}

void WaveformPlot::zoomAt(int steps, int cursorX)
{
    if (steps == 0 || m_samples.empty())
        return;
    const int plotWidth = m_view->width() - kLeft - kRight;
    double fraction = 0.5;
    if (plotWidth > 0)
        fraction = std::clamp((static_cast<double>(cursorX) - kLeft) / plotWidth, 0.0, 1.0);
    const double anchor = m_origin + fraction * visibleCount();
    applyZoom(m_zoom + steps, anchor, fraction);
}

void WaveformView::wheelEvent(QWheelEvent* event)
{
    const int steps = event->angleDelta().y() / 120;
    if (steps != 0)
        m_plot->zoomAt(steps, event->pos().x());
    event->accept();
}

void WaveformView::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    const QColor background("#121816");
    const QColor grid("#2a3b34");
    const QColor axis("#9fb0a6");
    const QColor trace("#7dff6b");
    const QColor marker("#ffb347");

    painter.fillRect(rect(), background);

    const QRect plot(kLeft, kTop, width() - kLeft - kRight, height() - kTop - kBottom);
    if (plot.width() < 8 || plot.height() < 8)
        return;

    painter.setPen(grid);
    const int midY = plot.center().y();
    painter.drawLine(plot.left(), midY, plot.right(), midY);
    for (int i = 1; i <= 2; ++i) {
        const int dy = plot.height() * i / 6;
        painter.drawLine(plot.left(), midY - dy, plot.right(), midY - dy);
        painter.drawLine(plot.left(), midY + dy, plot.right(), midY + dy);
    }

    auto yOf = [&](int sample) {
        const double norm = static_cast<double>(sample) / 32768.0;
        return midY - static_cast<int>(std::lround(norm * (plot.height() * 0.46)));
    };

    painter.setPen(axis);
    painter.setFont(QFont("Segoe UI", 8));
    painter.drawText(QRect(0, midY - 8, kLeft - 6, 16), Qt::AlignRight | Qt::AlignVCenter, "0");
    painter.drawText(QRect(0, kTop, kLeft - 6, 16), Qt::AlignRight | Qt::AlignVCenter, "+");
    painter.drawText(QRect(0, plot.bottom() - 16, kLeft - 6, 16), Qt::AlignRight | Qt::AlignVCenter, "−");

    const int count = static_cast<int>(m_plot->m_samples.size());
    if (count <= 0) {
        painter.setPen(QColor("#d5e0d8"));
        painter.setFont(QFont("Segoe UI", 11));
        painter.drawText(plot, Qt::AlignCenter, "Buffer was not formed");
        return;
    }

    const int origin = m_plot->m_origin;
    const int visible = std::max(1, m_plot->visibleCount());
    const double t0Us = static_cast<double>(origin) / SignalModel::kSampleRateHz * 1.0e6;
    const double t1Us = static_cast<double>(origin + visible - 1) / SignalModel::kSampleRateHz * 1.0e6;
    const double spanUs = t1Us - t0Us;

    painter.setPen(grid);
    if (spanUs > 0.0) {
        const double rawStep = spanUs / 8.0;
        const double power = std::pow(10.0, std::floor(std::log10(rawStep)));
        const double normalized = rawStep / power;
        const double multiplier = normalized < 1.5 ? 1.0 : normalized < 3.5 ? 2.0 : normalized < 7.5 ? 5.0 : 10.0;
        const double stepUs = multiplier * power;
        const int decimals = stepUs < 0.1 ? 3 : stepUs < 1.0 ? 2 : stepUs < 10.0 ? 1 : 0;
        const double firstTick = std::ceil(t0Us / stepUs) * stepUs;
        for (double t = firstTick; t <= t1Us + stepUs * 0.5; t += stepUs) {
            const int x = plot.left() + static_cast<int>(std::lround((t - t0Us) / spanUs * (plot.width() - 1)));
            if (x < plot.left() || x > plot.right())
                continue;
            painter.setPen(grid);
            painter.drawLine(x, plot.top(), x, plot.bottom());
            painter.setPen(axis);
            painter.drawText(QRect(x - 28, plot.bottom() + 4, 56, 18), Qt::AlignHCenter | Qt::AlignTop,
                             QString::number(t, 'f', decimals));
        }
    }
    painter.setPen(axis);
    painter.drawText(QRect(plot.left(), height() - 18, plot.width(), 16), Qt::AlignRight | Qt::AlignVCenter, "us");

    const int echo = m_plot->m_echoSample;
    if (echo >= origin && echo < origin + visible) {
        const int x = plot.left()
            + static_cast<int>(std::lround((static_cast<double>(echo - origin) + 0.5) / visible * plot.width()));
        painter.setPen(QPen(marker, 1, Qt::DashLine));
        painter.drawLine(x, plot.top(), x, plot.bottom());
        painter.setPen(marker);
        painter.drawText(QRect(x + 4, plot.top(), 48, 16), Qt::AlignLeft | Qt::AlignVCenter, "echo");
    }

    const int end = std::min(count, origin + visible);
    QPolygonF traceLine;
    traceLine.reserve(end - origin);
    for (int i = origin; i < end; ++i) {
        const double x = plot.left() + (static_cast<double>(i - origin) + 0.5) * plot.width() / static_cast<double>(visible);
        traceLine.append(QPointF(x, yOf(m_plot->m_samples[static_cast<size_t>(i)])));
    }
    painter.setPen(QPen(trace, 1));
    painter.setClipRect(plot.adjusted(0, -2, 0, 2));
    painter.drawPolyline(traceLine);
}
