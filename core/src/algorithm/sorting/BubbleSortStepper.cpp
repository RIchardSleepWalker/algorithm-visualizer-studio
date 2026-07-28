#include "avs/core/algorithm/sorting/BubbleSortStepper.hpp"

#include <string>
#include <utility>

namespace avs::core::algorithm::sorting
{
    BubbleSortStepper::BubbleSortStepper(std::vector<int> values)
        : initialValues_(std::move(values)),
        values_(initialValues_)
    {}

    std::string_view BubbleSortStepper::name() const noexcept
    {
        return "Bubble Sort";
    }

    AlgorithmCategory BubbleSortStepper::category() const noexcept
    {
        return AlgorithmCategory::Sorting;
    }

    bool BubbleSortStepper::hasNextStep() const
    {
        return !finished_;
    }

    std::optional<AlgorithmStep> BubbleSortStepper::nextStep()
    {
        if (finished_)
        {
            return std::nullopt;
        }

        if (!initialized_)
        {
            initialized_ = true;
            return makeInitializationStep();
        }

        if (pendingSwap_)
        {
            return makeSwapStep();
        }

        if (isSortingCompleted())
        {
            finished_ = true;
            return makeFinishStep();
        }

        return makeComparisonStep();
    }

    void BubbleSortStepper::reset()
    {
        values_ = initialValues_;

        stepIndex_ = 0;
        passIndex_ = 0;
        compareIndex_ = 0;

        initialized_ = false;
        finished_ = false;
        pendingSwap_ = false;
    }

    const std::vector<int>& BubbleSortStepper::values() const noexcept
    {
        return values_;
    }

    bool BubbleSortStepper::isSortingCompleted() const noexcept
    {
        if (values_.size() < 2)
        {
            return true;
        }

        return passIndex_ >= values_.size() - 1;
    }

    AlgorithmStep BubbleSortStepper::makeInitializationStep()
    {
        return AlgorithmStep(
            stepIndex_++,
            StepType::Initialization,
            AlgorithmCategory::Sorting,
            "Initialize Bubble Sort",
            "Initialize Bubble Sort with " + std::to_string(values_.size()) + " elements."
        );
    }

    AlgorithmStep BubbleSortStepper::makeComparisonStep()
    {
        const auto leftIndex = compareIndex_;
        const auto rightIndex = compareIndex_ + 1;

        const auto leftValue = values_[leftIndex];
        const auto rightValue = values_[rightIndex];

        const bool shouldSwap = leftValue > rightValue;

        if (shouldSwap)
        {
            pendingSwap_ = true;
        }
        else
        {
            advanceComparison();
        }

        return AlgorithmStep(
            stepIndex_++,
            StepType::Comparison,
            AlgorithmCategory::Sorting,
            "Compare elements",
            "Compare values " + std::to_string(leftValue)
            + " and " + std::to_string(rightValue)
            + " at indices " + std::to_string(leftIndex)
            + " and " + std::to_string(rightIndex) + "."
        );
    }

    AlgorithmStep BubbleSortStepper::makeSwapStep()
    {
        const auto leftIndex = compareIndex_;
        const auto rightIndex = compareIndex_ + 1;

        const auto leftValue = values_[leftIndex];
        const auto rightValue = values_[rightIndex];

        std::swap(values_[leftIndex], values_[rightIndex]);

        pendingSwap_ = false;
        advanceComparison();

        return AlgorithmStep(
            stepIndex_++,
            StepType::Swap,
            AlgorithmCategory::Sorting,
            "Swap elements",
            "Swap values " + std::to_string(leftValue)
            + " and " + std::to_string(rightValue)
            + " at indices " + std::to_string(leftIndex)
            + " and " + std::to_string(rightIndex) + "."
        );
    }

    AlgorithmStep BubbleSortStepper::makeFinishStep()
    {
        return AlgorithmStep(
            stepIndex_++,
            StepType::Finish,
            AlgorithmCategory::Sorting,
            "Finish Bubble Sort",
            "Bubble Sort finished. The collection is sorted."
        );
    }

    void BubbleSortStepper::advanceComparison() noexcept
    {
        if (values_.size() < 2)
        {
            return;
        }

        ++compareIndex_;

        const auto lastComparableIndex = values_.size() - passIndex_ - 1;

        if (compareIndex_ >= lastComparableIndex)
        {
            compareIndex_ = 0;
            ++passIndex_;
        }
    }
}
