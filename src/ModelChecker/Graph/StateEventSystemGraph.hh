/*
 * StateEventSystemGraph.hh
 *
 *  Created on: Aug 13, 2013
 *      Author: kquine
 */

#ifndef STATEEVENTSYSTEMGRAPH_HH_
#define STATEEVENTSYSTEMGRAPH_HH_
#include "BaseSystemGraphNoEnabled.hh"

namespace modelChecker {

/*
 * an ordinary state/event-based RW system graph.
 * every state proposition should be defined by equations (no special treatment for enabled props here)
 */
template <typename SPH, typename EPH>
class StateEventSystemGraph: public BaseSystemGraphNoEnabled<StateEventSystemGraph<SPH,EPH> >
{
	friend class BaseSystemGraph<StateEventSystemGraph<SPH,EPH> >;
	friend class BaseSystemGraphNoEnabled<StateEventSystemGraph<SPH,EPH> >;
	typedef BaseSystemGraphNoEnabled<StateEventSystemGraph<SPH,EPH> >	Super;
	typedef BaseSystemGraphTraits<StateEventSystemGraph<SPH,EPH> >		StateEventSystemGraphTraits;
public:
	StateEventSystemGraph(RewritingContext& initial, PropChecker& spc, PropChecker& epc, ProofTermGenerator& ptg);
	virtual ~StateEventSystemGraph() {}

	bool satisfiesStateProp(int propId, int stateNr) const;
	bool satisfiesEventProp(int propId, int stateNr, int transitionNr) const;

protected:
	typedef typename StateEventSystemGraphTraits::State					State;
	typedef typename StateEventSystemGraphTraits::ActiveState			ActiveState;
	typedef typename StateEventSystemGraphTraits::Transition			Transition;

	/* implements */ virtual unique_ptr<PropSet> updateStateLabel(DagNode* stateDag, State& s);
	/* implements */ virtual unique_ptr<State> createState(DagNode* stateDag) const;

	virtual unique_ptr<PropSet> updateTransitionLabel(RewriteTransitionState& rts, Transition& t, State& s);
	virtual unique_ptr<Transition> createTransition(int nextState, int transitionIndex) const;

	SPH spHandler;

private:
	bool insertTransition(int nextState, State& n);

	EPH epHandler;
	PropChecker& statePC;
	PropChecker& eventPC;
};

template <typename SPH, typename EPH>
struct BaseSystemGraphTraits<StateEventSystemGraph<SPH,EPH> >
{
	struct Transition: public EPH::Label
	{
		Transition(int nextState, int transitionIndex): nextState(nextState), transitionIndex(transitionIndex) {}
		virtual ~Transition() {}

		virtual bool operator<(const Transition& t) const;
		DagNode* makeDag(RewritingContext& parent, DagNode* stateDag, ProofTermGenerator& ptg) const;

		int nextState;
		int transitionIndex;	// to construct a proofterm
	};

	struct trans_ptr_compare
	{
		bool operator() (const Transition* a, const Transition* b) const;
	};

	struct ActiveState: public RewriteTransitionState
	{
		ActiveState(RewritingContext& parent, DagNode* stateDag): RewriteTransitionState(parent,stateDag), transitionCount(-1) {}
		set<Transition*, trans_ptr_compare> transitionPtrSet;	// to distinguish equivalent transitions
		int transitionCount;									// to recover the corresponding proofterm
	};

	struct State: public SPH::Label
	{
		State(RewritingContext& parent, DagNode* stateDag): explore(new ActiveState(parent,stateDag)) {}
		virtual ~State() {}

		vector<unique_ptr<Transition> > transitions;
		unique_ptr<ActiveState> explore;
	};
};

} /* namespace modelChecker */

#include "StateEventSystemGraph.cc"	// to separate the implementation for the template

#endif /* STATEEVENTSYSTEMGRAPH_HH_ */
