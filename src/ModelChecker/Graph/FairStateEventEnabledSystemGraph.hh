/*
 * FairStateEventEnabledSystemGraph.hh
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#ifndef FAIRSTATEEVENTENABLEDSYSTEMGRAPH_HH_
#define FAIRSTATEEVENTENABLEDSYSTEMGRAPH_HH_
#include "StateEventEnabledSystemGraph.hh"

namespace modelChecker {

template <typename PL, typename FL>
class FairStateEventEnabledSystemGraph: public BaseSystemGraphOnce<FairStateEventEnabledSystemGraph<PL,FL>>, private SystemGraphTraits<FairStateEventEnabledSystemGraph<PL,FL>>
{
	friend class BaseSystemGraph<FairStateEventEnabledSystemGraph<PL,FL>>;
	friend class BaseSystemGraphOnce<FairStateEventEnabledSystemGraph<PL,FL>>;
	friend class SystemGraphTraits<FairStateEventEnabledSystemGraph<PL,FL>>;
	using Super = BaseSystemGraphOnce<FairStateEventEnabledSystemGraph<PL,FL>>;

public:
	using typename SystemGraphTraits<FairStateEventEnabledSystemGraph<PL,FL>>::State;
	using typename SystemGraphTraits<FairStateEventEnabledSystemGraph<PL,FL>>::Transition;

	FairStateEventEnabledSystemGraph(unique_ptr<PL>&& sepl, unique_ptr<FL>&& sefl, unique_ptr<EnabledPropHandler>&& enpc,
			RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable);

	unique_ptr<FairSet> makeFairSet(unsigned int stateNr, unsigned int transitionNr) const;

private:
	const unique_ptr<PL> propLabel;
	const unique_ptr<FL> fairLabel;
	const unique_ptr<EnabledPropHandler> enabledHandler;
};

template <typename PL, typename FL>
class SystemGraphTraits<FairStateEventEnabledSystemGraph<PL,FL>>: public SystemGraphTraits<StateEventEnabledSystemGraph<PL>>
{
public:
	struct State;
	struct Transition;

	bool satisfiesStateProp(unsigned int propId, const State& s) const;
	bool satisfiesEventProp(unsigned int propId, const Transition& t) const;
	void updateAllLabels(DagNode* stateDag, const vector<DagNode*>& proofDags, State& s, vector<unique_ptr<Transition>>& trs);

private:
	using PreState = 		typename SystemGraphTraits<StateEventEnabledSystemGraph<PL>>::State;
	using PreTransition =	typename SystemGraphTraits<StateEventEnabledSystemGraph<PL>>::Transition;
};

template <typename PL, typename FL>
struct SystemGraphTraits<FairStateEventEnabledSystemGraph<PL,FL>>::State: public PreState, public FL::StateLabel {};

template <typename PL, typename FL>
struct SystemGraphTraits<FairStateEventEnabledSystemGraph<PL,FL>>::Transition: public PreTransition, public FL::EventLabel
{
	Transition(unsigned int nextState, unsigned int transitionIndex): PreTransition(nextState, transitionIndex) {}
	bool operator<(const Transition& t) const;
};



} /* namespace modelChecker */

#include "FairStateEventEnabledSystemGraph.cc"

#endif /* FAIRSTATEEVENTENABLEDSYSTEMGRAPH_HH_ */
