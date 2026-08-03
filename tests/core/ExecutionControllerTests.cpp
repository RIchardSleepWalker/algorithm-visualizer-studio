#include "avs/core/execution/ExecutionController.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <stdexcept>
#include <string_view>

namespace
{
    using namespace avs::core::algorithm;
    using avs::core::execution::ExecutionController;

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

TEST(ExecutionControllerTests, RejectsNullStepper)
{
    EXPECT_THROW(
        ExecutionController(nullptr),
        std::invalid_argument
    );
}

TEST(ExecutionControllerTests, StartsInReadyState)
{
    ExecutionController controller(std::make_unique<FakeStepper>());

    EXPECT_EQ(controller.status(), ExecutionStatus::Ready);
    EXPECT_FALSE(controller.isRunning());
    EXPECT_TRUE(controller.canStepForward());
    EXPECT_FALSE(controller.canStepBackward());
    EXPECT_EQ(controller.currentStep(), nullptr);
    EXPECT_TRUE(controller.timeline().empty());
}

TEST(ExecutionControllerTests, ExposesAlgorithmMetadata)
{
    ExecutionController controller(std::make_unique<FakeStepper>());

    EXPECT_EQ(controller.algorithmName(), std::string_view("Fake Algorithm"));
    EXPECT_EQ(controller.algorithmCategory(), AlgorithmCategory::Sorting);
}

TEST(ExecutionControllerTests, StartChangesStatusToRunning)
{
    ExecutionController controller(std::make_unique<FakeStepper>());

    controller.start();

    EXPECT_EQ(controller.status(), ExecutionStatus::Running);
    EXPECT_TRUE(controller.isRunning());
}

TEST(ExecutionControllerTests, PauseChangesRunningStatusToPaused)
{
    ExecutionController controller(std::make_unique<FakeStepper>());

    controller.start();
    controller.pause();

    EXPECT_EQ(controller.status(), ExecutionStatus::Paused);
    EXPECT_FALSE(controller.isRunning());
}

TEST(ExecutionControllerTests, TickDoesNothingWhenNotRunning)
{
    ExecutionController controller(std::make_unique<FakeStepper>());

    const AlgorithmStep* step = controller.tick();

    EXPECT_EQ(step, nullptr);
    EXPECT_EQ(controller.status(), ExecutionStatus::Ready);
    EXPECT_TRUE(controller.timeline().empty());
}

TEST(ExecutionControllerTests, TickAdvancesWhenRunning)
{
    ExecutionController controller(std::make_unique<FakeStepper>());

    controller.start();

    const AlgorithmStep* step = controller.tick();

    ASSERT_NE(step, nullptr);
    EXPECT_EQ(step->index(), 0U);
    EXPECT_EQ(controller.status(), ExecutionStatus::Running);
    EXPECT_EQ(controller.timeline().size(), 1U);
}

TEST(ExecutionControllerTests, TicksUntilFinished)
{
    ExecutionController controller(std::make_unique<FakeStepper>());

    controller.start();

    while (controller.isRunning())
    {
        controller.tick();
    }

    ASSERT_NE(controller.currentStep(), nullptr);
    EXPECT_EQ(controller.currentStep()->index(), 2U);
    EXPECT_EQ(controller.currentStep()->type(), StepType::Finish);
    EXPECT_EQ(controller.status(), ExecutionStatus::Finished);
    EXPECT_FALSE(controller.canStepForward());
    EXPECT_TRUE(controller.canStepBackward());
}

TEST(ExecutionControllerTests, ManualStepForwardPausesAfterStep)
{
    ExecutionController controller(std::make_unique<FakeStepper>());

    const AlgorithmStep* step = controller.stepForward();

    ASSERT_NE(step, nullptr);
    EXPECT_EQ(step->index(), 0U);
    EXPECT_EQ(controller.status(), ExecutionStatus::Paused);
}

TEST(ExecutionControllerTests, ManualStepForwardFinishesOnLastStep)
{
    ExecutionController controller(std::make_unique<FakeStepper>());

    controller.stepForward();
    controller.stepForward();
    const AlgorithmStep* finishStep = controller.stepForward();

    ASSERT_NE(finishStep, nullptr);
    EXPECT_EQ(finishStep->type(), StepType::Finish);
    EXPECT_EQ(controller.status(), ExecutionStatus::Finished);
}

TEST(ExecutionControllerTests, StepBackwardMovesToPreviousStepAndPauses)
{
    ExecutionController controller(std::make_unique<FakeStepper>());

    controller.stepForward();
    controller.stepForward();

    const AlgorithmStep* previousStep = controller.stepBackward();

    ASSERT_NE(previousStep, nullptr);
    EXPECT_EQ(previousStep->index(), 0U);
    EXPECT_EQ(controller.status(), ExecutionStatus::Paused);
}

TEST(ExecutionControllerTests, StopCancelsExecution)
{
    ExecutionController controller(std::make_unique<FakeStepper>());

    controller.start();
    controller.tick();

    controller.stop();

    EXPECT_EQ(controller.status(), ExecutionStatus::Cancelled);
    EXPECT_FALSE(controller.canStepForward());
    EXPECT_FALSE(controller.canStepBackward());

    const AlgorithmStep* currentStep = controller.currentStep();
    const AlgorithmStep* stepAfterStop = controller.stepForward();

    EXPECT_EQ(stepAfterStop, currentStep);
}

TEST(ExecutionControllerTests, ResetRestoresReadyState)
{
    ExecutionController controller(std::make_unique<FakeStepper>());

    controller.start();
    controller.tick();
    controller.stop();

    controller.reset();

    EXPECT_EQ(controller.status(), ExecutionStatus::Ready);
    EXPECT_TRUE(controller.canStepForward());
    EXPECT_FALSE(controller.canStepBackward());
    EXPECT_EQ(controller.currentStep(), nullptr);
    EXPECT_TRUE(controller.timeline().empty());

    const AlgorithmStep* stepAfterReset = controller.stepForward();

    ASSERT_NE(stepAfterReset, nullptr);
    EXPECT_EQ(stepAfterReset->index(), 0U);
}
