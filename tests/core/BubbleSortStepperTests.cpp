#include "avs/core/algorithm/sorting/BubbleSortStepper.hpp"
#include "avs/core/algorithm/StepPayload.hpp"

#include <gtest/gtest.h>

#include <optional>
#include <vector>
#include <variant>

namespace
{
    using avs::core::algorithm::AlgorithmCategory;
    using avs::core::algorithm::StepType;
    using avs::core::algorithm::sorting::BubbleSortStepper;

    void consumeAllSteps(BubbleSortStepper& stepper)
    {
        while (stepper.hasNextStep())
        {
            const auto step = stepper.nextStep();
            ASSERT_TRUE(step.has_value());
        }
    }
}

TEST(BubbleSortStepperTests, ProvidesNameAndCategory)
{
    const BubbleSortStepper stepper({ 3, 2, 1 });

    EXPECT_EQ(stepper.name(), "Bubble Sort");
    EXPECT_EQ(stepper.category(), AlgorithmCategory::Sorting);
}

TEST(BubbleSortStepperTests, FirstStepIsInitialization)
{
    BubbleSortStepper stepper({ 3, 2, 1 });

    const auto step = stepper.nextStep();

    ASSERT_TRUE(step.has_value());
    EXPECT_EQ(step->index(), 0U);
    EXPECT_EQ(step->type(), StepType::Initialization);
    EXPECT_EQ(step->category(), AlgorithmCategory::Sorting);
}

TEST(BubbleSortStepperTests, GeneratesComparisonStep)
{
    BubbleSortStepper stepper({ 1, 2 });

    ASSERT_TRUE(stepper.nextStep().has_value());

    const auto comparisonStep = stepper.nextStep();

    ASSERT_TRUE(comparisonStep.has_value());
    EXPECT_EQ(comparisonStep->type(), StepType::Comparison);
}

TEST(BubbleSortStepperTests, GeneratesSwapStepWhenValuesAreOutOfOrder)
{
    BubbleSortStepper stepper({ 2, 1 });

    ASSERT_TRUE(stepper.nextStep().has_value());

    const auto comparisonStep = stepper.nextStep();

    ASSERT_TRUE(comparisonStep.has_value());
    EXPECT_EQ(comparisonStep->type(), StepType::Comparison);

    const auto swapStep = stepper.nextStep();

    ASSERT_TRUE(swapStep.has_value());
    EXPECT_EQ(swapStep->type(), StepType::Swap);

    EXPECT_EQ(stepper.values(), std::vector<int>({ 1, 2 }));
}

TEST(BubbleSortStepperTests, SortsValuesAfterConsumingAllSteps)
{
    BubbleSortStepper stepper({ 5, 1, 4, 2, 8 });

    consumeAllSteps(stepper);

    EXPECT_EQ(stepper.values(), std::vector<int>({ 1, 2, 4, 5, 8 }));
    EXPECT_FALSE(stepper.hasNextStep());
}

TEST(BubbleSortStepperTests, HandlesAlreadySortedInput)
{
    BubbleSortStepper stepper({ 1, 2, 3, 4 });

    consumeAllSteps(stepper);

    EXPECT_EQ(stepper.values(), std::vector<int>({ 1, 2, 3, 4 }));
    EXPECT_FALSE(stepper.hasNextStep());
}

TEST(BubbleSortStepperTests, HandlesSingleElementInput)
{
    BubbleSortStepper stepper({ 42 });

    const auto initializationStep = stepper.nextStep();

    ASSERT_TRUE(initializationStep.has_value());
    EXPECT_EQ(initializationStep->type(), StepType::Initialization);

    const auto finishStep = stepper.nextStep();

    ASSERT_TRUE(finishStep.has_value());
    EXPECT_EQ(finishStep->type(), StepType::Finish);

    EXPECT_FALSE(stepper.hasNextStep());
    EXPECT_EQ(stepper.values(), std::vector<int>({ 42 }));
}

TEST(BubbleSortStepperTests, ResetRestoresInitialState)
{
    BubbleSortStepper stepper({ 2, 1 });

    ASSERT_TRUE(stepper.nextStep().has_value());
    ASSERT_TRUE(stepper.nextStep().has_value());
    ASSERT_TRUE(stepper.nextStep().has_value());

    EXPECT_EQ(stepper.values(), std::vector<int>({ 1, 2 }));

    stepper.reset();

    EXPECT_EQ(stepper.values(), std::vector<int>({ 2, 1 }));
    EXPECT_TRUE(stepper.hasNextStep());

    const auto firstStepAfterReset = stepper.nextStep();

    ASSERT_TRUE(firstStepAfterReset.has_value());
    EXPECT_EQ(firstStepAfterReset->index(), 0U);
    EXPECT_EQ(firstStepAfterReset->type(), StepType::Initialization);
}

TEST(BubbleSortStepperTests, ReturnsNulloptAfterFinished)
{
    BubbleSortStepper stepper({ 1 });

    ASSERT_TRUE(stepper.nextStep().has_value());
    ASSERT_TRUE(stepper.nextStep().has_value());

    EXPECT_FALSE(stepper.hasNextStep());

    const auto noStep = stepper.nextStep();

    EXPECT_FALSE(noStep.has_value());
}

TEST(BubbleSortStepperTests, ComparisonStepContainsArrayPayload)
{
    BubbleSortStepper stepper({ 5, 1, 4 });

    ASSERT_TRUE(stepper.nextStep().has_value());

    const auto comparisonStep = stepper.nextStep();

    ASSERT_TRUE(comparisonStep.has_value());
    ASSERT_TRUE(hasPayload(comparisonStep->payload()));

    const auto& payload = std::get<avs::core::algorithm::ArrayStepPayload>(
        comparisonStep->payload()
    );

    EXPECT_EQ(payload.values, std::vector<int>({ 5, 1, 4 }));
    EXPECT_EQ(payload.highlightedIndices, std::vector<std::size_t>({ 0, 1 }));
}

TEST(BubbleSortStepperTests, SwapStepContainsUpdatedArrayPayload)
{
    BubbleSortStepper stepper({ 5, 1, 4 });

    ASSERT_TRUE(stepper.nextStep().has_value());
    ASSERT_TRUE(stepper.nextStep().has_value());

    const auto swapStep = stepper.nextStep();

    ASSERT_TRUE(swapStep.has_value());
    ASSERT_TRUE(hasPayload(swapStep->payload()));

    const auto& payload = std::get<avs::core::algorithm::ArrayStepPayload>(
        swapStep->payload()
    );

    EXPECT_EQ(payload.values, std::vector<int>({ 1, 5, 4 }));
    EXPECT_EQ(payload.highlightedIndices, std::vector<std::size_t>({ 0, 1 }));
}
