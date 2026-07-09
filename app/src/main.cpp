#include "avs/core/Version.hpp"
#include "avs/visualization/VisualizationModule.hpp"

#include <iostream>

int main()
{
    const avs::visualization::VisualizationModule visualizationModule;

    std::cout << "Algorithm Visualizer Studio\n";
    std::cout << "Core version: " << avs::core::version() << '\n';
    std::cout << visualizationModule.name() << '\n';

    return 0;
}
