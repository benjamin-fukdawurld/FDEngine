#ifndef FDENGINE_ENGINESTATEMACHINE_H
#define FDENGINE_ENGINESTATEMACHINE_H

#include <FDEngine/StateMachine/EngineStateMachineDefinition.h>
#include <boost/msm/back/state_machine.hpp>

namespace FDEngine
{
    namespace StateMachine
    {
        typedef boost::msm::back::state_machine<EngineStateMachineDefinition> EngineStateMachine;
    } // namespace StateMachine
} // namespace FDEngine

#endif // FDENGINE_ENGINESTATEMACHINE_H