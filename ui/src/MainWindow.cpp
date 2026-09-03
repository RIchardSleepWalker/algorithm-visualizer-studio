#include "avs/ui/MainWindow.hpp"

#include "avs/core/algorithm/ExecutionStatus.hpp"
#include "avs/core/algorithm/sorting/BubbleSortStepper.hpp"
#include "avs/ui/widgets/ArrayVisualizationWidget.hpp"
#include "avs/visualization/array/ArrayVisualizationState.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

#include <memory>
#include <string_view>
#include <utility>
#include <vector>

namespace
{
    [[nodiscard]] QString toQString(std::string_view text)
    {
        return QString::fromUtf8(
            text.data(),
            static_cast<qsizetype>(text.size())
        );
    }

    [[nodiscard]] QString toQString(::avs::core::algorithm::ExecutionStatus status)
    {
        using ::avs::core::algorithm::ExecutionStatus;

        switch (status)
        {
        case ExecutionStatus::Ready:
            return "Ready";

        case ExecutionStatus::Running:
            return "Running";

        case ExecutionStatus::Paused:
            return "Paused";

        case ExecutionStatus::Finished:
            return "Finished";

        case ExecutionStatus::Cancelled:
            return "Cancelled";
        }

        return "Unknown";
    }
}

namespace avs::ui
{
    MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
    {
        setWindowTitle("Algorithm Visualizer Studio");
        resize(1000, 700);

        setupUi();
        setupController();
        connectSignals();

        refreshView();
    }

    void MainWindow::setupUi()
    {
        auto* centralWidget = new QWidget(this);
        auto* mainLayout = new QVBoxLayout(centralWidget);

        statusLabel_ = new QLabel(centralWidget);

        arrayWidget_ = new widgets::ArrayVisualizationWidget(centralWidget);

        previousButton_ = new QPushButton("Previous", centralWidget);
        nextButton_ = new QPushButton("Next", centralWidget);
        resetButton_ = new QPushButton("Reset", centralWidget);

        auto* controlsLayout = new QHBoxLayout();
        controlsLayout->addWidget(previousButton_);
        controlsLayout->addWidget(nextButton_);
        controlsLayout->addWidget(resetButton_);
        controlsLayout->addStretch();

        mainLayout->addWidget(statusLabel_);
        mainLayout->addWidget(arrayWidget_, 1);
        mainLayout->addLayout(controlsLayout);

        setCentralWidget(centralWidget);
    }

    void MainWindow::setupController()
    {
        auto stepper =
            std::make_unique<::avs::core::algorithm::sorting::BubbleSortStepper>(
                std::vector<int>{5, 1, 4, 2, 8, 3}
            );

        controller_ =
            std::make_unique<::avs::core::execution::ExecutionController>(
                std::move(stepper)
            );

        controller_->stepForward();
    }

    void MainWindow::connectSignals()
    {
        QObject::connect(
            previousButton_,
            &QPushButton::clicked,
            this,
            [this]()
            {
                controller_->stepBackward();
                refreshView();
            }
        );

        QObject::connect(
            nextButton_,
            &QPushButton::clicked,
            this,
            [this]()
            {
                controller_->stepForward();
                refreshView();
            }
        );

        QObject::connect(
            resetButton_,
            &QPushButton::clicked,
            this,
            [this]()
            {
                controller_->reset();
                controller_->stepForward();
                refreshView();
            }
        );
    }

    void MainWindow::refreshView()
    {
        updateVisualizationFromCurrentStep();

        statusLabel_->setText(
            QString("Algorithm: %1 | Status: %2")
            .arg(toQString(controller_->algorithmName()))
            .arg(toQString(controller_->status()))
        );

        previousButton_->setEnabled(controller_->canStepBackward());
        nextButton_->setEnabled(controller_->canStepForward());
        resetButton_->setEnabled(true);
    }

    void MainWindow::updateVisualizationFromCurrentStep()
    {
        const auto* currentStep = controller_->currentStep();

        if (currentStep == nullptr)
        {
            arrayWidget_->setState(
                visualization::array::ArrayVisualizationState{}
            );
            return;
        }

        auto visualizationState =
            visualization::array::ArrayVisualizationState::fromStep(*currentStep);

        if (!visualizationState.has_value())
        {
            arrayWidget_->setState(
                visualization::array::ArrayVisualizationState{}
            );
            return;
        }

        arrayWidget_->setState(std::move(*visualizationState));
    }
}
