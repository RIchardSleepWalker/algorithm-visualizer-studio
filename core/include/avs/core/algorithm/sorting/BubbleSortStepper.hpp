#pragma once

#include "avs/core/algorithm/IAlgorithmStepper.hpp"

#include <cstddef>
#include <optional>
#include <string_view>
#include <vector>

namespace avs::core::algorithm::sorting
{
    class BubbleSortStepper final : public IAlgorithmStepper
    {
    public:
        explicit BubbleSortStepper(std::vector<int> values);

        [[nodiscard]] std::string_view name() const noexcept override;
        [[nodiscard]] AlgorithmCategory category() const noexcept override;

        [[nodiscard]] bool hasNextStep() const override;
        [[nodiscard]] std::optional<AlgorithmStep> nextStep() override;

        void reset() override;

        [[nodiscard]] const std::vector<int>& values() const noexcept;

    private:
        [[nodiscard]] bool isSortingCompleted() const noexcept;

        [[nodiscard]] AlgorithmStep makeInitializationStep();
        [[nodiscard]] AlgorithmStep makeComparisonStep();
        [[nodiscard]] AlgorithmStep makeSwapStep();
        [[nodiscard]] AlgorithmStep makeFinishStep();

        void advanceComparison() noexcept;

    private:
        std::vector<int> initialValues_;
        std::vector<int> values_;

        std::size_t stepIndex_ = 0;
        std::size_t passIndex_ = 0;
        std::size_t compareIndex_ = 0;

        bool initialized_ = false;
        bool finished_ = false;
        bool pendingSwap_ = false;
    };
}
