/*
 * StateEventEnabledSystemGraph.hh
 *
 *  Created on: Aug 14, 2013
 *      Author: kquine
 */

#ifndef STATEEVENTENABLEDSYSTEMGRAPH_HH_
#define STATEEVENTENABLEDSYSTEMGRAPH_HH_
#include "BaseSystemGraphOnce.hh"
#include "PropChecker/EnabledPropHandler.hh"

namespace modelChecker {

/*
 * a state/event-based RW system graph with "operational" enabled propositions.
 */
template <typename PL>
class StateEventEnabledSystemGraph: public BaseSystemGraphOnce<StateEventEnabledSystemGraph<PL>>
{
	friend class BaseSystemGraph<StateEventEnabledSystemGraph<PL>>;
	friend class BaseSystemGraphOnce<StateEventEnabledSystemGraph<PL>>;

public:
	StateEventEnabledSystemGraph(unique_ptr<PL>&& sepl, unique_ptr<EnabledPropHandler>&& enph,
			RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable);

	bool satisfiesStateProp(unsigned int propId, unsigned int stateNr) const;
	bool satisfiesEventProp(unsigned int propId, unsigned int stateNr, unsigned int transitionNr) const;

protected:
	using Super =		BaseSystemGraphOnce<StateEventEnabledSystemGraph<PL>>;
	using State =		typename BaseSystemGraphTraits<StateEventEnabledSystemGraph<PL> >::State;
	using ActiveState =	typename BaseSystemGraphTraits<StateEventEnabledSystemGraph<PL> >::ActiveState;
	using Transition =	typename BaseSystemGraphTraits<StateEventEnabledSystemGraph<PL> >::Transition;
	using LabelSet =	pair<unique_ptr<PropSet>,vector<unique_ptr<PropSet>>>;

	virtual LabelSet updateAllLabels(DagNode* stateDag, const vector<DagNode*>& proofDags, State& s, vector<unique_ptr<Transition> >& trs);
	virtual unique_ptr<State> createState() const;
	virtual unique_ptr<Transition> createTransition(unsigned int nextState, unsigned int transitionIndex) const;

	unique_ptr<EnabledPropHandler> enabledHandler;

private:
	const unique_ptr<PL> propLabel;
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
		unsigned int nrVisited = 0;
	};
};

} /* namespace modelChecker */

#include "StateEventEnabledSystemGraph.cc"	// to separate the implementation for the template

#endif /* STATEEVENTENABLEDSYSTEMGRAPH_HH_ */
