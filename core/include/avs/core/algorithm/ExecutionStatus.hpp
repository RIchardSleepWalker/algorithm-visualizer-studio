#pragma once

#include <string_view>

namespace avs::core::algorithm
{
    enum class ExecutionStatus
    {
        Idle,
        Ready,
        Running,
        Paused,
        SteppingForward,
        SteppingBackward,
        Finished,
        Cancelled,
        Error
    };

    [[nodiscard]] constexpr std::string_view to_string(ExecutionStatus status) noexcept
    {
        switch (status)
        {
        case ExecutionStatus::Idle:
            return "Idle";
        case ExecutionStatus::Ready:
            return "Ready";
        case ExecutionStatus::Running:
            return "Running";
        case ExecutionStatus::Paused:
            return "Paused";
        case ExecutionStatus::SteppingForward:
            return "SteppingForward";
        case ExecutionStatus::SteppingBackward:
            return "SteppingBackward";
        case ExecutionStatus::Finished:
            return "Finished";
        case ExecutionStatus::Cancelled:
            return "Cancelled";
        case ExecutionStatus::Error:
            return "Error";
        }

        return "Error";
    }
}
