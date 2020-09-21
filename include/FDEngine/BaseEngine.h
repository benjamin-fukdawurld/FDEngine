#ifndef FDENGINE_BASEENGINE_H
#define FDENGINE_BASEENGINE_H

#include <FDCore/ApplicationManagement/AbstractApplication.h>
#include <FDEngine/StateMachine/AbstractEngineStateMachineController.h>
#include <FDEngine/StateMachine/EngineStateMachine.h>

namespace FDEngine
{
    class BaseEngine : public FDCore::AbstractApplication
    {
      public:
        FDEngine::StateMachine::EngineStateMachine m_stateMachine;

      public:
        BaseEngine(StateMachine::AbstractEngineStateMachineController *controller);
        ~BaseEngine() override = default;

        bool init() override;
        void quit() override;
        int run(int argc, char **argv) override;
    };
} // namespace FDEngine

#endif // FDENGINE_BASEENGINE_H