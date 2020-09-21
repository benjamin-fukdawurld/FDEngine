#ifndef FDENGINE_ENGINESTATEMACHINESTATES_H
#define FDENGINE_ENGINESTATEMACHINESTATES_H

#define FUSION_MAX_VECTOR_SIZE 15

#include <FDEngine/StateMachine/EngineStateMachineEvents.h>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/states.hpp>
#include <string_view>

namespace FDEngine
{
    namespace StateMachine
    {
        struct AbstractState : public boost::msm::front::state<>
        {
            virtual std::string_view getStateName() const = 0;
        };

        struct NoErrorState final : public AbstractState
        {
            std::string_view getStateName() const final { return "NoErrorState"; }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_entry(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onNoErrorStateEntry(evt, fsm);
            }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_exit(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onNoErrorStateExit(evt, fsm);
            }

            template<typename EventType, typename FSM>
            void on_exit(const EventType &, FSM &) {}
        };

        struct ErrorState final : public AbstractState
        {
            std::string_view getStateName() const final { return "ErrorState"; }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_entry(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onErrorStateEntry(evt, fsm);
            }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_exit(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onErrorStateExit(evt, fsm);
            }

            template<typename EventType, typename FSM>
            void on_exit(const EventType &, FSM &) {}
        };

        struct FatalErrorState final : public boost::msm::front::terminate_state<>
        {
            std::string_view getStateName() const { return "FatalErrorState"; }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_entry(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onFatalErrorStateEntry(evt, fsm);
            }

            template<typename EventType, typename FSM>
            void on_exit(const EventType &, FSM &) {}
        };

        struct LaunchState final : public AbstractState
        {
            std::string_view getStateName() const final { return "LaunchState"; }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_entry(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onLaunchStateEntry(evt, fsm);
            }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_exit(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onLaunchStateExit(evt, fsm);
            }

            template<typename EventType, typename FSM>
            void on_exit(const EventType &, FSM &) {}
        };

        struct InitState final : public AbstractState
        {
            std::string_view getStateName() const final { return "InitState"; }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_entry(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onInitStateEntry(evt, fsm);
            }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_exit(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onInitStateExit(evt, fsm);
            }

            template<typename EventType, typename FSM>
            void on_exit(const EventType &, FSM &) {}
        };

        struct QuitState final : public AbstractState
        {
            std::string_view getStateName() const final { return "QuitState"; }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_entry(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onQuitStateEntry(evt, fsm);
            }

            template<typename EventType, typename FSM>
            void on_exit(const EventType &, FSM &fsm)
            {
                fsm.getController().onQuitStateExit(fsm);
            }
        };

        struct IdleState final : public AbstractState
        {
            std::string_view getStateName() const final { return "IdleState"; }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_entry(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onIdleStateEntry(evt, fsm);
            }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_exit(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onIdleStateExit(evt, fsm);
            }

            template<typename EventType, typename FSM>
            void on_exit(const EventType &, FSM &) {}
        };

        struct NotUpToDateState final : public AbstractState
        {
            std::string_view getStateName() const final { return "NotUpToDateState"; }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_entry(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onNotUpToDateStateEntry(evt, fsm);
            }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_exit(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onNotUpToDateStateExit(evt, fsm);
            }

            template<typename EventType, typename FSM>
            void on_exit(const EventType &, FSM &) {}
        };

        struct UpdatedState final : public AbstractState
        {
            std::string_view getStateName() const final { return "UpdatedState"; }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_entry(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onUpdatedStateEntry(evt, fsm);
            }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_exit(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onUdatedStateExit(evt, fsm);
            }

            template<typename EventType, typename FSM>
            void on_exit(const EventType &, FSM &) {}
        };

        struct NotRenderedState final : public AbstractState
        {
            std::string_view getStateName() const final { return "NotRenderedState"; }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_entry(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onNotRenderedStateEntry(evt, fsm);
            }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_exit(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onNotRenderedStateExit(evt, fsm);
            }

            template<typename EventType, typename FSM>
            void on_exit(const EventType &, FSM &) {}
        };

        struct RenderedState final : public AbstractState
        {
            std::string_view getStateName() const final { return "RenderedState"; }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_entry(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onRenderedStateEntry(evt, fsm);
            }

            template<typename EventType, typename FSM>
            std::enable_if_t<std::is_base_of_v<AbstractEvent, EventType>, void> on_exit(const EventType &evt, FSM &fsm)
            {
                fsm.getController().onRenderedStateExit(evt, fsm);
            }

            template<typename EventType, typename FSM>
            void on_exit(const EventType &, FSM &) {}
        };
    } // namespace StateMachine
} // namespace FDEngine


#endif // FDENGINE_ENGINESTATEMACHINESTATES_H