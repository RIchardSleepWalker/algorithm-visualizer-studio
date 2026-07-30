#include "avs/core/algorithm/AlgorithmRunner.hpp"

#include <stdexcept>
#include <utility>

namespace avs::core::algorithm
{
    AlgorithmRunner::AlgorithmRunner(std::unique_ptr<IAlgorithmStepper> stepper)
        : stepper_(std::move(stepper))
    {
        if (stepper_ == nullptr)
        {
            throw std::invalid_argument("AlgorithmRunner requires a valid stepper.");
        }
    }

    std::string_view AlgorithmRunner::algorithmName() const noexcept
    {
        return stepper_->name();
    }

    AlgorithmCategory AlgorithmRunner::algorithmCategory() const noexcept
    {
        return stepper_->category();
    }

    ExecutionStatus AlgorithmRunner::status() const noexcept
    {
        return status_;
    }

    const Timeline& AlgorithmRunner::timeline() const noexcept
    {
        return timeline_;
    }

    const AlgorithmStep* AlgorithmRunner::currentStep() const noexcept
    {
        return timeline_.currentStep();
    }

    bool AlgorithmRunner::canStepForward() const
    {
        return timeline_.canMoveForward() || stepper_->hasNextStep();
    }

    bool AlgorithmRunner::canStepBackward() const noexcept
    {
        return timeline_.canMoveBackward();
    }

    const AlgorithmStep* AlgorithmRunner::stepForward()
    {
        if (!canStepForward())
        {
            status_ = ExecutionStatus::Finished;
            return currentStep();
        }

        status_ = ExecutionStatus::SteppingForward;

        if (timeline_.canMoveForward())
        {
            const AlgorithmStep* step = timeline_.moveForward();
            updateStatusAfterStep();
            return step;
        }

        auto nextStep = stepper_->nextStep();

        if (!nextStep.has_value())
        {
            status_ = ExecutionStatus::Finished;
            return currentStep();
        }

        timeline_.addStep(std::move(*nextStep));

        const AlgorithmStep* step = timeline_.moveForward();
        updateStatusAfterStep();

        return step;
    }

    const AlgorithmStep* AlgorithmRunner::stepBackward()
    {
        if (!canStepBackward())
        {
            return currentStep();
        }

        status_ = ExecutionStatus::SteppingBackward;

        const AlgorithmStep* step = timeline_.moveBackward();

        status_ = ExecutionStatus::Paused;

        return step;
    }

    void AlgorithmRunner::reset()
    {
        stepper_->reset();
        timeline_.clear();
        status_ = ExecutionStatus::Ready;
    }

    void AlgorithmRunner::updateStatusAfterStep()
    {
        const AlgorithmStep* step = currentStep();

        if (step == nullptr)
        {
            status_ = ExecutionStatus::Ready;
            return;
        }

        if (step->type() == StepType::Finish || !canStepForward())
        {
            status_ = ExecutionStatus::Finished;
            return;
        }

        status_ = ExecutionStatus::Paused;
    }
}
