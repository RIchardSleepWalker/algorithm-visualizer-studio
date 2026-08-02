#pragma once

#include <cstddef>
#include <variant>
#include <vector>

namespace avs::core::algorithm
{
    struct ArrayStepPayload
    {
        std::vector<int> values;
        std::vector<std::size_t> highlightedIndices;
    };

    using StepPayload = std::variant<std::monostate, ArrayStepPayload>;

    [[nodiscard]] inline bool hasPayload(const StepPayload& payload) noexcept
    {
        return !std::holds_alternative<std::monostate>(payload);
    }
}
