#ifndef FDENGINE_ABSTRACTENGINESTATEMACHINECONTROLLER_H
#define FDENGINE_ABSTRACTENGINESTATEMACHINECONTROLLER_H

#include <FDEngine/StateMachine/EngineStateMachineEvents.h>
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>

namespace FDEngine
{
    namespace StateMachine
    {
        class EngineStateMachineDefinition;

        class AbstractEngineStateMachineController
        {
          public:
            virtual ~AbstractEngineStateMachineController() = default;

            virtual void onNoErrorStateEntry(const LaunchEvent &event,
                                             EngineStateMachineDefinition &fsm) = 0;
            virtual void onNoErrorStateEntry(const ErrorHandledEvent &event,
                                             EngineStateMachineDefinition &fsm) = 0;
            virtual void onNoErrorStateExit(const ErrorEvent &event,
                                            EngineStateMachineDefinition &fsm) = 0;
            virtual void onNoError(const ErrorHandledEvent &evt) = 0;

            virtual void onErrorStateEntry(const ErrorEvent &event,
                                           EngineStateMachineDefinition &fsm) = 0;
            virtual void onErrorStateExit(const ErrorHandledEvent &event,
                                          EngineStateMachineDefinition &fsm) = 0;
            virtual void onError(const ErrorEvent &evt) = 0;

            virtual void onFatalErrorStateEntry(const ErrorEvent &event,
                                                EngineStateMachineDefinition &fsm) = 0;
            virtual void onFatalErrorStateEntry(const ErrorHandledEvent &event,
                                                EngineStateMachineDefinition &fsm) = 0;
            virtual void onFatalErrorStateExit(EngineStateMachineDefinition &fsm) = 0;
            virtual void onFatalError(const ErrorEvent &evt) = 0;
            virtual void onFatalError(const ErrorHandledEvent &evt) = 0;

            virtual void onLaunchStateEntry(const LaunchEvent &event,
                                            EngineStateMachineDefinition &fsm) = 0;
            virtual void onLaunchStateExit(const InitPendingEvent &event,
                                           EngineStateMachineDefinition &fsm) = 0;

            virtual void onInitStateEntry(const InitPendingEvent &event,
                                          EngineStateMachineDefinition &fsm) = 0;
            virtual void onInitStateExit(const InitDoneEvent &event,
                                         EngineStateMachineDefinition &fsm) = 0;
            virtual void onInit(const InitPendingEvent &evt) = 0;

            virtual void onIdleStateEntry(const InitDoneEvent &event,
                                          EngineStateMachineDefinition &fsm) = 0;
            virtual void onIdleStateEntry(const RenderDoneEvent &event,
                                          EngineStateMachineDefinition &fsm) = 0;
            virtual void onIdleStateExit(const QuitEvent &event,
                                         EngineStateMachineDefinition &fsm) = 0;
            virtual void onIdleStateExit(const ContinueEvent &event,
                                         EngineStateMachineDefinition &fsm) = 0;
            virtual void onIdle(const InitDoneEvent &evt) = 0;
            virtual void onIdle(const RenderDoneEvent &evt) = 0;

            virtual void onQuitStateEntry(const QuitEvent &event,
                                          EngineStateMachineDefinition &fsm) = 0;
            virtual void onQuitStateExit(EngineStateMachineDefinition &fsm) = 0;
            virtual void onQuit(const QuitEvent &evt) = 0;

            virtual void onNotUpToDateStateEntry(const ContinueEvent &event,
                                                 EngineStateMachineDefinition &fsm) = 0;
            virtual void onNotUpToDateStateExit(const UpdatePendingEvent &event,
                                                EngineStateMachineDefinition &fsm) = 0;
            virtual void onNotUpToDate(const ContinueEvent &evt) = 0;

            virtual void onUpdatedStateEntry(const UpdatePendingEvent &event,
                                             EngineStateMachineDefinition &fsm) = 0;
            virtual void onUpdatedStateExit(const UpdateDoneEvent &event,
                                            EngineStateMachineDefinition &fsm) = 0;
            virtual void onUpdate(const UpdatePendingEvent &evt) = 0;

            virtual void onNotRenderedStateEntry(const UpdateDoneEvent &event,
                                                 EngineStateMachineDefinition &fsm) = 0;
            virtual void onNotRenderedStateExit(const RenderPendingEvent &event,
                                                EngineStateMachineDefinition &fsm) = 0;
            virtual void onNotRendered(const UpdateDoneEvent &evt) = 0;

            virtual void onRenderedStateEntry(const RenderPendingEvent &event,
                                              EngineStateMachineDefinition &fsm) = 0;
            virtual void onRenderedStateExit(const RenderDoneEvent &event,
                                             EngineStateMachineDefinition &fsm) = 0;
            virtual void onRendered(const RenderPendingEvent &evt) = 0;
        };
    } // namespace StateMachine
} // namespace FDEngine

#endif // FDENGINE_ABSTRACTENGINESTATEMACHINECONTROLLER_H