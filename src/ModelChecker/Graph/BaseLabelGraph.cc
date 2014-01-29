/*
 * BaseLabelGraph.cc
 *
 *  Created on: Sep 29, 2013
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
#include "BaseLabelGraph.hh"

namespace modelChecker {

template <class T>
BaseLabelGraph<T>::BaseLabelGraph(unique_ptr<PropLabel>&& pl, const PropositionTable& propTable):
	propLabel(move(pl)), propTable(propTable) {}

template <class T> inline bool
BaseLabelGraph<T>::satisfiesStateFormula(Bdd formula, unsigned int stateNr) const
{
	return BddUtil::satisfiesFormula(formula, [this,stateNr] (unsigned int propId)
	{
		return static_cast<const T*>(this)->satisfiesStateProp(propId, static_cast<const T*>(this)->getState(stateNr));
	});
}

template <class T> inline bool
BaseLabelGraph<T>::satisfiesStateEventFormula(Bdd formula, unsigned int stateNr, unsigned int transitionNr) const
{
	auto check = [this,stateNr,transitionNr] (unsigned int propId)
	{
		return propTable.isEventProp(propId) ?
				  static_cast<const T*>(this)->satisfiesEventProp(propId, static_cast<const T*>(this)->getTransition(stateNr,transitionNr))
				: static_cast<const T*>(this)->satisfiesStateProp(propId, static_cast<const T*>(this)->getState(stateNr));
	};
	return BddUtil::satisfiesFormula(formula, check);
}


template <class T> inline pair<bool,Bdd>
BaseLabelGraph<T>::satisfiesPartialStateFormula(Bdd formula, unsigned int stateNr) const
{
	return BddUtil::satisfiesPartialFormula(formula,
			[this] (unsigned int propId) { return ! propTable.isEventProp(propId); },
			[this,stateNr] (unsigned int propId) { return static_cast<const T*>(this)->satisfiesStateProp(propId, static_cast<const T*>(this)->getState(stateNr)); });
}

} /* namespace modelChecker */
