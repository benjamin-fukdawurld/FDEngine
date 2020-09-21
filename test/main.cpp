#include <FDEngine/BaseEngine.h>
#include <FDEngine/StateMachine/AbstractEngineStateMachineController.h>
#include <FDEngine/StateMachine/EngineStateMachine.h>
#include <gtest/gtest.h>
#include <iostream>

using namespace FDEngine;
using namespace FDEngine::StateMachine;

class Controller : public AbstractEngineStateMachineController
{
  public:
    virtual ~Controller() = default;

    virtual void onNoErrorStateEntry(const LaunchEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onNoErrorStateEntry(const ErrorHandledEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onNoErrorStateExit(const ErrorEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onNoError(const ErrorHandledEvent &) { std::cout << __func__ << std::endl; }

    virtual void onErrorStateEntry(const ErrorEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onErrorStateExit(const ErrorHandledEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onError(const ErrorEvent &) { std::cout << __func__ << std::endl; }

    virtual void onFatalErrorStateEntry(const ErrorEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onFatalErrorStateEntry(const ErrorHandledEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onFatalErrorStateExit(EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onFatalError(const ErrorEvent &) { std::cout << __func__ << std::endl; }
    virtual void onFatalError(const ErrorHandledEvent &) { std::cout << __func__ << std::endl; }

    virtual void onLaunchStateEntry(const LaunchEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onLaunchStateExit(const InitPendingEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }

    virtual void onInitStateEntry(const InitPendingEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onInitStateExit(const InitDoneEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onInit(const InitPendingEvent &) { std::cout << __func__ << std::endl; }

    virtual void onIdleStateEntry(const InitDoneEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onIdleStateEntry(const RenderDoneEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onIdleStateExit(const QuitEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onIdleStateExit(const ContinueEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onIdle(const InitDoneEvent &) { std::cout << __func__ << std::endl; }
    virtual void onIdle(const RenderDoneEvent &) { std::cout << __func__ << std::endl; }

    virtual void onQuitStateEntry(const QuitEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onQuitStateExit(EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onQuit(const QuitEvent &) { std::cout << __func__ << std::endl; }

    virtual void onNotUpToDateStateEntry(const ContinueEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onNotUpToDateStateExit(const UpdatePendingEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onNotUpToDate(const ContinueEvent &) { std::cout << __func__ << std::endl; }

    virtual void onUpdatedStateEntry(const UpdatePendingEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onUpdatedStateExit(const UpdateDoneEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onUpdate(const UpdatePendingEvent &) { std::cout << __func__ << std::endl; }

    virtual void onNotRenderedStateEntry(const UpdateDoneEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onNotRenderedStateExit(const RenderPendingEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onNotRendered(const UpdateDoneEvent &) { std::cout << __func__ << std::endl; }

    virtual void onRenderedStateEntry(const RenderPendingEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onRenderedStateExit(const RenderDoneEvent &, EngineStateMachineDefinition &)
    {
        std::cout << __func__ << std::endl;
    }
    virtual void onRendered(const RenderPendingEvent &) { std::cout << __func__ << std::endl; }
};

int main(int argc, char **argv)
{
    BaseEngine eng(new Controller());
    eng.init();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}