#include "avs/visualization/VisualizationModule.hpp"

#include "avs/core/Version.hpp"

#include <string>

namespace avs::visualization
{
    std::string VisualizationModule::name() const
    {
        return "Algorithm Visualizer Studio - Visualization Module v"
            + std::string(avs::core::version());
    }
}
