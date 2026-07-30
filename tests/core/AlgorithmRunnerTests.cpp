#include "avs/core/algorithm/AlgorithmRunner.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <string_view>

namespace
{
    using namespace avs::core::algorithm;

    class FakeStepper final : public IAlgorithmStepper
    {
    public:
        [[nodiscard]] std::string_view name() const noexcept override
        {
            return "Fake Algorithm";
        }

        [[nodiscard]] AlgorithmCategory category() const noexcept override
        {
            return AlgorithmCategory::Sorting;
        }

        [[nodiscard]] bool hasNextStep() const override
        {
            return currentStepIndex_ < totalSteps_;
        }

        [[nodiscard]] std::optional<AlgorithmStep> nextStep() override
        {
            if (!hasNextStep())
            {
                return std::nullopt;
            }

            const auto stepIndex = currentStepIndex_;
            ++currentStepIndex_;

            const StepType type = stepIndex + 1 == totalSteps_
                ? StepType::Finish
                : StepType::Comparison;

            return AlgorithmStep(
                stepIndex,
                type,
                AlgorithmCategory::Sorting,
                "Fake step",
                "Fake step description."
            );
        }

        void reset() override
        {
            currentStepIndex_ = 0;
        }

    private:
        std::size_t currentStepIndex_ = 0;
        std::size_t totalSteps_ = 3;
    };
}

TEST(AlgorithmRunnerTests, RejectsNullStepper)
{
    EXPECT_THROW(
        AlgorithmRunner(nullptr),
        std::invalid_argument
    );
}

TEST(AlgorithmRunnerTests, ExposesAlgorithmMetadata)
{
    AlgorithmRunner runner(std::make_unique<FakeStepper>());

    EXPECT_EQ(runner.algorithmName(), std::string_view("Fake Algorithm"));
    EXPECT_EQ(runner.algorithmCategory(), AlgorithmCategory::Sorting);
}

TEST(AlgorithmRunnerTests, StartsInReadyState)
{
    AlgorithmRunner runner(std::make_unique<FakeStepper>());

    EXPECT_EQ(runner.status(), ExecutionStatus::Ready);
    EXPECT_EQ(runner.currentStep(), nullptr);
    EXPECT_TRUE(runner.timeline().empty());
    EXPECT_TRUE(runner.canStepForward());
    EXPECT_FALSE(runner.canStepBackward());
}

TEST(AlgorithmRunnerTests, StepForwardGeneratesFirstStep)
{
    AlgorithmRunner runner(std::make_unique<FakeStepper>());

    const AlgorithmStep* step = runner.stepForward();

    ASSERT_NE(step, nullptr);
    EXPECT_EQ(step->index(), 0U);
    EXPECT_EQ(step->type(), StepType::Comparison);
    EXPECT_EQ(runner.timeline().size(), 1U);
    EXPECT_EQ(runner.status(), ExecutionStatus::Paused);
}

TEST(AlgorithmRunnerTests, StepForwardGeneratesStepsUntilFinished)
{
    AlgorithmRunner runner(std::make_unique<FakeStepper>());

    const AlgorithmStep* firstStep = runner.stepForward();

    ASSERT_NE(firstStep, nullptr);
    EXPECT_EQ(firstStep->index(), 0U);
    EXPECT_EQ(firstStep->type(), StepType::Comparison);
    EXPECT_EQ(runner.status(), ExecutionStatus::Paused);

    const AlgorithmStep* secondStep = runner.stepForward();

    ASSERT_NE(secondStep, nullptr);
    EXPECT_EQ(secondStep->index(), 1U);
    EXPECT_EQ(secondStep->type(), StepType::Comparison);
    EXPECT_EQ(runner.status(), ExecutionStatus::Paused);

    const AlgorithmStep* finishStep = runner.stepForward();

    ASSERT_NE(finishStep, nullptr);
    EXPECT_EQ(finishStep->index(), 2U);
    EXPECT_EQ(finishStep->type(), StepType::Finish);

    EXPECT_FALSE(runner.canStepForward());
    EXPECT_TRUE(runner.canStepBackward());
    EXPECT_EQ(runner.status(), ExecutionStatus::Finished);
    EXPECT_EQ(runner.timeline().size(), 3U);
}

TEST(AlgorithmRunnerTests, StepBackwardMovesThroughTimeline)
{
    AlgorithmRunner runner(std::make_unique<FakeStepper>());

    runner.stepForward();
    runner.stepForward();
    runner.stepForward();

    const AlgorithmStep* previousStep = runner.stepBackward();

    ASSERT_NE(previousStep, nullptr);
    EXPECT_EQ(previousStep->index(), 1U);
    EXPECT_EQ(runner.status(), ExecutionStatus::Paused);
    EXPECT_TRUE(runner.canStepForward());
}

TEST(AlgorithmRunnerTests, StepForwardUsesCachedTimelineAfterMovingBackward)
{
    AlgorithmRunner runner(std::make_unique<FakeStepper>());

    runner.stepForward();
    runner.stepForward();
    runner.stepForward();

    ASSERT_EQ(runner.currentStep()->index(), 2U);

    runner.stepBackward();

    ASSERT_EQ(runner.currentStep()->index(), 1U);

    const AlgorithmStep* cachedForwardStep = runner.stepForward();

    ASSERT_NE(cachedForwardStep, nullptr);
    EXPECT_EQ(cachedForwardStep->index(), 2U);
    EXPECT_EQ(runner.timeline().size(), 3U);
    EXPECT_EQ(runner.status(), ExecutionStatus::Finished);
}

TEST(AlgorithmRunnerTests, ResetClearsTimelineAndStepperState)
{
    AlgorithmRunner runner(std::make_unique<FakeStepper>());

    runner.stepForward();
    runner.stepForward();

    ASSERT_EQ(runner.timeline().size(), 2U);

    runner.reset();

    EXPECT_EQ(runner.status(), ExecutionStatus::Ready);
    EXPECT_TRUE(runner.timeline().empty());
    EXPECT_EQ(runner.currentStep(), nullptr);
    EXPECT_TRUE(runner.canStepForward());
    EXPECT_FALSE(runner.canStepBackward());

    const AlgorithmStep* stepAfterReset = runner.stepForward();

    ASSERT_NE(stepAfterReset, nullptr);
    EXPECT_EQ(stepAfterReset->index(), 0U);
}
