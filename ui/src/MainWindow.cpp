//#include "avs/ui/MainWindow.hpp"
//
//#include "avs/core/algorithm/ExecutionStatus.hpp"
//#include "avs/core/algorithm/sorting/BubbleSortStepper.hpp"
//#include "avs/ui/widgets/ArrayVisualizationWidget.hpp"
//#include "avs/visualization/array/ArrayVisualizationState.hpp"
//
//#include <QHBoxLayout>
//#include <QLabel>
//#include <QPushButton>
//#include <QString>
//#include <QVBoxLayout>
//#include <QWidget>
//
//#include <memory>
//#include <string_view>
//#include <utility>
//#include <vector>
//
//namespace
//{
//    [[nodiscard]] QString toQString(std::string_view text)
//    {
//        return QString::fromUtf8(
//            text.data(),
//            static_cast<qsizetype>(text.size())
//        );
//    }
//
//    [[nodiscard]] QString toQString(::avs::core::algorithm::ExecutionStatus status)
//    {
//        using ::avs::core::algorithm::ExecutionStatus;
//
//        switch (status)
//        {
//        case ExecutionStatus::Ready:
//            return "Ready";
//
//        case ExecutionStatus::Running:
//            return "Running";
//
//        case ExecutionStatus::Paused:
//            return "Paused";
//
//        case ExecutionStatus::Finished:
//            return "Finished";
//
//        case ExecutionStatus::Cancelled:
//            return "Cancelled";
//        }
//
//        return "Unknown";
//    }
//}
//
//namespace avs::ui
//{
//    MainWindow::MainWindow(QWidget* parent)
//        : QMainWindow(parent)
//    {
//        setWindowTitle("Algorithm Visualizer Studio");
//        resize(1000, 700);
//
//        setupUi();
//        setupController();
//        connectSignals();
//
//        refreshView();
//    }
//
//    void MainWindow::setupUi()
//    {
//        auto* centralWidget = new QWidget(this);
//        auto* mainLayout = new QVBoxLayout(centralWidget);
//
//        statusLabel_ = new QLabel(centralWidget);
//
//        arrayWidget_ = new widgets::ArrayVisualizationWidget(centralWidget);
//
//        previousButton_ = new QPushButton("Previous", centralWidget);
//        nextButton_ = new QPushButton("Next", centralWidget);
//        resetButton_ = new QPushButton("Reset", centralWidget);
//
//        auto* controlsLayout = new QHBoxLayout();
//        controlsLayout->addWidget(previousButton_);
//        controlsLayout->addWidget(nextButton_);
//        controlsLayout->addWidget(resetButton_);
//        controlsLayout->addStretch();
//
//        mainLayout->addWidget(statusLabel_);
//        mainLayout->addWidget(arrayWidget_, 1);
//        mainLayout->addLayout(controlsLayout);
//
//        setCentralWidget(centralWidget);
//    }
//
//    void MainWindow::setupController()
//    {
//        auto stepper =
//            std::make_unique<::avs::core::algorithm::sorting::BubbleSortStepper>(
//                std::vector<int>{5, 1, 4, 2, 8, 3}
//            );
//
//        controller_ =
//            std::make_unique<::avs::core::execution::ExecutionController>(
//                std::move(stepper)
//            );
//
//        controller_->stepForward();
//    }
//
//    void MainWindow::connectSignals()
//    {
//        QObject::connect(
//            previousButton_,
//            &QPushButton::clicked,
//            this,
//            [this]()
//            {
//                controller_->stepBackward();
//                refreshView();
//            }
//        );
//
//        QObject::connect(
//            nextButton_,
//            &QPushButton::clicked,
//            this,
//            [this]()
//            {
//                controller_->stepForward();
//                refreshView();
//            }
//        );
//
//        QObject::connect(
//            resetButton_,
//            &QPushButton::clicked,
//            this,
//            [this]()
//            {
//                controller_->reset();
//                controller_->stepForward();
//                refreshView();
//            }
//        );
//    }
//
//    void MainWindow::refreshView()
//    {
//        updateVisualizationFromCurrentStep();
//
//        statusLabel_->setText(
//            QString("Algorithm: %1 | Status: %2")
//            .arg(toQString(controller_->algorithmName()))
//            .arg(toQString(controller_->status()))
//        );
//
//        previousButton_->setEnabled(controller_->canStepBackward());
//        nextButton_->setEnabled(controller_->canStepForward());
//        resetButton_->setEnabled(true);
//    }
//
//    void MainWindow::updateVisualizationFromCurrentStep()
//    {
//        const auto* currentStep = controller_->currentStep();
//
//        if (currentStep == nullptr)
//        {
//            arrayWidget_->setState(
//                visualization::array::ArrayVisualizationState{}
//            );
//            return;
//        }
//
//        auto visualizationState =
//            visualization::array::ArrayVisualizationState::fromStep(*currentStep);
//
//        if (!visualizationState.has_value())
//        {
//            arrayWidget_->setState(
//                visualization::array::ArrayVisualizationState{}
//            );
//            return;
//        }
//
//        arrayWidget_->setState(std::move(*visualizationState));
//    }
//}

