#pragma once

#include <string_view>

namespace avs::core::algorithm
{
    enum class AlgorithmCategory
    {
        Unknown,
        Sorting,
        Graph,
        Array,
        Statistics,
        Optimization
    };

    [[nodiscard]] constexpr std::string_view to_string(AlgorithmCategory category) noexcept
    {
        switch (category)
        {
        case AlgorithmCategory::Unknown:
            return "Unknown";
        case AlgorithmCategory::Sorting:
            return "Sorting";
        case AlgorithmCategory::Graph:
            return "Graph";
        case AlgorithmCategory::Array:
            return "Array";
        case AlgorithmCategory::Statistics:
            return "Statistics";
        case AlgorithmCategory::Optimization:
            return "Optimization";
        }

        return "Unknown";
    }
}
