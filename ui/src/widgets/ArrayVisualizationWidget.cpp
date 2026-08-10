#include "avs/ui/widgets/ArrayVisualizationWidget.hpp"

#include <QPainter>
#include <QPaintEvent>

#include <algorithm>
#include <utility>

namespace avs::ui::widgets
{
    ArrayVisualizationWidget::ArrayVisualizationWidget(QWidget* parent)
        : QWidget(parent)
    {
        setMinimumSize(600, 300);
    }

    void ArrayVisualizationWidget::setState(
        visualization::array::ArrayVisualizationState state
    )
    {
        state_ = std::move(state);
        update();
    }

    const visualization::array::ArrayVisualizationState&
        ArrayVisualizationWidget::state() const noexcept
    {
        return state_;
    }

    void ArrayVisualizationWidget::paintEvent(QPaintEvent* event)
    {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.fillRect(rect(), palette().window());

        if (state_.empty())
        {
            drawEmptyState(painter);
            return;
        }

        drawBars(painter);
    }

    void ArrayVisualizationWidget::drawEmptyState(QPainter& painter) const
    {
        painter.setPen(palette().text().color());
        painter.drawText(rect(), Qt::AlignCenter, "No array data to visualize");
    }

    void ArrayVisualizationWidget::drawBars(QPainter& painter) const
    {
        const auto& elements = state_.elements();

        const auto maxElement = std::max_element(
            elements.begin(),
            elements.end(),
            [](const auto& lhs, const auto& rhs)
            {
                return lhs.value < rhs.value;
            }
        );

        const int maxValue = std::max(1, maxElement->value);

        const QRect drawingArea = rect().adjusted(24, 24, -24, -48);

        if (drawingArea.width() <= 0 || drawingArea.height() <= 0)
        {
            return;
        }

        const qreal slotWidth =
            static_cast<qreal>(drawingArea.width()) / static_cast<qreal>(elements.size());

        const qreal barWidth = std::max<qreal>(4.0, slotWidth * 0.7);

        for (std::size_t i = 0; i < elements.size(); ++i)
        {
            const auto& element = elements[i];

            const int safeValue = std::max(0, element.value);

            const qreal normalizedHeight =
                static_cast<qreal>(safeValue) / static_cast<qreal>(maxValue);

            const qreal barHeight =
                normalizedHeight * static_cast<qreal>(drawingArea.height());

            const qreal x =
                static_cast<qreal>(drawingArea.left()) +
                static_cast<qreal>(i) * slotWidth +
                (slotWidth - barWidth) / 2.0;

            const qreal y =
                static_cast<qreal>(drawingArea.bottom()) - barHeight;

            const QRectF barRect(x, y, barWidth, barHeight);

            painter.setPen(Qt::NoPen);
            painter.setBrush(element.highlighted ? palette().highlight() : palette().mid());
            painter.drawRoundedRect(barRect, 4.0, 4.0);

            painter.setPen(palette().text().color());

            const QRectF labelRect(
                x,
                static_cast<qreal>(drawingArea.bottom()) + 8.0,
                barWidth,
                24.0
            );

            painter.drawText(
                labelRect,
                Qt::AlignCenter,
                QString::number(element.value)
            );
        }
    }
}
