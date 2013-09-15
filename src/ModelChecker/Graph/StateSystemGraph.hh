/*
 * StateSystemGraph.hh
 *
 *  Created on: Sep 11, 2013
 *      Author: kquine
 */

#ifndef STATESYSTEMGRAPH_HH_
#define STATESYSTEMGRAPH_HH_
#include "GraphImpl/BaseSystemGraphNoEnabled.hh"
#include "PropChecker/PropChecker.hh"

namespace modelChecker {

class StateSystemGraph;

template <>
struct BaseSystemGraphTraits<StateSystemGraph>
{
	struct Transition
	{
		Transition(int nextState, const Rule* rule): nextState(nextState), rule(rule) {}
		DagNode* makeDag(RewritingContext&, DagNode*, const ProofTermGenerator& ptg) const;

		const int nextState;
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

class StateSystemGraph: public BaseSystemGraphNoEnabled<StateSystemGraph>
{
	friend class BaseSystemGraph<StateSystemGraph>;
	friend class BaseSystemGraphNoEnabled<StateSystemGraph>;
public:
	StateSystemGraph(RewritingContext& initial, PropChecker& spc, const ProofTermGenerator& ptg);
	virtual ~StateSystemGraph() {}

	bool satisfiesStateFormula(Bdd formula, int stateNr) const;

protected:
	using Super = 		BaseSystemGraphNoEnabled<StateSystemGraph>;
	using State = 		typename BaseSystemGraphTraits<StateSystemGraph>::State;
	using ActiveState =	typename BaseSystemGraphTraits<StateSystemGraph>::ActiveState;
	using Transition =	typename BaseSystemGraphTraits<StateSystemGraph>::Transition;

	/* implements */ virtual unique_ptr<PropSet> updateStateLabel(DagNode* stateDag, State& s);
	/* implements */ virtual unique_ptr<State> createState(DagNode* stateDag) const;

	/* implements */ bool insertTransition(int nextState, State& n);
	/* implements */ void closeTransition(const State& ) { /* Do nothing */ }

private:
	PropChecker& statePC;
};



} /* namespace modelChecker */
#endif /* STATESYSTEMGRAPH_HH_ */
