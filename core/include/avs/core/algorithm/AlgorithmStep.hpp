#pragma once

#include "avs/core/algorithm/AlgorithmCategory.hpp"
#include "avs/core/algorithm/StepType.hpp"

#include <cstddef>
#include <string>
#include <string_view>

namespace avs::core::algorithm
{
    class AlgorithmStep
    {
    public:
        AlgorithmStep(
            std::size_t index,
            StepType type,
            AlgorithmCategory category,
            std::string title,
            std::string description
        );

        [[nodiscard]] std::size_t index() const noexcept;
        [[nodiscard]] StepType type() const noexcept;
        [[nodiscard]] AlgorithmCategory category() const noexcept;
        [[nodiscard]] std::string_view title() const noexcept;
        [[nodiscard]] std::string_view description() const noexcept;

    private:
        std::size_t index_;
        StepType type_;
        AlgorithmCategory category_;
        std::string title_;
        std::string description_;
    };
}
