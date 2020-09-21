#ifndef FDENGINE_ENGINESTATEMACHINEACTIONS_H
#define FDENGINE_ENGINESTATEMACHINEACTIONS_H

#include <FDEngine/StateMachine/EngineStateMachineEvents.h>
#include <FDEngine/StateMachine/EngineStateMachineStates.h>
#include <boost/msm/front/state_machine_def.hpp>


namespace FDEngine
{
    namespace StateMachine
    {
        class EngineStateMachineDefinition;

        struct NoErrorAction
        {
            void operator()(const ErrorHandledEvent &evt,
                            EngineStateMachineDefinition &fsm,
                            ErrorState &sourceState,
                            NoErrorState &targetState);
        };

        struct ErrorAction
        {
            void operator()(const ErrorEvent &evt,
                            EngineStateMachineDefinition &fsm,
                            NoErrorState &sourceState,
                            ErrorState &targetState);
        };

        struct FatalErrorAction
        {
            void operator()(const ErrorEvent &evt,
                            EngineStateMachineDefinition &fsm,
                            NoErrorState &sourceState,
                            FatalErrorState &targetState);

            void operator()(const ErrorHandledEvent &evt,
                            EngineStateMachineDefinition &fsm,
                            ErrorState &sourceState,
                            FatalErrorState &targetState);
        };

        struct InitAction
        {
            void operator()(const InitPendingEvent &evt,
                            EngineStateMachineDefinition &fsm,
                            LaunchState &sourceState,
                            InitState &targetState);
        };

        struct IdleAction
        {
            void operator()(const InitDoneEvent &evt,
                            EngineStateMachineDefinition &fsm,
                            InitState &sourceState,
                            IdleState &targetState);

            void operator()(const RenderDoneEvent &evt,
                            EngineStateMachineDefinition &fsm,
                            RenderedState &sourceState,
                            IdleState &targetState);
        };

        struct QuitAction
        {
            void operator()(const QuitEvent &evt,
                            EngineStateMachineDefinition &fsm,
                            IdleState &sourceState,
                            QuitState &targetState);
        };

        struct NotUpToDateAction
        {
            void operator()(const ContinueEvent &evt,
                            EngineStateMachineDefinition &fsm,
                            IdleState &sourceState,
                            NotUpToDateState &targetState);
        };

        struct UpdateAction
        {
            void operator()(const UpdatePendingEvent &evt,
                            EngineStateMachineDefinition &fsm,
                            NotUpToDateState &sourceState,
                            UpdatedState &targetState);
        };

        struct NotRenderedAction
        {
            void operator()(const UpdateDoneEvent &evt,
                            EngineStateMachineDefinition &fsm,
                            UpdatedState &sourceState,
                            NotRenderedState &targetState);
        };

        struct RenderedAction
        {
            void operator()(const RenderPendingEvent &evt,
                            EngineStateMachineDefinition &fsm,
                            NotRenderedState &sourceState,
                            RenderedState &targetState);
        };
    } // namespace StateMachine
} // namespace FDEngine

#endif // FDENGINE_ENGINESTATEMACHINEACTIONS_H