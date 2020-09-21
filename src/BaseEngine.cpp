#include <FDEngine/BaseEngine.h>

FDEngine::BaseEngine::BaseEngine(StateMachine::AbstractEngineStateMachineController *controller) :
    FDCore::AbstractApplication(),
    m_stateMachine(controller)
{
}

bool FDEngine::BaseEngine::init()
{
    m_stateMachine.start();
    return true;
}

void FDEngine::BaseEngine::quit()
{
    m_stateMachine.stop();
}
int FDEngine::BaseEngine::run(int, char **) { return FDCore::AbstractApplication::ExitSuccess; }