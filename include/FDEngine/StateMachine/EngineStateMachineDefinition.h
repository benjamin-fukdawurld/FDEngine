#ifndef FDENGINE_ENGINESTATEMACHINEDEFINITION_H
#define FDENGINE_ENGINESTATEMACHINEDEFINITION_H

#include <FDEngine/StateMachine/AbstractEngineStateMachineController.h>
#include <FDEngine/StateMachine/EngineTransitionTable.h>
#include <boost/msm/front/state_machine_def.hpp>

namespace FDEngine
{
    namespace StateMachine
    {
        class EngineStateMachineDefinition :
            public boost::msm::front::state_machine_def<EngineStateMachineDefinition>
        {
          public:
            typedef EngineTransitionTable transition_table;
            typedef boost::mpl::vector<NoErrorState, LaunchState> initial_state;
            typedef LaunchEvent initial_event;
            //typedef QuitEvent exit_event;
            //typedef int no_exception_thrown;

          private:
            std::unique_ptr<AbstractEngineStateMachineController> m_controller;

          public:
            EngineStateMachineDefinition(AbstractEngineStateMachineController *controller) :
                boost::msm::front::state_machine_def<EngineStateMachineDefinition>(),
                m_controller(controller)
            {
                assert(m_controller != nullptr && "Engine state machine requires a controller");
            }

            AbstractEngineStateMachineController &getController() { return *m_controller; }
            const AbstractEngineStateMachineController &getController() const
            {
                return *m_controller;
            }
        };
    } // namespace StateMachine
} // namespace FDEngine

#endif // FDENGINE_ENGINESTATEMACHINE_H