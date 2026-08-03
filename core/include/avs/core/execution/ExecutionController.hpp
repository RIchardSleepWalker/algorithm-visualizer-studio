#pragma once

#include "avs/core/algorithm/AlgorithmCategory.hpp"
#include "avs/core/algorithm/AlgorithmRunner.hpp"
#include "avs/core/algorithm/AlgorithmStep.hpp"
#include "avs/core/algorithm/ExecutionStatus.hpp"
#include "avs/core/algorithm/IAlgorithmStepper.hpp"
#include "avs/core/algorithm/Timeline.hpp"

#include <memory>
#include <string_view>

namespace avs::core::execution
{
    class ExecutionController
    {
    public:
        explicit ExecutionController(
            std::unique_ptr<algorithm::IAlgorithmStepper> stepper
        );

        [[nodiscard]] std::string_view algorithmName() const noexcept;
        [[nodiscard]] algorithm::AlgorithmCategory algorithmCategory() const noexcept;
        [[nodiscard]] algorithm::ExecutionStatus status() const noexcept;

        [[nodiscard]] const algorithm::Timeline& timeline() const noexcept;
        [[nodiscard]] const algorithm::AlgorithmStep* currentStep() const noexcept;

        [[nodiscard]] bool isRunning() const noexcept;
        [[nodiscard]] bool canStepForward() const;
        [[nodiscard]] bool canStepBackward() const noexcept;

        void start();
        void pause();
        void stop();
        void reset();

        const algorithm::AlgorithmStep* stepForward();
        const algorithm::AlgorithmStep* stepBackward();

        const algorithm::AlgorithmStep* tick();

    private:
        void updateStatusAfterManualStep();

    private:
        algorithm::AlgorithmRunner runner_;
        algorithm::ExecutionStatus status_ = algorithm::ExecutionStatus::Ready;
    };
}
