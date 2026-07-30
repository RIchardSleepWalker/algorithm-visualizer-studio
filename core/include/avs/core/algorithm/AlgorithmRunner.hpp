#pragma once

#include "avs/core/algorithm/AlgorithmCategory.hpp"
#include "avs/core/algorithm/AlgorithmStep.hpp"
#include "avs/core/algorithm/ExecutionStatus.hpp"
#include "avs/core/algorithm/IAlgorithmStepper.hpp"
#include "avs/core/algorithm/Timeline.hpp"

#include <memory>
#include <string_view>

namespace avs::core::algorithm
{
    class AlgorithmRunner
    {
    public:
        explicit AlgorithmRunner(std::unique_ptr<IAlgorithmStepper> stepper);

        [[nodiscard]] std::string_view algorithmName() const noexcept;
        [[nodiscard]] AlgorithmCategory algorithmCategory() const noexcept;
        [[nodiscard]] ExecutionStatus status() const noexcept;

        [[nodiscard]] const Timeline& timeline() const noexcept;
        [[nodiscard]] const AlgorithmStep* currentStep() const noexcept;

        [[nodiscard]] bool canStepForward() const;
        [[nodiscard]] bool canStepBackward() const noexcept;

        const AlgorithmStep* stepForward();
        const AlgorithmStep* stepBackward();

        void reset();

    private:
        void updateStatusAfterStep();

    private:
        std::unique_ptr<IAlgorithmStepper> stepper_;
        Timeline timeline_;
        ExecutionStatus status_ = ExecutionStatus::Ready;
    };
}
