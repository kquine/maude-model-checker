/*
 * ParamPropSet.cc
 *
 *  Created on: Aug 26, 2013
 *      Author: kquine
 */

// utility stuff
#include <memory>
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"
#include "symbolMap.hh"

// ltlr definitions
#include "ParamPropSet.hh"

namespace modelChecker {

const set<int> ParamPropSet::emptySet;

ParamPropSet::ParamPropSet(const ParamPropositionTable& propTable, const PropSet& base): PropSet(base), propTable(propTable)  {}

void
ParamPropSet::setTrue(int propId)
{
	PropSet::setTrue(propId);
	if (const ParamMatchMap* pmm = propTable.getParamMatches(propId))
	{
		for (const ParamMatchMap::value_type& j : *pmm)
			setTrueParamSubst(j.first, j.second);
	}
}

void
ParamPropSet::setTrue(const PropSet& ps)
{
	PropSet::setTrue(ps);
	if (const ParamPropSet* pps = dynamic_cast<const ParamPropSet*>(&ps))
	{
		for (const ParamMatchMap::value_type& j : pps->trueParamSubstIds)
			setTrueParamSubst(j.first, j.second);
	}
}

void
ParamPropSet::setTrueParamSubst(int propId, const set<int>& substIds)
{
	for (int i : substIds)
		trueParamSubstIds[propId].insert(i);
}

const set<int>&
ParamPropSet::getTrueParamSubst(int propId) const
{
	ParamMatchMap::const_iterator it = trueParamSubstIds.find(propId);
	return it != trueParamSubstIds.end() ? it->second : emptySet;
}

void
ParamPropSet::dump(ostream& s)
{
	PropSet::dump(s);
	s << ", ";
	for (const ParamMatchMap::value_type& j : trueParamSubstIds)
	{
		s << "(" << j.first << " |->";
		for (const int k : j.second)
			s<< " " << k;
		s << ") ";
	}

}

} /* namespace modelChecker */
