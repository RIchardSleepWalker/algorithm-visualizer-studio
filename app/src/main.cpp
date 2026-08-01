#include "avs/core/Version.hpp"
#include "avs/core/algorithm/AlgorithmRunner.hpp"
#include "avs/core/algorithm/sorting/BubbleSortStepper.hpp"

#include <iostream>
#include <memory>
#include <string_view>
#include <vector>

namespace
{
    void printValues(const std::vector<int>& values)
    {
        std::cout << '[';

        for (std::size_t i = 0; i < values.size(); ++i)
        {
            std::cout << values[i];

            if (i + 1 < values.size())
            {
                std::cout << ", ";
            }
        }

        std::cout << ']';
    }

    void printStep(const avs::core::algorithm::AlgorithmStep& step)
    {
        std::cout
            << "Step " << step.index()
            << " | Type: " << avs::core::algorithm::to_string(step.type())
            << " | Title: " << step.title()
            << '\n';

        std::cout << "  " << step.description() << '\n';
    }
}

int main()
{
    using avs::core::algorithm::AlgorithmRunner;
    using avs::core::algorithm::sorting::BubbleSortStepper;

    std::vector<int> inputValues{ 5, 1, 4, 2, 8 };

    std::cout << "Algorithm Visualizer Studio\n";
    std::cout << "Core version: " << avs::core::version() << "\n\n";

    std::cout << "Input values: ";
    printValues(inputValues);
    std::cout << "\n\n";

    auto stepper = std::make_unique<BubbleSortStepper>(inputValues);
    AlgorithmRunner runner(std::move(stepper));

    std::cout << "Algorithm: " << runner.algorithmName() << '\n';
    std::cout << "Category: " << avs::core::algorithm::to_string(runner.algorithmCategory()) << "\n\n";

    while (runner.canStepForward())
    {
        const avs::core::algorithm::AlgorithmStep* step = runner.stepForward();

        if (step == nullptr)
        {
            break;
        }

        printStep(*step);
    }

    std::cout << "\nExecution status: "
        << avs::core::algorithm::to_string(runner.status())
        << '\n';

    std::cout << "Generated steps: "
        << runner.timeline().size()
        << '\n';

    return 0;
}
