/*
 * StateSystemGraph.hh
 *
 *  Created on: Aug 12, 2013
 *      Author: kquine
 */

#ifndef STATESYSTEMGRAPH_HH_
#define STATESYSTEMGRAPH_HH_
#include "BaseSystemGraph.hh"

namespace modelChecker {

template <typename StatePropHandler>
class StateSystemGraph: public BaseSystemGraph<StateSystemGraph<StatePropHandler> >
{
	friend class BaseSystemGraph<StateSystemGraph<StatePropHandler> >;
	typedef BaseSystemGraph<StateSystemGraph<StatePropHandler> >			Super;
	typedef BaseSystemGraphTraits<StateSystemGraph<StatePropHandler> >		StateSystemGraphTraits;
	typedef typename StateSystemGraphTraits::StateLabel						StateLabel;
	typedef typename StateSystemGraphTraits::State							State;
	typedef typename StateSystemGraphTraits::ActiveState					ActiveState;
	typedef typename StateSystemGraphTraits::Transition						Transition;
public:
	StateSystemGraph(RewritingContext* initial, const StatePropHandler& sph, ProofTermGenerator& ptg);

private:
	DagNode* makeTransitionDag(int stateNr, int transitionNr) const;
	int computeNextState(int stateNr, int index);
	int insertState(DagNode* stateDag);

	const StatePropHandler& spHandler;
};

template <typename StatePropHandler>
struct BaseSystemGraphTraits<StateSystemGraph<StatePropHandler> >
{
	typedef typename StatePropHandler::Label	StateLabel;

	struct Transition
	{
		Transition(int nextState, const Rule* rule): nextState(nextState), rule(rule) {}
		int nextState;
		const Rule* rule;
	};

	struct ActiveState: public RewriteTransitionState
	{
		ActiveState(RewritingContext* parent, DagNode* stateDag): RewriteTransitionState(parent,stateDag) {}
		set<int> nextStateSet;
	};

	struct State: public StateLabel
	{
		State(RewritingContext* parent, DagNode* stateDag): explore(new ActiveState(parent,stateDag)) {}
		PtrVector<Transition> transitions;
		auto_ptr<ActiveState> explore;
	};
};

} /* namespace modelChecker */

#include "StateSystemGraph.cc"	// to separate the implementation for the template

#endif /* STATESYSTEMGRAPH_HH_ */
