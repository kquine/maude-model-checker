/*
 * StateMetaGraph.cc
 *
 *  Created on: Nov 15, 2011
 *      Author: kquine
 */
//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//      free theory class definitions
#include "freeDagNode.hh"

// core class definitions
#include "rewritingContext.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"

// lmc definitions
#include "StateMetaGraph.hh"


namespace modelChecker {


StateMetaGraph::StateMetaGraph(unique_ptr<StatePropLabel>&& spl, RewritingContext& initial, const ProofTermGenerator& ptg,
			const PropositionTable& propTable, Symbol* stateSymbol):
		BaseSystemGraphIter<StateMetaGraph>(initial,ptg,propTable),
		SystemGraphTraits<StateMetaGraph>(move(spl)), stateSymbol(stateSymbol) {}


unsigned int
StateMetaGraph::insertState(DagNode* stateDag)
{
	auto nextState = StateDagContainer::insertDag(stateDag);
	if (nextState == this->seen.size())	// if a new state identified
	{
		Vector<DagNode*> arg(1);
		arg[0] = this->getStateDag(nextState);
		auto activeDag = stateSymbol->makeDagNode(arg);		// make META transitions from [STATE] ...

		unique_ptr<State> s(new State);
		s->explore.reset(new ActiveState(this->getContext(), activeDag));
		static_cast<StateMetaGraph*>(this)->updateStateLabel(arg[0], *s);
		this->seen.push_back(std::move(s));
	}
	return nextState;
}

DagNode*
SystemGraphTraits<StateMetaGraph>::ActiveState::getNextStateDag(RewritingContext& parent)
{
	auto ns = SystemGraphTraits<StateSystemGraph<StatePropLabel>>::ActiveState::getNextStateDag(parent);
	return ns != nullptr ? safeCast(FreeDagNode*,ns)->getArgument(1) : nullptr;		// return state of "subst |->[state]"
}

} /* namespace modelChecker */
