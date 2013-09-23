/*
 * FairStateEventSystemGraph.hh
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#ifndef FAIRSTATEEVENTSYSTEMGRAPH_HH_
#define FAIRSTATEEVENTSYSTEMGRAPH_HH_
#include "StateEventSystemGraph.hh"
#include "FairChecker/FairnessChecker.hh"

namespace modelChecker {

template <typename PL, typename FL>
class FairStateEventSystemGraph: public StateEventSystemGraph<PL>
{
public:
	FairStateEventSystemGraph(unique_ptr<PL>&& sepl, unique_ptr<FL>&& sefl, RewritingContext& initial, const ProofTermGenerator& ptg);

	unique_ptr<FairSet> makeFairSet(unsigned int stateNr, unsigned int transitionNr) const;

private:
	using Super =			StateEventSystemGraph<PL>;
	using PreState =		typename Super::State;
	using PreTransition =	typename Super::Transition;


	struct State: public PreState, public FL::StateLabel
	{
		State(RewritingContext& parent, DagNode* stateDag): PreState(parent,stateDag) {}
	};
	struct Transition: public PreTransition, public FL::EventLabel
	{
		Transition(unsigned int nextState, unsigned int transitionIndex): PreTransition(nextState, transitionIndex) {}
		bool operator<(const PreTransition& t) const override;
	};

	unique_ptr<PropSet> updateStateLabel(DagNode* stateDag, PreState& s) override;
	unique_ptr<PreState> createState(DagNode* stateDag) const override;

	unique_ptr<PropSet> updateTransitionLabel(RewriteTransitionState& rts, PreTransition& t, PreState& s) override;
	unique_ptr<PreTransition> createTransition(unsigned int nextState, unsigned int transitionIndex) const override;

	const unique_ptr<FL> stateEventFairLabel;
};

} /* namespace modelChecker */

#include "FairStateEventSystemGraph.cc"

#endif /* FAIRSTATEEVENTSYSTEMGRAPH_HH_ */
