#include "avs/core/algorithm/AlgorithmStep.hpp"
#include "avs/core/algorithm/ExecutionStatus.hpp"

#include <gtest/gtest.h>

#include <stdexcept>
#include <string_view>

namespace
{
    using namespace avs::core::algorithm;
}

TEST(AlgorithmStepTests, StoresProvidedValues)
{
    const AlgorithmStep step(
        3,
        StepType::Comparison,
        AlgorithmCategory::Sorting,
        "Compare values",
        "Compare array elements at indices 0 and 1."
    );

    EXPECT_EQ(step.index(), 3U);
    EXPECT_EQ(step.type(), StepType::Comparison);
    EXPECT_EQ(step.category(), AlgorithmCategory::Sorting);
    EXPECT_EQ(step.title(), std::string_view("Compare values"));
    EXPECT_EQ(step.description(), std::string_view("Compare array elements at indices 0 and 1."));
}

TEST(AlgorithmStepTests, RejectsEmptyTitle)
{
    EXPECT_THROW(
        AlgorithmStep(
            0,
            StepType::Initialization,
            AlgorithmCategory::Sorting,
            "",
            "Initialize sorting algorithm."
        ),
        std::invalid_argument
    );
}

TEST(AlgorithmStepTests, ConvertsStepTypeToString)
{
    EXPECT_EQ(to_string(StepType::Swap), std::string_view("Swap"));
    EXPECT_EQ(to_string(StepType::RelaxEdge), std::string_view("RelaxEdge"));
}

TEST(AlgorithmStepTests, ConvertsAlgorithmCategoryToString)
{
    EXPECT_EQ(to_string(AlgorithmCategory::Graph), std::string_view("Graph"));
    EXPECT_EQ(to_string(AlgorithmCategory::Optimization), std::string_view("Optimization"));
}

TEST(AlgorithmStepTests, ConvertsExecutionStatusToString)
{
    EXPECT_EQ(to_string(ExecutionStatus::Running), std::string_view("Running"));
    EXPECT_EQ(to_string(ExecutionStatus::Finished), std::string_view("Finished"));
}
