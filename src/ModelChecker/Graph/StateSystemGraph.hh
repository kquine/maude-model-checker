/*
 * StateSystemGraph.hh
 *
 *  Created on: Aug 12, 2013
 *      Author: kquine
 */

#ifndef STATESYSTEMGRAPH_HH_
#define STATESYSTEMGRAPH_HH_
#include "BaseSystemGraphNoEnabled.hh"
#include "Formula/PropChecker.hh"
#include "Fairness/FairnessChecker.hh"

namespace modelChecker {

template <typename PropHandler>
class StateSystemGraph: public BaseSystemGraphNoEnabled<StateSystemGraph<PropHandler> >
{
	friend class BaseSystemGraph<StateSystemGraph<PropHandler> >;
	friend class BaseSystemGraphNoEnabled<StateSystemGraph<PropHandler> >;
	typedef BaseSystemGraphNoEnabled<StateSystemGraph<PropHandler> >	Super;
	typedef BaseSystemGraphTraits<StateSystemGraph<PropHandler> >		StateSystemGraphTraits;
public:
	StateSystemGraph(RewritingContext& initial, PropChecker& spc,ProofTermGenerator& ptg);
	virtual ~StateSystemGraph() {}

	bool satisfiesStateProp(int propId, int stateNr) const;

protected:
	typedef typename StateSystemGraphTraits::State						State;
	typedef typename StateSystemGraphTraits::ActiveState				ActiveState;
	typedef typename StateSystemGraphTraits::Transition					Transition;

	/* implements */ unique_ptr<PropSet> updateStateLabel(DagNode* stateDag, State& s);
	/* implements */ virtual unique_ptr<State> createState(DagNode* stateDag) const;

private:
	bool insertTransition(int nextState, State& n);

	PropHandler spHandler;
	PropChecker& statePC;
};

template <typename PropHandler>
struct BaseSystemGraphTraits<StateSystemGraph<PropHandler> >
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

	struct State: public PropHandler::Label
	{
		State(RewritingContext& parent, DagNode* stateDag): explore(new ActiveState(parent,stateDag)) {}
		virtual ~State() {}

		vector<unique_ptr<Transition> > transitions;
		unique_ptr<ActiveState> explore;
	};
};

} /* namespace modelChecker */

#include "StateSystemGraph.cc"

#endif /* STATESYSTEMGRAPH_HH_ */
