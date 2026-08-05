#include "avs/visualization/array/ArrayVisualizationState.hpp"

#include <algorithm>
#include <variant>
#include <utility>

namespace avs::visualization::array
{
    namespace
    {
        [[nodiscard]] bool containsIndex(
            const std::vector<std::size_t>& indices,
            std::size_t index
        )
        {
            return std::find(indices.begin(), indices.end(), index) != indices.end();
        }
    }

    ArrayVisualizationState::ArrayVisualizationState(
        std::vector<ArrayVisualElement> elements
    )
        : elements_(std::move(elements))
    {}

    ArrayVisualizationState ArrayVisualizationState::fromPayload(
        const core::algorithm::ArrayStepPayload& payload
    )
    {
        std::vector<ArrayVisualElement> elements;
        elements.reserve(payload.values.size());

        for (std::size_t i = 0; i < payload.values.size(); ++i)
        {
            elements.push_back(
                ArrayVisualElement{
                    payload.values[i],
                    containsIndex(payload.highlightedIndices, i)
                }
            );
        }

        return ArrayVisualizationState(std::move(elements));
    }

    std::optional<ArrayVisualizationState> ArrayVisualizationState::fromStep(
        const core::algorithm::AlgorithmStep& step
    )
    {
        const auto* arrayPayload = std::get_if<core::algorithm::ArrayStepPayload>(
            &step.payload()
        );

        if (arrayPayload == nullptr)
        {
            return std::nullopt;
        }

        return fromPayload(*arrayPayload);
    }

    bool ArrayVisualizationState::empty() const noexcept
    {
        return elements_.empty();
    }

    std::size_t ArrayVisualizationState::size() const noexcept
    {
        return elements_.size();
    }

    const std::vector<ArrayVisualElement>& ArrayVisualizationState::elements() const noexcept
    {
        return elements_;
    }
}
