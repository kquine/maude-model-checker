/*
 * FairStateSystemGraph.hh
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#ifndef FAIRSTATESYSTEMGRAPH_HH_
#define FAIRSTATESYSTEMGRAPH_HH_
#include "StateSystemGraph.hh"
#include "FairChecker/FairnessChecker.hh"

namespace modelChecker {

template <typename PL, typename FL>
class FairStateSystemGraph: public BaseSystemGraphIter<FairStateSystemGraph<PL,FL>>, private SystemGraphTraits<FairStateSystemGraph<PL,FL>>
{
	friend class BaseSystemGraph<FairStateSystemGraph>;
	friend class BaseSystemGraphIter<FairStateSystemGraph>;
	friend class SystemGraphTraits<FairStateSystemGraph<PL,FL>>;
	using Super = BaseSystemGraphIter<FairStateSystemGraph<PL,FL>>;

public:
	using typename SystemGraphTraits<FairStateSystemGraph<PL,FL>>::State;
	using typename SystemGraphTraits<FairStateSystemGraph<PL,FL>>::Transition;

	FairStateSystemGraph(unique_ptr<PL>&& spl, unique_ptr<FL>&& sfl, RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable);

	unique_ptr<FairSet> makeFairSet(unsigned int stateNr, unsigned int transitionNr) const;

private:
	const unique_ptr<PL> propLabel;
	const unique_ptr<FL> fairLabel;
};


template <typename PL, typename FL>
class SystemGraphTraits<FairStateSystemGraph<PL,FL>>: public SystemGraphTraits<StateSystemGraph<PL>>
{
public:
	struct State;
	using typename SystemGraphTraits<StateSystemGraph<PL>>::Transition;
	using typename SystemGraphTraits<StateSystemGraph<PL>>::ActiveState;

	void updateStateLabel(DagNode* stateDag, State& s) const;

private:
	using PreState = typename SystemGraphTraits<StateSystemGraph<PL>>::State;
};

template <typename PL, typename FL>
struct SystemGraphTraits<FairStateSystemGraph<PL,FL>>::State: public PreState, public FL::StateLabel {};

} /* namespace modelChecker */

#include "FairStateSystemGraph.cc"	// to separate the implementation for the template

#endif /* FAIRSTATESYSTEMGRAPH_HH_ */
