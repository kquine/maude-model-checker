/*
 * StateSystemGraph.hh
 *
 *  Created on: Sep 11, 2013
 *      Author: kquine
 */

#ifndef STATESYSTEMGRAPH_HH_
#define STATESYSTEMGRAPH_HH_
#include "GraphImpl/BaseSystemGraphNoEnabled.hh"
#include "GraphImpl/StateLabelGraph.hh"
#include "PropChecker/PropChecker.hh"

namespace modelChecker {

class StateSystemGraph;

template <>
struct BaseSystemGraphTraits<StateSystemGraph>
{
	struct Transition
	{
		Transition(int nextState, const Rule* rule): nextState(nextState), rule(rule) {}
		DagNode* makeDag(RewritingContext&, DagNode*, ProofTermGenerator& ptg) const;

		int nextState;
		const Rule* rule;
	};

	struct ActiveState: public RewriteTransitionState
	{
		ActiveState(RewritingContext& parent, DagNode* stateDag): RewriteTransitionState(parent,stateDag) {}
		set<int> nextStateSet;
	};

	struct State
	{
		State(RewritingContext& parent, DagNode* stateDag): explore(new ActiveState(parent,stateDag)) {}
		virtual ~State() {}

		NatSet label;
		vector<unique_ptr<Transition> > transitions;
		unique_ptr<ActiveState> explore;
	};
};

class StateSystemGraph: public BaseSystemGraphNoEnabled<StateSystemGraph>, public StateLabelGraph
{
	friend class BaseSystemGraph<StateSystemGraph>;
	friend class BaseSystemGraphNoEnabled<StateSystemGraph>;
	typedef BaseSystemGraphNoEnabled<StateSystemGraph>			Super;
	typedef BaseSystemGraphTraits<StateSystemGraph>				StateSystemGraphTraits;
public:
	StateSystemGraph(RewritingContext& initial, PropChecker& spc, ProofTermGenerator& ptg);
	virtual ~StateSystemGraph() {}

protected:
	typedef typename StateSystemGraphTraits::State						State;
	typedef typename StateSystemGraphTraits::ActiveState				ActiveState;
	typedef typename StateSystemGraphTraits::Transition					Transition;

	/* implements */ virtual unique_ptr<PropSet> updateStateLabel(DagNode* stateDag, State& s);
	/* implements */ virtual unique_ptr<State> createState(DagNode* stateDag) const;

	/* implements */ bool insertTransition(int nextState, State& n);
	/* implements */ void closeTransition(const State& ) { /* Do nothing */ }

private:
	bool satisfiesStateProp(int propId, int stateNr) const override;

	PropChecker& statePC;
};



} /* namespace modelChecker */
#endif /* STATESYSTEMGRAPH_HH_ */
