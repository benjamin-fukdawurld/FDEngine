#ifndef FDENGINE_ENGINESTATEMACHINEGUARDS_H
#define FDENGINE_ENGINESTATEMACHINEGUARDS_H

#include <FDEngine/StateMachine/EngineStateMachineEvents.h>

namespace FDEngine
{
    namespace StateMachine
    {
        struct FatalErrorGuard
        {
            template<typename Fsm, typename SourceState, typename TargetState>
            bool operator()(const ErrorEvent &evt, Fsm &, SourceState &, TargetState &)
            {
                return evt.isFatal;
            }

            template<typename Fsm, typename SourceState, typename TargetState>
            bool operator()(const ErrorHandledEvent &evt, Fsm &, SourceState &, TargetState &)
            {
                return !evt.success;
            }
        };
    } // namespace StateMachine
} // namespace FDEngine

#endif // FDENGINE_ENGINESTATEMACHINEGUARDS_H