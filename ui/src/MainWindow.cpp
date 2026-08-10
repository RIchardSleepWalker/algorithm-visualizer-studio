#include "avs/ui/MainWindow.hpp"

#include "avs/core/algorithm/StepPayload.hpp"
#include "avs/ui/widgets/ArrayVisualizationWidget.hpp"
#include "avs/visualization/array/ArrayVisualizationState.hpp"

namespace avs::ui
{
    MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
    {
        setWindowTitle("Algorithm Visualizer Studio");
        resize(1000, 700);

        arrayWidget_ = new widgets::ArrayVisualizationWidget(this);
        setCentralWidget(arrayWidget_);

        const core::algorithm::ArrayStepPayload payload{
            {5, 1, 4, 2, 8, 3},
            {1, 2}
        };

        arrayWidget_->setState(
            visualization::array::ArrayVisualizationState::fromPayload(payload)
        );
    }
}
