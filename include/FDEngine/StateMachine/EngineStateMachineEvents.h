#ifndef FDENGINE_ENGINESTATEMACHINEEVENTS_H
#define FDENGINE_ENGINESTATEMACHINEEVENTS_H

#include <string_view>

namespace FDEngine
{
    namespace StateMachine
    {
        struct AbstractEvent
        {
            virtual std::string_view getEventName() const = 0;
        };

        struct ErrorEvent final : AbstractEvent
        {
            bool isFatal;

            ErrorEvent(bool isFatal = false) : isFatal(isFatal) {}
            std::string_view getEventName() const final { return "ErrorEvent"; }
        };

        struct ErrorHandledEvent final : AbstractEvent
        {
            bool success;

            ErrorHandledEvent(bool success = true) : success(success) {}
            std::string_view getEventName() const final { return "ErrorHandledEvent"; }
        };

        struct LaunchEvent final : AbstractEvent
        {
            std::string_view getEventName() const final { return "LaunchEvent"; }
        };

        struct InitPendingEvent final : AbstractEvent
        {
            std::string_view getEventName() const final { return "InitPendingEvent"; }
        };

        struct InitDoneEvent final : AbstractEvent
        {
            std::string_view getEventName() const final { return "InitDoneEvent"; }
        };

        struct QuitEvent final : AbstractEvent
        {
            std::string_view getEventName() const final { return "QuitEvent"; }
        };

        struct ContinueEvent final : AbstractEvent
        {
            std::string_view getEventName() const final { return "ContinueEvent"; }
        };

        struct UpdatePendingEvent final : AbstractEvent
        {
            std::string_view getEventName() const final { return "UpdatePendingEvent"; }
        };

        struct UpdateDoneEvent final : AbstractEvent
        {
            std::string_view getEventName() const final { return "UpdateDoneEvent"; }
        };

        struct RenderPendingEvent final : AbstractEvent
        {
            std::string_view getEventName() const final { return "RenderPendingEvent"; }
        };

        struct RenderDoneEvent final : AbstractEvent
        {
            std::string_view getEventName() const final { return "RenderDoneEvent"; }
        };
    } // namespace StateMachine
} // namespace FDEngine

#endif // FDENGINE_ENGINESTATEMACHINEEVENTS_H