/*
 * StateTransitionGraph.hh
 *
 *  Created on: Jun 23, 2011
 *      Author: kquine
 */

#ifndef STATETRANSITIONGRAPH_HH_
#define STATETRANSITIONGRAPH_HH_
#include <set>
#include "hashConsSet.hh"
#include "Util/PtrVector.hh"
#include "RewriteTransitionState.hh"
#include "StateDagContainer.hh"
#include "SystemGraph.hh"

namespace modelChecker {

/*
 * TransitionHandler:	how to handle transition information (e.g., rule, proof-term, etc.)
 * StatePropHandler:	state propositions, including (parameterized) fairness state props
 * EventPropHandler:	event propositions, including (parameterized) fairness event props
 */
template <typename TransitionHandler, typename StatePropHandler, typename EventPropHandler>
class SystemGraphImpl: private StateDagContainer, public SystemGraph
{
	NO_COPYING(SystemGraphImpl);
	typedef typename StatePropHandler::PropSet		StatePropSet;
	typedef typename EventPropHandler::PropSet		EventPropSet;
public:
	SystemGraphImpl(RewritingContext* initial, TransitionHandler* th, StatePropHandler* sph, EventPropHandler* eph);

	int getNrStates() const;
	int getNrTransitions(int stateNr) const;
	int getNextState(int stateNr, int index);

	DagNode* getStateDag(int stateNr) const;
	DagNode* getTransitionDag(int stateNr, int index) const;

	bool satisfiesStateProp(int propId, int stateNr) const;
	bool satisfiesEventProp(int propId, int stateNr, int transitionNr) const;

private:
	struct State;
	struct Transition;
	struct ActiveState;

	int insertState(DagNode* stateDag, ActiveState* previous);
	bool insertTransition(State* s, int nextState);

	// handlers will be destroyed when this graph is deleted...
	auto_ptr<TransitionHandler> th;
	auto_ptr<StatePropHandler> sph;		// formula state props + fairness state props (including parameterized)
	auto_ptr<EventPropHandler> eph;		// formula event props + fairness event props (including parameterized)

	PtrVector<State> seen;	// state information
	RewritingContext* initial;
};

/*
 * State information
 */
template <typename TH,typename SPH,typename EPH>
struct SystemGraphImpl<TH,SPH,EPH>::State: public StatePropSet
{
	State(RewritingContext* parent, DagNode* stateDag);
	PtrVector<Transition> transitions;
	auto_ptr<ActiveState> explore;
};

/*
 * Transition information
 */
template <typename TH,typename SPH,typename EPH>
struct SystemGraphImpl<TH,SPH,EPH>::Transition: public TH::TransitionData, public EventPropSet
{
	int nextState;

	Transition(int nextState): nextState(nextState) {}
	bool operator<(const Transition& t) const;
};

/*
 * The information that is used only during graph search.
 */
template <typename TH,typename SPH,typename EPH>
struct SystemGraphImpl<TH,SPH,EPH>::ActiveState: public RewriteTransitionState, public TH::StateData
{
	typedef set<Transition*, bool (*)(const Transition*,const Transition*)> TransitionPtrSet;
	auto_ptr<TransitionPtrSet> transitionPtrSet;

	ActiveState(RewritingContext* parent, DagNode* stateDag);
	static bool trans_ptr_less(const Transition* a, const Transition* b);
};

}

#include "SystemGraphImpl.cc"

#endif /* STATETRANSITIONGRAPH_HH_ */