#include "avs/ui/MainWindow.hpp"

#include "avs/core/algorithm/ExecutionStatus.hpp"
#include "avs/core/algorithm/sorting/BubbleSortStepper.hpp"
#include "avs/ui/widgets/ArrayVisualizationWidget.hpp"
#include "avs/visualization/array/ArrayVisualizationState.hpp"

#include <QFontMetrics>
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QString>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include <memory>
#include <string_view>
#include <utility>
#include <vector>

namespace
{
    constexpr int MinPlaybackIntervalMs = 100;
    constexpr int MaxPlaybackIntervalMs = 1500;
    constexpr int DefaultPlaybackIntervalMs = 500;

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
        , playbackIntervalMs_(DefaultPlaybackIntervalMs)
    {
        setWindowTitle("Algorithm Visualizer Studio");
        resize(1000, 700);

        setupUi();
        setupController();
        setupPlaybackTimer();
        connectSignals();

        refreshView();
    }

    void MainWindow::setupUi()
    {
        auto* centralWidget = new QWidget(this);
        auto* mainLayout = new QVBoxLayout(centralWidget);

        statusLabel_ = new QLabel(centralWidget);

        stepTitleLabel_ = new QLabel(centralWidget);
        stepDescriptionLabel_ = new QLabel(centralWidget);

        stepTitleLabel_->setWordWrap(true);
        stepDescriptionLabel_->setWordWrap(true);

        QFont titleFont = stepTitleLabel_->font();
        titleFont.setBold(true);
        stepTitleLabel_->setFont(titleFont);

        arrayWidget_ = new widgets::ArrayVisualizationWidget(centralWidget);

        previousButton_ = new QPushButton("Previous", centralWidget);
        nextButton_ = new QPushButton("Next", centralWidget);
        startButton_ = new QPushButton("Start", centralWidget);
        pauseButton_ = new QPushButton("Pause", centralWidget);
        resetButton_ = new QPushButton("Reset", centralWidget);

        playbackSpeedLabel_ = new QLabel(centralWidget);

        const int playbackSpeedLabelWidth =
            QFontMetrics(playbackSpeedLabel_->font())
            .horizontalAdvance("Playback interval: 1500 ms") + 12;

        playbackSpeedLabel_->setFixedWidth(playbackSpeedLabelWidth);
        playbackSpeedLabel_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        playbackSpeedSlider_ = new QSlider(Qt::Horizontal, centralWidget);
        playbackSpeedSlider_->setRange(MinPlaybackIntervalMs, MaxPlaybackIntervalMs);
        playbackSpeedSlider_->setSingleStep(50);
        playbackSpeedSlider_->setPageStep(100);
        playbackSpeedSlider_->setTickInterval(200);
        playbackSpeedSlider_->setTickPosition(QSlider::TicksBelow);
        playbackSpeedSlider_->setValue(DefaultPlaybackIntervalMs);
        playbackSpeedSlider_->setMinimumWidth(260);

        auto* controlsLayout = new QHBoxLayout();
        controlsLayout->addWidget(previousButton_);
        controlsLayout->addWidget(nextButton_);
        controlsLayout->addWidget(startButton_);
        controlsLayout->addWidget(pauseButton_);
        controlsLayout->addWidget(resetButton_);
        controlsLayout->addStretch();

        auto* speedLayout = new QHBoxLayout();
        speedLayout->addWidget(playbackSpeedLabel_);
        speedLayout->addWidget(playbackSpeedSlider_);
        speedLayout->addStretch();

        mainLayout->addWidget(statusLabel_);
        mainLayout->addWidget(stepTitleLabel_);
        mainLayout->addWidget(stepDescriptionLabel_);
        mainLayout->addWidget(arrayWidget_, 1);
        mainLayout->addLayout(controlsLayout);
        mainLayout->addLayout(speedLayout);

        setCentralWidget(centralWidget);

        updatePlaybackSpeedLabel();
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

    void MainWindow::setupPlaybackTimer()
    {
        playbackTimer_ = new QTimer(this);
        playbackTimer_->setInterval(playbackIntervalMs_);
    }

    void MainWindow::connectSignals()
    {
        QObject::connect(
            previousButton_,
            &QPushButton::clicked,
            this,
            [this]()
            {
                pausePlayback();
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
                pausePlayback();
                controller_->stepForward();
                refreshView();
            }
        );

        QObject::connect(
            startButton_,
            &QPushButton::clicked,
            this,
            [this]()
            {
                startPlayback();
            }
        );

        QObject::connect(
            pauseButton_,
            &QPushButton::clicked,
            this,
            [this]()
            {
                pausePlayback();
                refreshView();
            }
        );

        QObject::connect(
            resetButton_,
            &QPushButton::clicked,
            this,
            [this]()
            {
                pausePlayback();
                controller_->reset();
                controller_->stepForward();
                refreshView();
            }
        );

        QObject::connect(
            playbackSpeedSlider_,
            &QSlider::valueChanged,
            this,
            [this](int intervalMs)
            {
                handlePlaybackSpeedChanged(intervalMs);
            }
        );

        QObject::connect(
            playbackTimer_,
            &QTimer::timeout,
            this,
            [this]()
            {
                handlePlaybackTick();
            }
        );
    }

    void MainWindow::startPlayback()
    {
        controller_->start();

        if (controller_->isRunning())
        {
            playbackTimer_->start();
        }

        refreshView();
    }

    void MainWindow::pausePlayback()
    {
        playbackTimer_->stop();
        controller_->pause();
    }

    void MainWindow::handlePlaybackTick()
    {
        controller_->tick();

        if (!controller_->isRunning())
        {
            playbackTimer_->stop();
        }

        refreshView();
    }

    void MainWindow::handlePlaybackSpeedChanged(int intervalMs)
    {
        playbackIntervalMs_ = intervalMs;

        if (playbackTimer_ != nullptr)
        {
            playbackTimer_->setInterval(playbackIntervalMs_);
        }

        updatePlaybackSpeedLabel();
    }

    void MainWindow::refreshView()
    {
        updateVisualizationFromCurrentStep();
        updateStepDetails();

        statusLabel_->setText(
            QString("Algorithm: %1 | Status: %2")
            .arg(toQString(controller_->algorithmName()))
            .arg(toQString(controller_->status()))
        );

        const bool running = controller_->isRunning();

        previousButton_->setEnabled(!running && controller_->canStepBackward());
        nextButton_->setEnabled(!running && controller_->canStepForward());
        startButton_->setEnabled(!running && controller_->canStepForward());
        pauseButton_->setEnabled(running);
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

    void MainWindow::updateStepDetails()
    {
        const auto* currentStep = controller_->currentStep();

        if (currentStep == nullptr)
        {
            stepTitleLabel_->setText("Step: -");
            stepDescriptionLabel_->setText("No step selected.");
            return;
        }

        stepTitleLabel_->setText(
            QString("Step %1: %2")
            .arg(currentStep->index())
            .arg(toQString(currentStep->title()))
        );

        stepDescriptionLabel_->setText(
            toQString(currentStep->description())
        );
    }

    void MainWindow::updatePlaybackSpeedLabel()
    {
        playbackSpeedLabel_->setText(
            QString("Playback interval: %1 ms ").arg(playbackIntervalMs_)
        );
    }
}
