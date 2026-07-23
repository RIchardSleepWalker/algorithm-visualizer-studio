#include "avs/core/algorithm/AlgorithmStep.hpp"

#include <stdexcept>
#include <utility>

namespace avs::core::algorithm
{
    AlgorithmStep::AlgorithmStep(
        std::size_t index,
        StepType type,
        AlgorithmCategory category,
        std::string title,
        std::string description
    )
        : index_(index),
        type_(type),
        category_(category),
        title_(std::move(title)),
        description_(std::move(description))
    {
        if (title_.empty())
        {
            throw std::invalid_argument("AlgorithmStep title cannot be empty.");
        }
    }

    std::size_t AlgorithmStep::index() const noexcept
    {
        return index_;
    }

    StepType AlgorithmStep::type() const noexcept
    {
        return type_;
    }

    AlgorithmCategory AlgorithmStep::category() const noexcept
    {
        return category_;
    }

    std::string_view AlgorithmStep::title() const noexcept
    {
        return title_;
    }

    std::string_view AlgorithmStep::description() const noexcept
    {
        return description_;
    }
}
