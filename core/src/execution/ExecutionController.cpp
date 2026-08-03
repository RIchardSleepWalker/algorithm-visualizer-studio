#include "avs/core/execution/ExecutionController.hpp"

#include <utility>

namespace avs::core::execution
{
    ExecutionController::ExecutionController(
        std::unique_ptr<algorithm::IAlgorithmStepper> stepper
    )
        : runner_(std::move(stepper))
    {}

    std::string_view ExecutionController::algorithmName() const noexcept
    {
        return runner_.algorithmName();
    }

    algorithm::AlgorithmCategory ExecutionController::algorithmCategory() const noexcept
    {
        return runner_.algorithmCategory();
    }

    algorithm::ExecutionStatus ExecutionController::status() const noexcept
    {
        return status_;
    }

    const algorithm::Timeline& ExecutionController::timeline() const noexcept
    {
        return runner_.timeline();
    }

    const algorithm::AlgorithmStep* ExecutionController::currentStep() const noexcept
    {
        return runner_.currentStep();
    }

    bool ExecutionController::isRunning() const noexcept
    {
        return status_ == algorithm::ExecutionStatus::Running;
    }

    bool ExecutionController::canStepForward() const
    {
        if (status_ == algorithm::ExecutionStatus::Cancelled)
        {
            return false;
        }

        return runner_.canStepForward();
    }

    bool ExecutionController::canStepBackward() const noexcept
    {
        if (status_ == algorithm::ExecutionStatus::Cancelled)
        {
            return false;
        }

        return runner_.canStepBackward();
    }

    void ExecutionController::start()
    {
        if (status_ == algorithm::ExecutionStatus::Cancelled)
        {
            return;
        }

        if (!runner_.canStepForward())
        {
            status_ = algorithm::ExecutionStatus::Finished;
            return;
        }

        status_ = algorithm::ExecutionStatus::Running;
    }

    void ExecutionController::pause()
    {
        if (status_ == algorithm::ExecutionStatus::Running)
        {
            status_ = algorithm::ExecutionStatus::Paused;
        }
    }

    void ExecutionController::stop()
    {
        status_ = algorithm::ExecutionStatus::Cancelled;
    }

    void ExecutionController::reset()
    {
        runner_.reset();
        status_ = algorithm::ExecutionStatus::Ready;
    }

    const algorithm::AlgorithmStep* ExecutionController::stepForward()
    {
        if (status_ == algorithm::ExecutionStatus::Cancelled)
        {
            return currentStep();
        }

        const algorithm::AlgorithmStep* step = runner_.stepForward();

        updateStatusAfterManualStep();

        return step;
    }

    const algorithm::AlgorithmStep* ExecutionController::stepBackward()
    {
        if (status_ == algorithm::ExecutionStatus::Cancelled)
        {
            return currentStep();
        }

        const algorithm::AlgorithmStep* step = runner_.stepBackward();

        if (step != nullptr)
        {
            status_ = algorithm::ExecutionStatus::Paused;
        }

        return step;
    }

    const algorithm::AlgorithmStep* ExecutionController::tick()
    {
        if (status_ != algorithm::ExecutionStatus::Running)
        {
            return currentStep();
        }

        if (!runner_.canStepForward())
        {
            status_ = algorithm::ExecutionStatus::Finished;
            return currentStep();
        }

        const algorithm::AlgorithmStep* step = runner_.stepForward();

        if (!runner_.canStepForward())
        {
            status_ = algorithm::ExecutionStatus::Finished;
            return step;
        }

        status_ = algorithm::ExecutionStatus::Running;

        return step;
    }

    void ExecutionController::updateStatusAfterManualStep()
    {
        if (!runner_.canStepForward())
        {
            status_ = algorithm::ExecutionStatus::Finished;
            return;
        }

        status_ = algorithm::ExecutionStatus::Paused;
    }
}
