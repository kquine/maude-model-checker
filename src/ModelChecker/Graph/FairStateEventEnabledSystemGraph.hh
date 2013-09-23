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
class FairStateEventEnabledSystemGraph: public StateEventEnabledSystemGraph<PL>
{
public:
	FairStateEventEnabledSystemGraph(unique_ptr<PL>&& sepl, unique_ptr<FL>&& sefl, unique_ptr<EnabledPropTransferer>&& enpc,
			RewritingContext& initial, const ProofTermGenerator& ptg);

	unique_ptr<FairSet> makeFairSet(unsigned int stateNr, unsigned int transitionNr) const;

private:
	using Super =			StateEventEnabledSystemGraph<PL>;
	using PreState =		typename Super::State;
	using PreTransition =	typename Super::Transition			;
	using LabelSet =		typename Super::LabelSet;

	struct State: public PreState, public FL::StateLabel {};

	struct Transition: public PreTransition, public FL::EventLabel
	{
		Transition(unsigned int nextState, unsigned int transitionIndex): PreTransition(nextState, transitionIndex) {}
		bool operator<(const PreTransition& t) const override;
	};

	LabelSet updateAllLabels(DagNode* stateDag, const vector<DagNode*>& proofDags, PreState& s, vector<unique_ptr<PreTransition> >& trs) override;
	unique_ptr<PreState> createState() const override;
	unique_ptr<PreTransition> createTransition(unsigned int nextState, unsigned int transitionIndex) const override;

	const unique_ptr<FL> stateEventFairLabel;
};

} /* namespace modelChecker */

#include "FairStateEventEnabledSystemGraph.cc"

#endif /* FAIRSTATEEVENTENABLEDSYSTEMGRAPH_HH_ */
