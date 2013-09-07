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
#include "freeTheory.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

// interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"

// ltlr definitions

#include "FairHandler.hh"
#include "FairStateSystemGraph.hh"

namespace modelChecker {

template <typename PH, typename FH>
FairStateSystemGraph<PH,FH>::FairStateSystemGraph(RewritingContext& initial, PropChecker& spc, FairnessChecker& fc, ProofTermGenerator& ptg):
		Super(initial, spc, ptg), fairC(fc) {}

template <typename PH, typename FH>
unique_ptr<PropSet>
FairStateSystemGraph<PH,FH>::updateStateLabel(DagNode* stateDag, PreState& s)
{
	unique_ptr<PropSet> truePropIds = Super::updateStateLabel(stateDag,s);
	unique_ptr<FairSet> currFS(fairC.computeAllFairness(*truePropIds));	// compute all state fairness conditions
	fHandler.updateLabel(*currFS, static_cast<State&>(s));
	return truePropIds;
}

template <typename PH, typename FH>
unique_ptr<typename FairStateSystemGraph<PH,FH>::PreState>
FairStateSystemGraph<PH,FH>::createState(DagNode* stateDag) const
{
	return unique_ptr<PreState>(new State(Super::initial, stateDag));
}

} /* namespace modelChecker */
