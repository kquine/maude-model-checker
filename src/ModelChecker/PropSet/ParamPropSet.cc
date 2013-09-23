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

const set<const ParamSubstitution*> ParamPropSet::emptySet;

bool
ParamPropSet::isParamProp(unsigned int propId) const
{
	return propTable.isParamProp(propId);
}

void
ParamPropSet::setTrue(unsigned int propId)
{
	PropSet::setTrue(propId);
	if (auto pmm = propTable.getParamMatches(propId))
	{
		for (auto& j : *pmm)
			setTrueParamSubst(j.first, j.second);
	}
}

void
ParamPropSet::setTrue(const NatSet& tPids)
{
	PropSet::setTrue(tPids);
	for (auto p : tPids)
	{
		if (auto pmm = propTable.getParamMatches(p))
		{
			for (auto& j : *pmm)
				setTrueParamSubst(j.first, j.second);
		}
	}
}

void
ParamPropSet::setTrue(const PropSet& ps)
{
	PropSet::setTrue(ps);
	if (const ParamPropSet* pps = dynamic_cast<const ParamPropSet*>(&ps))
	{
		for (const auto& j : pps->trueParamSubstRefs)
			setTrueParamSubst(j.first, j.second);
	}
}

template <typename T> void
ParamPropSet::setTrueParamSubst(unsigned int propId, const T& substIds)
{
	trueParamSubstRefs[propId].insert(substIds.begin(), substIds.end());
}

const set<const ParamSubstitution*>&
ParamPropSet::getTrueParamSubst(unsigned int propId) const
{
	auto it = trueParamSubstRefs.find(propId);
	return it != trueParamSubstRefs.end() ? it->second : emptySet;
}

void
ParamPropSet::dump(ostream& s)
{
	PropSet::dump(s);
	s << ", ";
	for (const auto& j : trueParamSubstRefs)
	{
		s << "(" << j.first << " |->";
		for (auto k : j.second)
			s<< " " << k;
		s << ") ";
	}

}

} /* namespace modelChecker */
