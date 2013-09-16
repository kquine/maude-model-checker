/*
 * FairnessTable.cc
 *
 *  Created on: Aug 22, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// ltlr definitions
#include "FairnessTable.hh"

namespace modelChecker {

template <typename Formula>
FairnessTable<Formula>::FairnessTable(PropositionTable& propTable): propTable(propTable) {}


template <typename Formula> const Formula&
FairnessTable<Formula>::getFairFormula(unsigned int fairId) const
{
	return fairFormulas[static_cast<GroundFairness&>(*fairTable[fairId]).formulaId];
}

template <typename Formula> bool
FairnessTable<Formula>::isStateFairness(unsigned int fairId) const
{
	return static_cast<GroundFairness&>(*fairTable[fairId]).noEvent;
}

template <typename Formula> bool
FairnessTable<Formula>::isStateEventFairness(unsigned int fairId) const
{
	return ! isStateFairness(fairId);
}

template <typename Formula> unsigned int
FairnessTable<Formula>::insertFairnessFormula(const Formula& f, const set<unsigned int>& propIds, DagNode* fairDag)
{
	auto oldSize = fairFormulas.size();
	auto fi = fairFormulas.insert(f);
	if (fi >= oldSize)	// when new fairness formula identified
	{
		formulaInfoMap.push_back(fairTable.size());
		fairTable.push_back( createFormulaFairness(fi, propIds, fairDag) );
	}
	return formulaInfoMap[fi];
}

template <typename Formula>
unique_ptr<typename FairnessTable<Formula>::GroundFairness>
FairnessTable<Formula>::createFormulaFairness(unsigned int formulaId, const set<unsigned int>& propIds, DagNode*) const
{
	bool noEvent = none_of(propIds.begin(), propIds.end(), [&](unsigned int i){ return propTable.isEventProp(i); });
	return unique_ptr<GroundFairness>(new GroundFairness(formulaId, noEvent));
}



} /* namespace modelChecker */
