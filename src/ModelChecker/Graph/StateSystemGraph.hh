/*
 * StateSystemGraph.hh
 *
 *  Created on: Sep 11, 2013
 *      Author: kquine
 */

#ifndef STATESYSTEMGRAPH_HH_
#define STATESYSTEMGRAPH_HH_
#include "BaseSystemGraphNoEnabled.hh"
#include "PropSet/PropSet.hh"

namespace modelChecker {

template <typename PL>
class StateSystemGraph: public BaseSystemGraphNoEnabled<StateSystemGraph<PL>>
{
	friend class BaseSystemGraph<StateSystemGraph<PL>>;
	friend class BaseSystemGraphNoEnabled<StateSystemGraph<PL>>;

public:
	StateSystemGraph(unique_ptr<PL>&& statePropLabel, RewritingContext& initial, const ProofTermGenerator& ptg);
	virtual ~StateSystemGraph() = default;

	bool satisfiesStateFormula(Bdd formula, unsigned int stateNr) const;

protected:
	using Super = 		BaseSystemGraphNoEnabled<StateSystemGraph>;
	using State = 		typename BaseSystemGraphTraits<StateSystemGraph>::State;
	using ActiveState =	typename BaseSystemGraphTraits<StateSystemGraph>::ActiveState;
	using Transition =	typename BaseSystemGraphTraits<StateSystemGraph>::Transition;

	/* implements */ virtual unique_ptr<PropSet> updateStateLabel(DagNode* stateDag, State& s);
	/* implements */ virtual unique_ptr<State> createState(DagNode* stateDag) const;

private:
	/* implements */ bool insertTransition(unsigned int nextState, State& n);
	/* implements */ void closeTransition(State& ) {}

	const unique_ptr<PL> statePropLabel;
};

template <typename PL>
struct BaseSystemGraphTraits<StateSystemGraph<PL>>
{
	struct Transition
	{
		Transition(unsigned int nextState, const Rule* rule): nextState(nextState), rule(rule) {}
		DagNode* makeDag(RewritingContext&, DagNode*, const ProofTermGenerator& ptg) const;

		const unsigned int nextState;
		const Rule* rule;
	};

	struct ActiveState: public RewriteTransitionState
	{
		ActiveState(RewritingContext& parent, DagNode* stateDag): RewriteTransitionState(parent,stateDag) {}
		set<unsigned int> nextStateSet;
	};

	struct State: public PL::StateLabel
	{
		State(RewritingContext& parent, DagNode* stateDag): explore(new ActiveState(parent,stateDag)) {}
		virtual ~State() = default;

		vector<unique_ptr<Transition> > transitions;
		unique_ptr<ActiveState> explore;
	};
};

} /* namespace modelChecker */

#include "StateSystemGraph.cc"	// to separate the implementation for the template

#endif /* STATESYSTEMGRAPH_HH_ */
