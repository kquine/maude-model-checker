/*
 * BaseSystemGraph.cc
 *
 *  Created on: Aug 14, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"

// ltlr definitions
#include "Utility/BddUtil.hh"
#include "BaseSystemGraph.hh"

namespace modelChecker {

template <class T>
BaseSystemGraph<T>::BaseSystemGraph(RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable):
	initial(initial), ptGenerator(ptg), propTable(propTable)
{
	initial.reduce();
}

template <class T> inline void
BaseSystemGraph<T>::init()
{
	static_cast<T*>(this)->insertState(initial.root());
}

template <class T> inline DagNode*
BaseSystemGraph<T>::getStateDag(unsigned int stateNr) const
{
	return StateDagContainer::getStateDag(stateNr);
}

template <class T> inline DagNode*
BaseSystemGraph<T>::getTransitionDag(unsigned int stateNr, unsigned int index) const
{
	Assert(stateNr < seen.size() && seen[stateNr], "BaseSystemGraph::getTransitionDag: invalid state lookup");
	return static_cast<Transition&>(*seen[stateNr]->transitions[index]).makeDag(getStateDag(stateNr), initial, ptGenerator);
}

template <class T> inline unsigned int
BaseSystemGraph<T>::getNrStates() const
{
	return seen.size();
}

template <class T> inline unsigned int
BaseSystemGraph<T>::getNrTransitions(unsigned int stateNr) const
{
	Assert(stateNr < seen.size(), "BaseSystemGraph::getNrTransitions: invalid state lookup");
	return seen[stateNr] ? seen[stateNr]->transitions.size() : 0;
}

template <class T> inline int
BaseSystemGraph<T>::getNextState(unsigned int stateNr, unsigned int index)
{
	return static_cast<T*>(this)->computeNextState(stateNr, index);
}


template <class T> inline RewritingContext&
BaseSystemGraph<T>::getContext() const
{
	return initial;
}

template <class T> inline DagNode*
BaseSystemGraph<T>::getProofTerm(const PositionState* ps, const Rule& rule, const Substitution* subst) const
{
	DagNode* pd = ptGenerator.makeProofDag(ps,rule,subst);
	pd->reduce(initial);
	return pd;
}

template <class T> inline typename BaseSystemGraph<T>::State&
BaseSystemGraph<T>::getState(unsigned int stateNr) const
{
	return static_cast<State&>(*seen[stateNr]);
}

template <class T> inline typename BaseSystemGraph<T>::Transition&
BaseSystemGraph<T>::getTransition(unsigned int stateNr, unsigned int transitionNr) const
{
	return static_cast<Transition&>(*seen[stateNr]->transitions[transitionNr]);
}


template <class T> inline bool
BaseSystemGraph<T>::satisfiesStateFormula(Bdd formula, unsigned int stateNr) const
{
	return BddUtil::satisfiesFormula(formula, [this,stateNr] (unsigned int propId)
	{
		return static_cast<const T*>(this)->satisfiesStateProp(propId, getState(stateNr));
	});
}

template <class T> inline bool
BaseSystemGraph<T>::satisfiesStateEventFormula(Bdd formula, unsigned int stateNr, unsigned int transitionNr) const
{
	auto check = [this,stateNr,transitionNr] (unsigned int propId)
	{
		return propTable.isEventProp(propId) ?
				  static_cast<const T*>(this)->satisfiesEventProp(propId, getTransition(stateNr,transitionNr))
				: static_cast<const T*>(this)->satisfiesStateProp(propId, getState(stateNr));
	};
	return BddUtil::satisfiesFormula(formula, check);
}


template <class T> inline pair<bool,Bdd>
BaseSystemGraph<T>::satisfiesPartialStateFormula(Bdd formula, unsigned int stateNr) const
{
	return BddUtil::satisfiesPartialFormula(formula,
			[this] (unsigned int propId) { return ! propTable.isEventProp(propId); },
			[this,stateNr] (unsigned int propId) { return static_cast<const T*>(this)->satisfiesStateProp(propId, getState(stateNr)); });
}

inline bool
BaseSystemGraphTraits::Transition::operator<(const Transition& t) const
{
	return nextState < t.nextState;
}

} /* namespace modelChecker */
