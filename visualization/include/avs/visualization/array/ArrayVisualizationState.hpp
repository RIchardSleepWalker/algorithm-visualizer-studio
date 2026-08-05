#pragma once

#include "avs/core/algorithm/AlgorithmStep.hpp"
#include "avs/core/algorithm/StepPayload.hpp"

#include <cstddef>
#include <optional>
#include <vector>

namespace avs::visualization::array
{
    struct ArrayVisualElement
    {
        int value = 0;
        bool highlighted = false;
    };

    class ArrayVisualizationState
    {
    public:
        ArrayVisualizationState() = default;

        [[nodiscard]] static ArrayVisualizationState fromPayload(
            const core::algorithm::ArrayStepPayload& payload
        );

        [[nodiscard]] static std::optional<ArrayVisualizationState> fromStep(
            const core::algorithm::AlgorithmStep& step
        );

        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] std::size_t size() const noexcept;

        [[nodiscard]] const std::vector<ArrayVisualElement>& elements() const noexcept;

    private:
        explicit ArrayVisualizationState(std::vector<ArrayVisualElement> elements);

    private:
        std::vector<ArrayVisualElement> elements_;
    };
}
