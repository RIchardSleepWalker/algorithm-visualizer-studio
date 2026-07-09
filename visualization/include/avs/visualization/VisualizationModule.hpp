#pragma once

#include <string>

namespace avs::visualization
{
    class VisualizationModule
    {
    public:
        [[nodiscard]] std::string name() const;
    };
}
