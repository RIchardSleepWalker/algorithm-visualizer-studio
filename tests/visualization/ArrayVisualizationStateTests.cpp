#include "avs/core/algorithm/AlgorithmStep.hpp"
#include "avs/core/algorithm/StepPayload.hpp"
#include "avs/visualization/array/ArrayVisualizationState.hpp"

#include <gtest/gtest.h>

#include <optional>
#include <vector>

namespace
{
    using avs::core::algorithm::AlgorithmCategory;
    using avs::core::algorithm::AlgorithmStep;
    using avs::core::algorithm::ArrayStepPayload;
    using avs::core::algorithm::StepPayload;
    using avs::core::algorithm::StepType;
    using avs::visualization::array::ArrayVisualizationState;
}

TEST(ArrayVisualizationStateTests, CreatesStateFromPayload)
{
    const ArrayStepPayload payload{
        {5, 1, 4},
        {0, 2}
    };

    const ArrayVisualizationState state = ArrayVisualizationState::fromPayload(payload);

    ASSERT_EQ(state.size(), 3U);

    EXPECT_EQ(state.elements()[0].value, 5);
    EXPECT_TRUE(state.elements()[0].highlighted);

    EXPECT_EQ(state.elements()[1].value, 1);
    EXPECT_FALSE(state.elements()[1].highlighted);

    EXPECT_EQ(state.elements()[2].value, 4);
    EXPECT_TRUE(state.elements()[2].highlighted);
}

TEST(ArrayVisualizationStateTests, IgnoresHighlightedIndicesOutsideArrayRange)
{
    const ArrayStepPayload payload{
        {10, 20},
        {0, 5}
    };

    const ArrayVisualizationState state = ArrayVisualizationState::fromPayload(payload);

    ASSERT_EQ(state.size(), 2U);

    EXPECT_TRUE(state.elements()[0].highlighted);
    EXPECT_FALSE(state.elements()[1].highlighted);
}

TEST(ArrayVisualizationStateTests, CreatesStateFromAlgorithmStepWithArrayPayload)
{
    const StepPayload payload = ArrayStepPayload{
        {3, 2, 1},
        {1}
    };

    const AlgorithmStep step(
        0,
        StepType::Comparison,
        AlgorithmCategory::Sorting,
        "Compare",
        "Compare array values.",
        payload
    );

    const auto state = ArrayVisualizationState::fromStep(step);

    ASSERT_TRUE(state.has_value());
    ASSERT_EQ(state->size(), 3U);

    EXPECT_EQ(state->elements()[0].value, 3);
    EXPECT_FALSE(state->elements()[0].highlighted);

    EXPECT_EQ(state->elements()[1].value, 2);
    EXPECT_TRUE(state->elements()[1].highlighted);

    EXPECT_EQ(state->elements()[2].value, 1);
    EXPECT_FALSE(state->elements()[2].highlighted);
}

TEST(ArrayVisualizationStateTests, ReturnsNulloptForStepWithoutArrayPayload)
{
    const AlgorithmStep step(
        0,
        StepType::Initialization,
        AlgorithmCategory::Sorting,
        "Initialize",
        "Initialize algorithm."
    );

    const auto state = ArrayVisualizationState::fromStep(step);

    EXPECT_FALSE(state.has_value());
}

TEST(ArrayVisualizationStateTests, EmptyPayloadCreatesEmptyState)
{
    const ArrayStepPayload payload{
        {},
        {}
    };

    const ArrayVisualizationState state = ArrayVisualizationState::fromPayload(payload);

    EXPECT_TRUE(state.empty());
    EXPECT_EQ(state.size(), 0U);
}
