/*
 * StateEventEnabledSystemGraph.hh
 *
 *  Created on: Aug 14, 2013
 *      Author: kquine
 */

#ifndef STATEEVENTENABLEDSYSTEMGRAPH_HH_
#define STATEEVENTENABLEDSYSTEMGRAPH_HH_
#include "BaseSystemGraph.hh"

namespace modelChecker {

/*
 * an extended state/event-based RW system graph.
 * in order to handle the enabled props operationally,   all the transitions for a state are computed at the same time.
 */
template <typename SEELabelHandler>
class StateEventEnabledSystemGraph: public BaseSystemGraph<SEELabelHandler,StateEventEnabledSystemGraph<SEELabelHandler> >
{
	friend class BaseSystemGraph<SEELabelHandler,StateEventEnabledSystemGraph<SEELabelHandler> >;
	typedef BaseSystemGraph<SEELabelHandler,StateEventEnabledSystemGraph<SEELabelHandler> >				BaseSystemGraph;
	typedef typename BaseSystemGraphTraits<StateEventEnabledSystemGraph<SEELabelHandler> >::StateLabel	StateLabel;
	typedef typename BaseSystemGraphTraits<StateEventEnabledSystemGraph<SEELabelHandler> >::EventLabel	EventLabel;
	typedef typename BaseSystemGraphTraits<StateEventEnabledSystemGraph<SEELabelHandler> >::State		State;
	typedef typename BaseSystemGraphTraits<StateEventEnabledSystemGraph<SEELabelHandler> >::Transition	Transition;

public:
	StateEventEnabledSystemGraph(RewritingContext* initial, SEELabelHandler& selh, ProofTermGenerator& ptg);

private:
	int insertState(DagNode* stateDag);
	int computeNextState(int stateNr, int index);
	bool insertTransition(int nextState, State* n) { CantHappen("StateEventEnabledSystemGraph::StateEventEnabledSystemGraph"); };

	DagNode* computeTransitionDag(const RewriteTransitionState& rts, ProtectedDagNodeSet& tdags) const;
	State* createState(const Vector<DagNode*>& proofDags, PtrVector<Transition>& transitions, DagNode* stateDag) const;
};

} /* namespace modelChecker */

#include "StateEventEnabledSystemGraph.cc"	// to separate the implementation for the template

#endif /* STATEEVENTENABLEDSYSTEMGRAPH_HH_ */
