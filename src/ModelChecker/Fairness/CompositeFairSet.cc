/*
 * CompositeFairSet.cc
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

#include "macros.hh"
#include "vector.hh"

#include "natSet.hh"
#include "CompositeFairSet.hh"

namespace modelChecker {

FairSet::Goal*
CompositeFairSet::makeFairGoal() const
{
	return new Goal(this);
}

FairSet::Bad*
CompositeFairSet::makeBadGoal() const
{
	return new Bad(this);
}


void
CompositeFairSet::dump(ostream& o) const
{
	o << "[composited:";
	for (auto& i : fairSets)
	{
		o << " ";
		i->dump(o);
	}
	o << "]";
}

CompositeFairSet::Goal::Goal(const CompositeFairSet* fs)
{
	for (auto& i : fs->fairSets)
		fairGoals.emplace_back(i->makeFairGoal());
}

bool
CompositeFairSet::Goal::empty() const
{
	for (auto& i : fairGoals)
		if ( ! i->empty())
			return false;
	return true;
}

void
CompositeFairSet::Goal::dump(ostream& o) const
{
	o << "[composited:";
	for (auto& i : fairGoals)
	{
		o << " ";
		i->dump(o);
	}
	o << "]";
}

CompositeFairSet::Bad::Bad(const CompositeFairSet* fs)
{
	for (auto& i : fs->fairSets)
		fairBadSets.emplace_back(i->makeBadGoal());
}

bool
CompositeFairSet::Bad::isBad(const FairSet& f) const
{
	const CompositeFairSet& nf = static_cast<const CompositeFairSet&>(f);
	for (unsigned int i = 0; i < fairBadSets.size(); ++i)
		if ( fairBadSets[i]->isBad(*nf.fairSets[i]) )
			return true;
	return false;
}

bool
CompositeFairSet::Bad::empty() const
{
	for(auto& i : fairBadSets)
		if ( ! i->empty())
			return false;
	return true;
}

void
CompositeFairSet::Bad::merge(const FairSet::Bad& b)
{
	const Bad& nb = static_cast<const Bad&>(b);
	for (unsigned int i = 0; i < fairBadSets.size(); ++i)
		fairBadSets[i]->merge(*nb.fairBadSets[i]);
}

} /* namespace modelChecker */
