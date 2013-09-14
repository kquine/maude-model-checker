/*
 * StateSystemGraph.cc
 *
 *  Created on: Sep 11, 2013
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
#include "StateSystemGraph.hh"

namespace modelChecker {

StateSystemGraph::StateSystemGraph(RewritingContext& initial, PropChecker& spc, ProofTermGenerator& ptg):
	Super(initial, ptg), statePC(spc) {}

bool
StateSystemGraph::satisfiesStateProp(int propId, int stateNr) const
{
	return Super::seen[stateNr]->label.contains(propId);
}

unique_ptr<PropSet>
StateSystemGraph::updateStateLabel(DagNode* stateDag, State& s)
{
	unique_ptr<PropSet> truePropIds(statePC.computeCheckResult(stateDag)); 	// compute all state props
	s.label.insert(truePropIds->getTruePropIds());							// store the formula state props
	return truePropIds;
}

unique_ptr<typename StateSystemGraph::State>
StateSystemGraph::createState(DagNode* stateDag) const
{
	return unique_ptr<State>(new State(Super::initial, stateDag));
}

bool
StateSystemGraph::insertTransition(int nextState, State& n)
{
	if (n.explore->nextStateSet.insert(nextState).second)	// if a new transition identified
	{
		n.transitions.emplace_back(new Transition(nextState, n.explore->getRule()));
		return true;
	}
	return false;
}

DagNode*
BaseSystemGraphTraits<StateSystemGraph>::Transition::makeDag(RewritingContext&, DagNode*, ProofTermGenerator& ptg) const
{
	return ptg.makeProofDag(nullptr,*rule, nullptr);
}

} /* namespace modelChecker */
