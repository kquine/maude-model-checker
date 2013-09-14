/*
 * FairnessTable.cc
 *
 *  Created on: Aug 22, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "temporal.hh"
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"

// interface class definitions
#include "symbol.hh"
#include "DagNodeSet.hh"
#include "term.hh"

// core class definitions
#include "rewritingContext.hh"
#include "dagArgumentIterator.hh"

// temporal class definitions
#include "logicFormula.hh"

//      free theory class definitions
#include "freeDagNode.hh"


// ltlr definitions
#include "FairnessTable.hh"

namespace modelChecker {

template <typename Formula>
FairnessTable<Formula>::FairnessTable(PropositionTable& propTable): propTable(propTable) {}


template <typename Formula> const Formula&
FairnessTable<Formula>::getFairFormula(int fairId) const
{
	return fairFormulas[static_cast<GroundFairness&>(*fairTable[fairId]).formulaId];
}

template <typename Formula> bool
FairnessTable<Formula>::isStateFairness(int fairId) const
{
	return static_cast<GroundFairness&>(*fairTable[fairId]).noEvent;
}

template <typename Formula> bool
FairnessTable<Formula>::isStateEventFairness(int fairId) const
{
	return ! isStateFairness(fairId);
}

template <typename Formula> int
FairnessTable<Formula>::insertFairnessFormula(const Formula& f, const set<int>& propIds, DagNode* fairDag)
{
	int oldSize = fairFormulas.size();
	int fi = fairFormulas.insert(f);
	if (fi >= oldSize)	// when new fairness formula identified
	{
		formulaInfoMap.push_back(fairTable.size());
		fairTable.push_back( createFormulaFairness(fi, propIds, fairDag) );
	}
	return formulaInfoMap[fi];
}

template <typename Formula>
unique_ptr<typename FairnessTable<Formula>::GroundFairness>
FairnessTable<Formula>::createFormulaFairness(int formulaId, const set<int>& propIds, DagNode* fairDag) const
{
	bool noEvent = none_of(propIds.begin(), propIds.end(), [&](int i){ return propTable.isEventProp(i); });
	return unique_ptr<GroundFairness>(new GroundFairness(formulaId, noEvent));
}



} /* namespace modelChecker */
