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

template <typename PL>
StateSystemGraph<PL>::StateSystemGraph(unique_ptr<PL>&& pl, RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable):
	Super(initial,ptg,propTable), propLabel(move(pl)) {}

template <typename PL> bool
SystemGraphTraits<StateSystemGraph<PL>>::satisfiesStateProp(unsigned int propId, const State& s) const
{
	return static_cast<const StateSystemGraph<PL>*>(this)->propLabel->satisfiesStateProp(propId, s);
}

template <typename PL> bool
SystemGraphTraits<StateSystemGraph<PL>>::satisfiesEventProp(unsigned int, const Transition&) const
{
	throw logic_error("StateSystemGraph::satisfiesEventProp.");
}

template <typename PL> unique_ptr<PropSet>
SystemGraphTraits<StateSystemGraph<PL>>::updateStateLabel(DagNode* stateDag, State& s) const
{
	return static_cast<const StateSystemGraph<PL>*>(this)->propLabel->updateStateLabel(stateDag, s);
}

template <typename PL> bool
SystemGraphTraits<StateSystemGraph<PL>>::insertTransition(unsigned int nextState, State& n) const
{
	if (static_cast<ActiveState&>(*n.explore).nextStateSet.insert(nextState).second)	// if a new transition identified
	{
		n.transitions.emplace_back(new Transition(nextState, n.explore->getRule()));
		return true;
	}
	return false;
}

} /* namespace modelChecker */
