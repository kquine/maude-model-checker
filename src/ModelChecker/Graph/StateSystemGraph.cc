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
StateSystemGraph<PL>::StateSystemGraph(unique_ptr<PL>&& spl, RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable):
		Super(initial,ptg,propTable), propLabel(move(spl)) {}

template <typename PL> inline bool
StateSystemGraph<PL>::satisfiesStateProp(unsigned int propId, unsigned int stateNr) const
{
	Assert(this->propTable.isStateProp(propId), "StateSystemGraph::satisfiesStateFormula: not a state prop");
	return propLabel->satisfiesStateProp(propId, *this->seen[stateNr]);
}

template <typename PL> unique_ptr<PropSet>
StateSystemGraph<PL>::updateStateLabel(DagNode* stateDag, State& s)
{
	return propLabel->updateStateLabel(stateDag, s);		// compute all state props and store the formula state props
}

template <typename PL>
unique_ptr<typename StateSystemGraph<PL>::State>
StateSystemGraph<PL>::createState(DagNode* stateDag) const
{
	return unique_ptr<State>(new State(this->initial, stateDag));
}

template <typename PL> bool
StateSystemGraph<PL>::insertTransition(unsigned int nextState, State& n)
{
	if (n.explore->nextStateSet.insert(nextState).second)	// if a new transition identified
	{
		n.transitions.emplace_back(new Transition(nextState, n.explore->getRule()));
		return true;
	}
	return false;
}

template <typename PL> DagNode*
BaseSystemGraphTraits<StateSystemGraph<PL>>::Transition::makeDag(RewritingContext& context, DagNode*, const ProofTermGenerator& ptg) const
{
	DagNode* d = ptg.makeProofDag(nullptr,*rule, nullptr);
	d->reduce(context);
	return d;
}

} /* namespace modelChecker */
