#pragma once

#include "avs/core/algorithm/AlgorithmStep.hpp"

#include <cstddef>
#include <optional>
#include <vector>

namespace avs::core::algorithm
{
    class Timeline
    {
    public:
        Timeline() = default;

        void addStep(AlgorithmStep step);
        void clear() noexcept;
        void reset() noexcept;

        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] std::size_t size() const noexcept;

        [[nodiscard]] bool hasCurrentStep() const noexcept;
        [[nodiscard]] const AlgorithmStep* currentStep() const noexcept;

        [[nodiscard]] bool canMoveForward() const noexcept;
        [[nodiscard]] bool canMoveBackward() const noexcept;

        const AlgorithmStep* moveForward() noexcept;
        const AlgorithmStep* moveBackward() noexcept;

        const AlgorithmStep* jumpToBeginning() noexcept;
        const AlgorithmStep* jumpToEnd() noexcept;

    private:
        std::vector<AlgorithmStep> steps_;
        std::optional<std::size_t> currentIndex_;
    };
}
