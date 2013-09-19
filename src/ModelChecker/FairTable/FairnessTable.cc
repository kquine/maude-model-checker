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
FairnessTable<Formula>::getFairFormula(index_type fairId) const
{
	return static_cast<GroundFairness&>(*fairTable[fairId]).formula;
}

template <typename Formula> bool
FairnessTable<Formula>::isStateFairness(index_type fairId) const
{
	return static_cast<GroundFairness&>(*fairTable[fairId]).noEvent;
}

template <typename Formula> bool
FairnessTable<Formula>::isStateEventFairness(index_type fairId) const
{
	return ! isStateFairness(fairId);
}

template <typename Formula>
typename FairnessTable<Formula>::index_type
FairnessTable<Formula>::insertFairnessFormula(const Formula& f, const vector<index_type>& propIds, DagNode* fairDag)
{
	auto newIndex = fairTable.size();
	auto fi = fairFormulas.insert(make_pair(f,newIndex));
	if (fi.second)	// when new fairness formula identified
	{
		fairTable.push_back( createFormulaFairness(fi.first->first, propIds, fairDag) );
		origFairIds.insert(newIndex);
	}
	return fi.first->second;
}

template <typename Formula>
unique_ptr<typename FairnessTable<Formula>::GroundFairness>
FairnessTable<Formula>::createFormulaFairness(const Formula& f, const vector<index_type>& propIds, DagNode*) const
{
	bool noEvent = none_of(propIds.begin(), propIds.end(), [&](index_type i){ return propTable.isEventProp(i); });
	return unique_ptr<GroundFairness>(new GroundFairness(f, noEvent));
}



} /* namespace modelChecker */
