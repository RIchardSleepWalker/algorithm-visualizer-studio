#include "avs/core/algorithm/Timeline.hpp"

#include <gtest/gtest.h>

namespace
{
    using namespace avs::core::algorithm;

    AlgorithmStep makeStep(std::size_t index, StepType type = StepType::Comparison)
    {
        return AlgorithmStep(
            index,
            type,
            AlgorithmCategory::Sorting,
            "Test step",
            "Test step description."
        );
    }
}

TEST(TimelineTests, IsEmptyByDefault)
{
    const Timeline timeline;

    EXPECT_TRUE(timeline.empty());
    EXPECT_EQ(timeline.size(), 0U);
    EXPECT_FALSE(timeline.hasCurrentStep());
    EXPECT_EQ(timeline.currentStep(), nullptr);
    EXPECT_FALSE(timeline.canMoveForward());
    EXPECT_FALSE(timeline.canMoveBackward());
}

TEST(TimelineTests, AddsStepsWithoutSelectingCurrentStep)
{
    Timeline timeline;

    timeline.addStep(makeStep(0));
    timeline.addStep(makeStep(1));

    EXPECT_FALSE(timeline.empty());
    EXPECT_EQ(timeline.size(), 2U);
    EXPECT_FALSE(timeline.hasCurrentStep());
    EXPECT_TRUE(timeline.canMoveForward());
    EXPECT_FALSE(timeline.canMoveBackward());
}

TEST(TimelineTests, MovesForwardFromInitialPosition)
{
    Timeline timeline;

    timeline.addStep(makeStep(0));
    timeline.addStep(makeStep(1));

    const AlgorithmStep* firstStep = timeline.moveForward();

    ASSERT_NE(firstStep, nullptr);
    EXPECT_EQ(firstStep->index(), 0U);
    EXPECT_TRUE(timeline.hasCurrentStep());
    EXPECT_TRUE(timeline.canMoveForward());
    EXPECT_FALSE(timeline.canMoveBackward());
}

TEST(TimelineTests, MovesForwardThroughSteps)
{
    Timeline timeline;

    timeline.addStep(makeStep(0));
    timeline.addStep(makeStep(1));
    timeline.addStep(makeStep(2));

    EXPECT_EQ(timeline.moveForward()->index(), 0U);
    EXPECT_EQ(timeline.moveForward()->index(), 1U);
    EXPECT_EQ(timeline.moveForward()->index(), 2U);

    EXPECT_FALSE(timeline.canMoveForward());
    EXPECT_TRUE(timeline.canMoveBackward());
}

TEST(TimelineTests, DoesNotMoveForwardPastLastStep)
{
    Timeline timeline;

    timeline.addStep(makeStep(0));

    EXPECT_EQ(timeline.moveForward()->index(), 0U);
    EXPECT_EQ(timeline.moveForward()->index(), 0U);

    EXPECT_FALSE(timeline.canMoveForward());
}

TEST(TimelineTests, MovesBackward)
{
    Timeline timeline;

    timeline.addStep(makeStep(0));
    timeline.addStep(makeStep(1));

    timeline.moveForward();
    timeline.moveForward();

    const AlgorithmStep* previousStep = timeline.moveBackward();

    ASSERT_NE(previousStep, nullptr);
    EXPECT_EQ(previousStep->index(), 0U);
    EXPECT_FALSE(timeline.canMoveBackward());
}

TEST(TimelineTests, ResetClearsCurrentStepButKeepsSteps)
{
    Timeline timeline;

    timeline.addStep(makeStep(0));
    timeline.addStep(makeStep(1));

    timeline.moveForward();
    timeline.reset();

    EXPECT_EQ(timeline.size(), 2U);
    EXPECT_FALSE(timeline.hasCurrentStep());
    EXPECT_EQ(timeline.currentStep(), nullptr);
    EXPECT_TRUE(timeline.canMoveForward());
}

TEST(TimelineTests, ClearRemovesAllSteps)
{
    Timeline timeline;

    timeline.addStep(makeStep(0));
    timeline.addStep(makeStep(1));

    timeline.moveForward();
    timeline.clear();

    EXPECT_TRUE(timeline.empty());
    EXPECT_EQ(timeline.size(), 0U);
    EXPECT_FALSE(timeline.hasCurrentStep());
    EXPECT_EQ(timeline.currentStep(), nullptr);
}

TEST(TimelineTests, JumpsToBeginning)
{
    Timeline timeline;

    timeline.addStep(makeStep(0));
    timeline.addStep(makeStep(1));
    timeline.addStep(makeStep(2));

    const AlgorithmStep* step = timeline.jumpToBeginning();

    ASSERT_NE(step, nullptr);
    EXPECT_EQ(step->index(), 0U);
}

TEST(TimelineTests, JumpsToEnd)
{
    Timeline timeline;

    timeline.addStep(makeStep(0));
    timeline.addStep(makeStep(1));
    timeline.addStep(makeStep(2));

    const AlgorithmStep* step = timeline.jumpToEnd();

    ASSERT_NE(step, nullptr);
    EXPECT_EQ(step->index(), 2U);
}
