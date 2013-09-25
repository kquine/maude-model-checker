/*
 * FairStateSystemGraph.cc
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

// interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"

// ltlr definitions
#include "FairStateSystemGraph.hh"

namespace modelChecker {

template <typename PL, typename FL>
FairStateSystemGraph<PL,FL>::FairStateSystemGraph(unique_ptr<PL>&& spl, unique_ptr<FL>&& sfl,
		RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable):
		Super(move(spl),initial,ptg,propTable), fairLabel(move(sfl)) {}

template <typename PL, typename FL> unique_ptr<FairSet>
FairStateSystemGraph<PL,FL>::makeFairSet(unsigned int stateNr, unsigned int) const
{
	return fairLabel->makeFairSet(static_cast<State*>(this->seen[stateNr].get()), nullptr);
}

template <typename PL, typename FL> unique_ptr<PropSet>
FairStateSystemGraph<PL,FL>::updateStateLabel(DagNode* stateDag, PreState& s)
{
	unique_ptr<PropSet> truePropIds = Super::updateStateLabel(stateDag,s);
	if (truePropIds)
		fairLabel->updateStateLabel(*truePropIds, static_cast<State&>(s));		// compute all state fairness conditions
	return truePropIds;
}

template <typename PL, typename FL>
unique_ptr<typename FairStateSystemGraph<PL,FL>::PreState>
FairStateSystemGraph<PL,FL>::createState(DagNode* stateDag) const
{
	return unique_ptr<PreState>(new State(this->initial, stateDag));
}

} /* namespace modelChecker */
