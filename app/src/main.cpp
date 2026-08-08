//#include "avs/core/Version.hpp"
//#include "avs/core/algorithm/AlgorithmRunner.hpp"
//#include "avs/core/algorithm/sorting/BubbleSortStepper.hpp"
//#include "avs/core/algorithm/StepPayload.hpp"
//
//#include <iostream>
//#include <memory>
//#include <string_view>
//#include <vector>
//#include <cstddef>
//#include <variant>
//
//namespace
//{
//    void printValues(const std::vector<int>& values)
//    {
//        std::cout << '[';
//
//        for (std::size_t i = 0; i < values.size(); ++i)
//        {
//            std::cout << values[i];
//
//            if (i + 1 < values.size())
//            {
//                std::cout << ", ";
//            }
//        }
//
//        std::cout << ']';
//    }
//
//    void printIndices(const std::vector<std::size_t>& indices)
//    {
//        std::cout << '[';
//
//        for (std::size_t i = 0; i < indices.size(); ++i)
//        {
//            std::cout << indices[i];
//
//            if (i + 1 < indices.size())
//            {
//                std::cout << ", ";
//            }
//        }
//
//        std::cout << ']';
//    }
//
//    void printStep(const avs::core::algorithm::AlgorithmStep& step)
//    {
//        std::cout
//            << "Step " << step.index()
//            << " | Type: " << avs::core::algorithm::to_string(step.type())
//            << " | Title: " << step.title()
//            << '\n';
//
//        std::cout << "  " << step.description() << '\n';
//
//        const auto* arrayPayload = std::get_if<avs::core::algorithm::ArrayStepPayload>(
//            &step.payload()
//        );
//
//        if (arrayPayload != nullptr)
//        {
//            std::cout << "  Values: ";
//            printValues(arrayPayload->values);
//            std::cout << '\n';
//
//            if (!arrayPayload->highlightedIndices.empty())
//            {
//                std::cout << "  Highlighted indices: ";
//                printIndices(arrayPayload->highlightedIndices);
//                std::cout << '\n';
//            }
//        }
//
//        std::cout << '\n';
//    }
//}
//
//int main()
//{
//    using avs::core::algorithm::AlgorithmRunner;
//    using avs::core::algorithm::sorting::BubbleSortStepper;
//
//    std::vector<int> inputValues{ 5, 1, 4, 2, 8 };
//
//    std::cout << "Algorithm Visualizer Studio\n";
//    std::cout << "Core version: " << avs::core::version() << "\n\n";
//
//    std::cout << "Input values: ";
//    printValues(inputValues);
//    std::cout << "\n\n";
//
//    auto stepper = std::make_unique<BubbleSortStepper>(inputValues);
//    AlgorithmRunner runner(std::move(stepper));
//
//    std::cout << "Algorithm: " << runner.algorithmName() << '\n';
//    std::cout << "Category: " << avs::core::algorithm::to_string(runner.algorithmCategory()) << "\n\n";
//
//    while (runner.canStepForward())
//    {
//        const avs::core::algorithm::AlgorithmStep* step = runner.stepForward();
//
//        if (step == nullptr)
//        {
//            break;
//        }
//
//        printStep(*step);
//    }
//
//    std::cout << "Execution status: "
//        << avs::core::algorithm::to_string(runner.status())
//        << '\n';
//
//    std::cout << "Generated steps: "
//        << runner.timeline().size()
//        << '\n';
//
//    return 0;
//}

#include "avs/ui/MainWindow.hpp"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    avs::ui::MainWindow window;
    window.show();

    return app.exec();
}
