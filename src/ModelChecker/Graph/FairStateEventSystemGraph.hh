/*
 * FairStateEventSystemGraph.hh
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#ifndef FAIRSTATEEVENTSYSTEMGRAPH_HH_
#define FAIRSTATEEVENTSYSTEMGRAPH_HH_
#include "StateEventSystemGraph.hh"
#include "BaseSystemGraphOnce.hh"
#include "FairChecker/FairnessChecker.hh"

namespace modelChecker {

//
// a fair system graph with both state and event props, but all transitions of each state are evaluated at once for fairness conditions.
//
template <typename PL, typename FL>
class FairStateEventSystemGraph: public BaseSystemGraphOnce<FairStateEventSystemGraph<PL,FL>>
{
	friend class BaseSystemGraph<FairStateEventSystemGraph<PL,FL>>;
	friend class BaseSystemGraphOnce<FairStateEventSystemGraph<PL,FL>>;

public:
	FairStateEventSystemGraph(unique_ptr<PL>&& sepl, unique_ptr<FL>&& sefl, RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable);

	bool satisfiesStateProp(unsigned int propId, unsigned int stateNr) const;
	bool satisfiesEventProp(unsigned int propId, unsigned int stateNr, unsigned int transitionNr) const;

	unique_ptr<FairSet> makeFairSet(unsigned int stateNr, unsigned int transitionNr) const;

private:
	using Super =		BaseSystemGraphOnce<FairStateEventSystemGraph<PL,FL>>;
	using State =		typename BaseSystemGraphTraits<FairStateEventSystemGraph<PL,FL>>::State;
	using ActiveState =	typename BaseSystemGraphTraits<FairStateEventSystemGraph<PL,FL>>::ActiveState;
	using Transition =	typename BaseSystemGraphTraits<FairStateEventSystemGraph<PL,FL>>::Transition;

	void updateAllLabels(DagNode* stateDag, const vector<DagNode*>& proofDags, State& s, vector<unique_ptr<Transition> >& trs);
	unique_ptr<State> createState() const;
	unique_ptr<Transition> createTransition(unsigned int nextState, unsigned int transitionIndex) const;

	const unique_ptr<PL> propLabel;
	const unique_ptr<FL> fairLabel;
};

template <typename PL, typename FL>
struct BaseSystemGraphTraits<FairStateEventSystemGraph<PL,FL>>: public BaseSystemGraphTraits<StateEventSystemGraph<PL>>
{
	using ActiveState =			typename BaseSystemGraphTraits<StateEventSystemGraph<PL>>::ActiveState;
	using trans_ptr_compare =	typename BaseSystemGraphTraits<StateEventSystemGraph<PL>>::trans_ptr_compare;
	using PreTransition =		typename BaseSystemGraphTraits<StateEventSystemGraph<PL>>::Transition;

	struct Transition: public PreTransition, public FL::EventLabel
	{
		Transition(unsigned int nextState, unsigned int transitionIndex): PreTransition(nextState, transitionIndex) {}
		bool operator<(const PreTransition& t) const override
		{
			return PreTransition::operator<(t) || FL::EventLabel::operator<(static_cast<const Transition&>(t));
		}
	};

	struct State: public PL::StateLabel, public FL::StateLabel
	{
		vector<unique_ptr<Transition> > transitions;
		unsigned int nrVisited = 0;
	};
};

} /* namespace modelChecker */

#include "FairStateEventSystemGraph.cc"

#endif /* FAIRSTATEEVENTSYSTEMGRAPH_HH_ */
