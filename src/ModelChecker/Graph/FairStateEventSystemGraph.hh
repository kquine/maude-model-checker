/*
 * FairStateEventSystemGraph.hh
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#ifndef FAIRSTATEEVENTSYSTEMGRAPH_HH_
#define FAIRSTATEEVENTSYSTEMGRAPH_HH_
#include "StateEventSystemGraph.hh"
#include "StateEventEnabledSystemGraph.hh"
#include "FairChecker/FairnessChecker.hh"

namespace modelChecker {

//
// a fair system graph with both state and event props, but all transitions of each state are evaluated at once for fairness conditions.
//
template <typename PL, typename FL>
class FairStateEventSystemGraph: public BaseSystemGraphOnce<FairStateEventSystemGraph<PL,FL>>, private SystemGraphTraits<FairStateEventSystemGraph<PL,FL>>
{
	friend class BaseSystemGraph<FairStateEventSystemGraph>;
	friend class BaseSystemGraphOnce<FairStateEventSystemGraph>;
	friend class SystemGraphTraits<FairStateEventSystemGraph<PL,FL>>;
	using Super = BaseSystemGraphOnce<FairStateEventSystemGraph<PL,FL>>;

public:
	using typename SystemGraphTraits<FairStateEventSystemGraph<PL,FL>>::State;
	using typename SystemGraphTraits<FairStateEventSystemGraph<PL,FL>>::Transition;

	FairStateEventSystemGraph(unique_ptr<PL>&& sepl, unique_ptr<FL>&& sefl, RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable);

	unique_ptr<FairSet> makeFairSet(unsigned int stateNr, unsigned int transitionNr) const;

private:
	const unique_ptr<PL> propLabel;
	const unique_ptr<FL> fairLabel;
};

template <typename PL, typename FL>
class SystemGraphTraits<FairStateEventSystemGraph<PL,FL>>
{
public:
	struct State;
	struct Transition;

	bool satisfiesStateProp(unsigned int propId, const State& s) const;
	bool satisfiesEventProp(unsigned int propId, const Transition& t) const;
	void updateAllLabels(DagNode* stateDag, const vector<DagNode*>& proofDags, State& s, vector<unique_ptr<Transition> >& trs);
};

template <typename PL, typename FL>
struct SystemGraphTraits<FairStateEventSystemGraph<PL,FL>>::State: public BaseSystemGraphOnceTraits::State<Transition>, public PL::StateLabel, public FL::StateLabel {};

template <typename PL, typename FL>
struct SystemGraphTraits<FairStateEventSystemGraph<PL,FL>>::Transition: public BaseSystemGraphOnceTraits::Transition, public ProofDagGenerator, public PL::EventLabel, public FL::EventLabel
{
	Transition(unsigned int nextState, unsigned int transitionIndex): BaseSystemGraphOnceTraits::Transition{nextState}, ProofDagGenerator(transitionIndex) {}
	bool operator<(const Transition& t) const;
};

} /* namespace modelChecker */

#include "FairStateEventSystemGraph.cc"

#endif /* FAIRSTATEEVENTSYSTEMGRAPH_HH_ */
