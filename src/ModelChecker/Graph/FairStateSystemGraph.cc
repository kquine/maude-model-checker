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
FairStateSystemGraph<PL,FL>::FairStateSystemGraph(
		unique_ptr<PL>&& pl, unique_ptr<FL>&& fl, RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable):
			Super(initial,ptg,propTable), propLabel(move(pl)), fairLabel(move(fl)) {}

template <typename PL, typename FL> unique_ptr<FairSet>
FairStateSystemGraph<PL,FL>::makeFairSet(unsigned int stateNr, unsigned int) const
{
	return fairLabel->makeFairSet(this->seen[stateNr].get(), nullptr);
}

template <typename PL, typename FL> void
SystemGraphTraits<FairStateSystemGraph<PL,FL>>::updateStateLabel(DagNode* stateDag, State& s) const
{
	auto self = static_cast<const FairStateSystemGraph<PL,FL>*>(this);
	if (unique_ptr<PropSet> truePropIds = self->propLabel->updateStateLabel(stateDag, s))
		self->fairLabel->updateStateLabel(*truePropIds, s);	// compute all state fairness conditions
}

} /* namespace modelChecker */
