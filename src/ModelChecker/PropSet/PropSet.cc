/*
 * PropSet.cc
 *
 *  Created on: Aug 26, 2013
 *      Author: kquine
 */

#include "macros.hh"
#include "vector.hh"
#include "natSet.hh"
#include "PropSet.hh"

namespace modelChecker {

PropSet::PropSet(PropSet&& other) noexcept
{
	truePropIds.swap(other.truePropIds);
}

const NatSet&
PropSet::getTruePropIds() const
{
	return truePropIds;
}

bool
PropSet::isTrue(int propId) const
{
	return truePropIds.contains(propId);
}

void
PropSet::setTrue(int propId)
{
	truePropIds.insert(propId);
}

void
PropSet::setTrue(const PropSet& ps)
{
	truePropIds.insert(ps.truePropIds);
}

void
PropSet::dump(ostream& s)
{
	s << truePropIds;
}



} /* namespace modelChecker */
