#include "avs/core/algorithm/Timeline.hpp"

#include <utility>

namespace avs::core::algorithm
{
    void Timeline::addStep(AlgorithmStep step)
    {
        steps_.push_back(std::move(step));
    }

    void Timeline::clear() noexcept
    {
        steps_.clear();
        currentIndex_.reset();
    }

    void Timeline::reset() noexcept
    {
        currentIndex_.reset();
    }

    bool Timeline::empty() const noexcept
    {
        return steps_.empty();
    }

    std::size_t Timeline::size() const noexcept
    {
        return steps_.size();
    }

    bool Timeline::hasCurrentStep() const noexcept
    {
        return currentIndex_.has_value();
    }

    const AlgorithmStep* Timeline::currentStep() const noexcept
    {
        if (!currentIndex_.has_value())
        {
            return nullptr;
        }

        return &steps_[*currentIndex_];
    }

    bool Timeline::canMoveForward() const noexcept
    {
        if (steps_.empty())
        {
            return false;
        }

        if (!currentIndex_.has_value())
        {
            return true;
        }

        return *currentIndex_ + 1 < steps_.size();
    }

    bool Timeline::canMoveBackward() const noexcept
    {
        return currentIndex_.has_value() && *currentIndex_ > 0;
    }

    const AlgorithmStep* Timeline::moveForward() noexcept
    {
        if (!canMoveForward())
        {
            return currentStep();
        }

        if (!currentIndex_.has_value())
        {
            currentIndex_ = 0;
        }
        else
        {
            ++(*currentIndex_);
        }

        return currentStep();
    }

    const AlgorithmStep* Timeline::moveBackward() noexcept
    {
        if (!canMoveBackward())
        {
            return currentStep();
        }

        --(*currentIndex_);
        return currentStep();
    }

    const AlgorithmStep* Timeline::jumpToBeginning() noexcept
    {
        if (steps_.empty())
        {
            currentIndex_.reset();
            return nullptr;
        }

        currentIndex_ = 0;
        return currentStep();
    }

    const AlgorithmStep* Timeline::jumpToEnd() noexcept
    {
        if (steps_.empty())
        {
            currentIndex_.reset();
            return nullptr;
        }

        currentIndex_ = steps_.size() - 1;
        return currentStep();
    }
}
