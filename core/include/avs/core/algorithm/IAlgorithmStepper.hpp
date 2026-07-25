#pragma once

#include "avs/core/algorithm/AlgorithmCategory.hpp"
#include "avs/core/algorithm/AlgorithmStep.hpp"

#include <optional>
#include <string_view>

namespace avs::core::algorithm
{
    class IAlgorithmStepper
    {
    public:
        virtual ~IAlgorithmStepper() = default;

        [[nodiscard]] virtual std::string_view name() const noexcept = 0;
        [[nodiscard]] virtual AlgorithmCategory category() const noexcept = 0;

        [[nodiscard]] virtual bool hasNextStep() const = 0;
        [[nodiscard]] virtual std::optional<AlgorithmStep> nextStep() = 0;

        virtual void reset() = 0;
    };
}
