/*
 * RealizedFairnessTable.cc
 *
 *  Created on: Sep 17, 2013
 *      Author: kquine
 */

// utility stuff
#include <queue>
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "temporal.hh"
#include "interface.hh"
#include "core.hh"

// ltlr definitions
#include "RealizedFairnessTable.hh"

namespace modelChecker {

const ParamSubstitutionBuilder&
RealizedFairnessTable::getSubstBuilder(index_type fairId) const
{
	return getParamInfo(fairId).builder;
}

RealizedFairnessTable::index_type
RealizedFairnessTable::getRealizedFairId(index_type fairId, const NatSet& realizedFair) const
{
	queue<index_type> bfs;
	bfs.push(fairId);

	while ( !bfs.empty() )	// find the most specific ancestor of fairId in realizedFair
	{
		index_type cur = bfs.front();
		bfs.pop();
		if ( realizedFair.contains(cur) )
			return cur;
		for (auto k : getInstanceInfo(cur).directBase)
			bfs.push(k);
	}
	return getInstanceInfo(fairId).paramFairId;	// the base id (all bots), otherwise
}

RealizedFairnessTable::index_type
RealizedFairnessTable::insertFairnessInstance(index_type paramFairId, const ParamSubstitution& subst)
{
	auto& substMap = getParamInfo(paramFairId).substMap;
	auto si = substMap.insert(make_pair(subst,getNextFairIndex()));
	if (si.second)	// if newly identified
	{
		insertInstance(paramFairId, &si.first->first);
		updateInstanceBaseMap(substMap, si.first);
	}
	return si.first->second;
}


void
RealizedFairnessTable::updateInstanceBaseMap(const InstanceSubstMap& substMap, const InstanceSubstMap::const_iterator f)
{
	//NOTE: the keys in InstanceSubstMap are ordered!
	//
	auto& fBase = getInstanceInfo(f->second).directBase;
	for (auto i = substMap.cbegin(); i != f; ++i)		// iterating over more general substs than f
		if (i->first.subsume(f->first))
		{
			const auto& iBase = getInstanceInfo(i->second).directBase;
			for (auto k = fBase.begin(); k != fBase.end(); )
				iBase.find(*k) != iBase.end() ? fBase.erase(k++) : ++k;		// any direct base of f is removed if it's also one of j.
			fBase.insert(i->second);
		}

	vector<bool> fDes(substMap.size());
	for (auto j = next(f); j != substMap.cend(); ++j)	// iterating over more specific substs than f
		if (f->first.subsume(j->first))
		{
			auto& jBase = getInstanceInfo(j->second).directBase;
			if (none_of(jBase.begin(), jBase.end(), [&fDes] (index_type l) { return fDes[l]; }))	// if any direct base of j has not been marked as a descendant of f.
			{
				for (auto k = jBase.begin(); k != jBase.end(); )
					fBase.find(*k) != fBase.end() ? jBase.erase(k++) : ++k;	// any direct base of j is removed if it's also one of f
				jBase.insert(f->second);
			}
			fDes[j->second] = true;
		}
}


} /* namespace modelChecker */
