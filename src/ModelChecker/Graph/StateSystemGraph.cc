/*
 * StateSystemGraph.cc
 *
 *  Created on: Aug 12, 2013
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
#include "PropHandler.hh"
#include "StateSystemGraph.hh"

namespace modelChecker {

template <typename PH>
StateSystemGraph<PH>::StateSystemGraph(RewritingContext& initial, PropChecker& spc, ProofTermGenerator& ptg):
	Super(initial, ptg), statePC(spc) {}

template <typename PH>
unique_ptr<PropSet>
StateSystemGraph<PH>::updateStateLabel(DagNode* stateDag, State& s)
{
	unique_ptr<PropSet> truePropIds(statePC.computeCheckResult(stateDag)); 	// compute all state props
	spHandler.updateLabel(*truePropIds, s);								// store the formula state props
	return truePropIds;
}

template <typename PH>
unique_ptr<typename StateSystemGraph<PH>::State>
StateSystemGraph<PH>::createState(DagNode* stateDag) const
{
	return unique_ptr<State>(new State(Super::initial, stateDag));
}

template <typename PH> inline bool
StateSystemGraph<PH>::insertTransition(int nextState, State& n)
{
	if (n.explore->nextStateSet.insert(nextState).second)	// if a new transition identified
	{
		n.transitions.emplace_back(new Transition(nextState, n.explore->getRule()));
		return true;
	}
	return false;
}

template <typename PH> inline  DagNode*
BaseSystemGraphTraits<StateSystemGraph<PH> >::Transition::makeDag(RewritingContext&, DagNode*, ProofTermGenerator& ptg) const
{
	return ptg.makeProofDag(nullptr,*rule, nullptr);
}

} /* namespace modelChecker */
