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
#include "FairStateSystemGraph.hh"

namespace modelChecker {

FairStateSystemGraph::FairStateSystemGraph(RewritingContext& initial, PropChecker& spc, const NatSet& formulaPropIds, FairnessChecker& fc, ProofTermGenerator& ptg):
		Super(initial, spc, ptg), fairC(fc), formulaPropIds(formulaPropIds) {}

unique_ptr<FairSet>
FairStateSystemGraph::makeFairSet(int stateNr, int transitionNr) const
{
	return static_cast<State&>(*Super::seen[stateNr]).fs->clone();
}

unique_ptr<PropSet>
FairStateSystemGraph::updateStateLabel(DagNode* stateDag, PreState& s)
{
	unique_ptr<PropSet> truePropIds = Super::updateStateLabel(stateDag,s);
	static_cast<State&>(s).fs = fairC.computeAllFairness(*truePropIds);		// compute all state fairness conditions
	s.label.intersect(formulaPropIds);										// keep only state formula props
	return truePropIds;
}

unique_ptr<FairStateSystemGraph::PreState>
FairStateSystemGraph::createState(DagNode* stateDag) const
{
	return unique_ptr<PreState>(new State(Super::initial, stateDag));
}

} /* namespace modelChecker */
