/*
 * ParamPropSet.cc
 *
 *  Created on: Aug 26, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"

// ltlr definitions
#include "ParamPropSet.hh"

namespace modelChecker {

const set<unsigned int> ParamPropSet::emptySet;

void
ParamPropSet::setTrue(unsigned int propId)
{
	PropSet::setTrue(propId);
	if (const ParamMatchMap* pmm = propTable.getParamMatches(propId))
	{
		for (const auto& j : *pmm)
			setTrueParamSubst(j.first, j.second);
	}
}

void
ParamPropSet::setTrue(const PropSet& ps)
{
	PropSet::setTrue(ps);
	if (const ParamPropSet* pps = dynamic_cast<const ParamPropSet*>(&ps))
	{
		for (const auto& j : pps->trueParamSubstIds)
			setTrueParamSubst(j.first, j.second);
	}
}

void
ParamPropSet::setTrueParamSubst(unsigned int propId, const set<unsigned int>& substIds)
{
	for (auto i : substIds)
		trueParamSubstIds[propId].insert(i);
}

const set<unsigned int>&
ParamPropSet::getTrueParamSubst(unsigned int propId) const
{
	auto it = trueParamSubstIds.find(propId);
	return it != trueParamSubstIds.end() ? it->second : emptySet;
}

void
ParamPropSet::dump(ostream& s)
{
	PropSet::dump(s);
	s << ", ";
	for (const auto& j : trueParamSubstIds)
	{
		s << "(" << j.first << " |->";
		for (auto k : j.second)
			s<< " " << k;
		s << ") ";
	}

}

} /* namespace modelChecker */
