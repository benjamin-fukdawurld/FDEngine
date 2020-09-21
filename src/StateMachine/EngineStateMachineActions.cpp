#include <FDEngine/StateMachine/EngineStateMachineDefinition.h>

namespace fsm = FDEngine::StateMachine;

void fsm::NoErrorAction::operator()(const ErrorHandledEvent &evt,
                                    EngineStateMachineDefinition &fsm,
                                    ErrorState &,
                                    NoErrorState &)
{
    fsm.getController().onNoError(evt);
}

void fsm::ErrorAction::operator()(const ErrorEvent &evt,
                                  EngineStateMachineDefinition &fsm,
                                  NoErrorState &,
                                  ErrorState &)
{
    fsm.getController().onError(evt);
}

void fsm::FatalErrorAction::operator()(const ErrorEvent &evt,
                                       EngineStateMachineDefinition &fsm,
                                       NoErrorState &,
                                       FatalErrorState &)
{
    fsm.getController().onFatalError(evt);
}

void fsm::FatalErrorAction::operator()(const ErrorHandledEvent &evt,
                                       EngineStateMachineDefinition &fsm,
                                       ErrorState &,
                                       FatalErrorState &)
{
    fsm.getController().onFatalError(evt);
}

void fsm::InitAction::operator()(const InitPendingEvent &evt,
                                 EngineStateMachineDefinition &fsm,
                                 LaunchState &,
                                 InitState &)
{
    fsm.getController().onInit(evt);
}

void fsm::IdleAction::operator()(const InitDoneEvent &evt,
                                 EngineStateMachineDefinition &fsm,
                                 InitState &,
                                 IdleState &)
{
    fsm.getController().onIdle(evt);
}

void fsm::IdleAction::operator()(const RenderDoneEvent &evt,
                                 EngineStateMachineDefinition &fsm,
                                 RenderedState &,
                                 IdleState &)
{
    fsm.getController().onIdle(evt);
}

void fsm::QuitAction::operator()(const QuitEvent &evt,
                                 EngineStateMachineDefinition &fsm,
                                 IdleState &,
                                 QuitState &)
{
    fsm.getController().onQuit(evt);
}

void fsm::NotUpToDateAction::operator()(const ContinueEvent &evt,
                                        EngineStateMachineDefinition &fsm,
                                        IdleState &,
                                        NotUpToDateState &)
{
    fsm.getController().onNotUpToDate(evt);
}

void fsm::UpdateAction::operator()(const UpdatePendingEvent &evt,
                                   EngineStateMachineDefinition &fsm,
                                   NotUpToDateState &,
                                   UpdatedState &)
{
    fsm.getController().onUpdate(evt);
}

void fsm::NotRenderedAction::operator()(const UpdateDoneEvent &evt,
                                        EngineStateMachineDefinition &fsm,
                                        UpdatedState &,
                                        NotRenderedState &)
{
    fsm.getController().onNotRendered(evt);
}

void fsm::RenderedAction::operator()(const RenderPendingEvent &evt,
                                     EngineStateMachineDefinition &fsm,
                                     NotRenderedState &,
                                     RenderedState &)
{
    fsm.getController().onRendered(evt);
}