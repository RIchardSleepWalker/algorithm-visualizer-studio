#pragma once

#include "avs/visualization/array/ArrayVisualizationState.hpp"

#include <QWidget>

namespace avs::ui::widgets
{
    class ArrayVisualizationWidget final : public QWidget
    {
    public:
        explicit ArrayVisualizationWidget(QWidget* parent = nullptr);

        void setState(visualization::array::ArrayVisualizationState state);

        [[nodiscard]] const visualization::array::ArrayVisualizationState& state() const noexcept;

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        void drawEmptyState(QPainter& painter) const;
        void drawBars(QPainter& painter) const;

    private:
        visualization::array::ArrayVisualizationState state_;
    };
}
