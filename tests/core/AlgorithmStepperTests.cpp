#include "avs/core/algorithm/IAlgorithmStepper.hpp"

#include <gtest/gtest.h>

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
            return "Fake Stepper";
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

            return AlgorithmStep(
                stepIndex,
                StepType::Comparison,
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
        std::size_t totalSteps_ = 2;
    };
}

TEST(AlgorithmStepperTests, ProvidesNameAndCategory)
{
    const FakeStepper stepper;

    EXPECT_EQ(stepper.name(), std::string_view("Fake Stepper"));
    EXPECT_EQ(stepper.category(), AlgorithmCategory::Sorting);
}

TEST(AlgorithmStepperTests, GeneratesStepsUntilFinished)
{
    FakeStepper stepper;

    EXPECT_TRUE(stepper.hasNextStep());

    const auto firstStep = stepper.nextStep();

    ASSERT_TRUE(firstStep.has_value());
    EXPECT_EQ(firstStep->index(), 0U);
    EXPECT_EQ(firstStep->type(), StepType::Comparison);

    const auto secondStep = stepper.nextStep();

    ASSERT_TRUE(secondStep.has_value());
    EXPECT_EQ(secondStep->index(), 1U);

    EXPECT_FALSE(stepper.hasNextStep());

    const auto noStep = stepper.nextStep();

    EXPECT_FALSE(noStep.has_value());
}

TEST(AlgorithmStepperTests, ResetRestoresInitialState)
{
    FakeStepper stepper;

    EXPECT_TRUE(stepper.nextStep().has_value());
    EXPECT_TRUE(stepper.nextStep().has_value());
    EXPECT_FALSE(stepper.hasNextStep());

    stepper.reset();

    EXPECT_TRUE(stepper.hasNextStep());

    const auto stepAfterReset = stepper.nextStep();

    ASSERT_TRUE(stepAfterReset.has_value());
    EXPECT_EQ(stepAfterReset->index(), 0U);
}
