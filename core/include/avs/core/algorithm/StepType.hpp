#pragma once

#include <string_view>

namespace avs::core::algorithm
{
    enum class StepType
    {
        Unknown,
        Initialization,
        Comparison,
        Swap,
        VisitVertex,
        RelaxEdge,
        UpdateValue,
        PushToQueue,
        PopFromQueue,
        Backtrack,
        Prune,
        Finish
    };

    [[nodiscard]] constexpr std::string_view to_string(StepType type) noexcept
    {
        switch (type)
        {
        case StepType::Unknown:
            return "Unknown";
        case StepType::Initialization:
            return "Initialization";
        case StepType::Comparison:
            return "Comparison";
        case StepType::Swap:
            return "Swap";
        case StepType::VisitVertex:
            return "VisitVertex";
        case StepType::RelaxEdge:
            return "RelaxEdge";
        case StepType::UpdateValue:
            return "UpdateValue";
        case StepType::PushToQueue:
            return "PushToQueue";
        case StepType::PopFromQueue:
            return "PopFromQueue";
        case StepType::Backtrack:
            return "Backtrack";
        case StepType::Prune:
            return "Prune";
        case StepType::Finish:
            return "Finish";
        }

        return "Unknown";
    }
}
