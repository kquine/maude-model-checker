/*
 * StateEventEnabledSystemGraph.hh
 *
 *  Created on: Aug 14, 2013
 *      Author: kquine
 */

#ifndef STATEEVENTENABLEDSYSTEMGRAPH_HH_
#define STATEEVENTENABLEDSYSTEMGRAPH_HH_
#include "BaseSystemGraph.hh"
#include "PropChecker/EnabledPropTransferer.hh"

namespace modelChecker {

template <typename PL>
class StateEventEnabledSystemGraph: public BaseSystemGraph<StateEventEnabledSystemGraph<PL> >
{
	friend class BaseSystemGraph<StateEventEnabledSystemGraph<PL> >;

public:
	StateEventEnabledSystemGraph(unique_ptr<PL>&& sepl, unique_ptr<EnabledPropTransferer>&& enpc, RewritingContext& initial, const ProofTermGenerator& ptg);

	bool satisfiesStateFormula(Bdd formula, unsigned int stateNr) const;
	pair<bool,Bdd> satisfiesPartialStateFormula(Bdd formula, unsigned int stateNr) const;
	bool satisfiesStateEventFormula(Bdd formula, unsigned int stateNr, unsigned int transitionNr) const;

protected:
	using Super =		BaseSystemGraph<StateEventEnabledSystemGraph<PL>>;
	using State =		typename BaseSystemGraphTraits<StateEventEnabledSystemGraph<PL> >::State;
	using ActiveState =	typename BaseSystemGraphTraits<StateEventEnabledSystemGraph<PL> >::ActiveState;
	using Transition =	typename BaseSystemGraphTraits<StateEventEnabledSystemGraph<PL> >::Transition;
	using LabelSet =	pair<unique_ptr<PropSet>,vector<unique_ptr<PropSet>>>;

	virtual LabelSet updateAllLabels(DagNode* stateDag, const vector<DagNode*>& proofDags, State& s, vector<unique_ptr<Transition> >& trs);
	virtual unique_ptr<State> createState() const;
	virtual unique_ptr<Transition> createTransition(unsigned int nextState, unsigned int transitionIndex) const;

	const unique_ptr<EnabledPropTransferer> enabledPropHandler;
private:
	/* implements */ int computeNextState(unsigned int stateNr, unsigned int index);
	/* implements */ unsigned int insertState(DagNode* stateDag);

	const unique_ptr<PL> stateEventPropLabel;
};

template <typename PL>
struct BaseSystemGraphTraits<StateEventEnabledSystemGraph<PL>>: public BaseSystemGraphTraits<StateEventSystemGraph<PL>>
{

	using ActiveState =			typename BaseSystemGraphTraits<StateEventSystemGraph<PL>>::ActiveState;
	using Transition =			typename BaseSystemGraphTraits<StateEventSystemGraph<PL>>::Transition;
	using trans_ptr_compare =	typename BaseSystemGraphTraits<StateEventSystemGraph<PL>>::trans_ptr_compare;

	struct State: public PL::StateLabel
	{
		virtual ~State() = default;
		vector<unique_ptr<Transition> > transitions;
	};
};

} /* namespace modelChecker */

#include "StateEventEnabledSystemGraph.cc"	// to separate the implementation for the template

#endif /* STATEEVENTENABLEDSYSTEMGRAPH_HH_ */
