#ifndef FDENGINE_ENGINESTATEMACHINETRANSITIONTABLE_H
#define FDENGINE_ENGINESTATEMACHINETRANSITIONTABLE_H

#include <FDEngine/StateMachine/EngineStateMachineActions.h>
#include <FDEngine/StateMachine/EngineStateMachineEvents.h>
#include <FDEngine/StateMachine/EngineStateMachineGuards.h>
#include <FDEngine/StateMachine/EngineStateMachineStates.h>
#include <boost/mpl/vector.hpp>
#include <boost/msm/front/euml/operator.hpp>
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/state_machine_def.hpp>

namespace FDEngine
{
    namespace StateMachine
    {
        namespace bmf = boost::msm::front;
        namespace mpl = boost::mpl;
        namespace uml = bmf::euml;
        struct EngineTransitionTable :
            mpl::vector<
              bmf::
                Row<NoErrorState, ErrorEvent, ErrorState, ErrorAction, uml::Not_<FatalErrorGuard>>,

              bmf::Row<ErrorState,
                       ErrorHandledEvent,
                       NoErrorState,
                       NoErrorAction,
                       uml::Not_<FatalErrorGuard>>,

              bmf::
                Row<NoErrorState, ErrorEvent, FatalErrorState, FatalErrorAction, FatalErrorGuard>,

              bmf::Row<ErrorState,
                       ErrorHandledEvent,
                       FatalErrorState,
                       FatalErrorAction,
                       FatalErrorGuard>,

              bmf::Row<LaunchState, InitPendingEvent, InitState, InitAction, bmf::none>,

              bmf::Row<InitState, InitDoneEvent, IdleState, IdleAction, bmf::none>,

              bmf::Row<IdleState, QuitEvent, QuitState, QuitAction, bmf::none>,

              bmf::Row<IdleState, ContinueEvent, NotUpToDateState, NotUpToDateAction, bmf::none>,

              bmf::Row<NotUpToDateState, UpdatePendingEvent, UpdatedState, UpdateAction, bmf::none>,

              bmf::
                Row<UpdatedState, UpdateDoneEvent, NotRenderedState, NotRenderedAction, bmf::none>,

              bmf::
                Row<NotRenderedState, RenderPendingEvent, RenderedState, RenderedAction, bmf::none>,

              bmf::Row<RenderedState, RenderDoneEvent, IdleState, IdleAction, bmf::none>>
        {
        };
    } // namespace StateMachine
} // namespace FDEngine

#endif // FDENGINE_ENGINESTATEMACHINETRANSITIONTABLE_H