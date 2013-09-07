/*
 * WeakFairSet.cc
 *
 *  Created on: Dec 10, 2010
 *      Author: kquine
 */
#include "macros.hh"
#include "vector.hh"

#include "natSet.hh"
#include "WeakFairSet.hh"

namespace modelChecker {


WeakFairSet::WeakFairSet(const NatSet& falsified): falsifiedWeakFair(falsified) {}

void
WeakFairSet::merge(const WeakFairSet* wf)
{
	falsifiedWeakFair.intersect(wf->falsifiedWeakFair);
}


bool
WeakFairSet::isSatisfied() const
{
	return falsifiedWeakFair.empty();
}

void
WeakFairSet::dump(ostream& o) const
{
	o << "(weak: " << falsifiedWeakFair << ")";
}


FairSet::Goal*
WeakFairSet::makeFairGoal() const
{
	return new Goal(this);
}

FairSet::Bad*
WeakFairSet::makeBadGoal() const
{
	return new Bad;
}

WeakFairSet::Goal::Goal(const WeakFairSet* fs): weakFairGoal(fs->falsifiedWeakFair) {}

bool
WeakFairSet::Goal::empty() const
{
	return weakFairGoal.empty();
}

bool
WeakFairSet::Goal::update(const FairSet* f)
{
	const WeakFairSet* nf = safeCast(const WeakFairSet*,f);
	if (weakFairGoal.disjoint(nf->falsifiedWeakFair))
		return false;
	else
	{
		weakFairGoal.subtract(nf->falsifiedWeakFair);	// FIXME: satisfied -> falsified??
		return true;
	}
}

void
WeakFairSet::Goal::dump(ostream& o) const
{
	o << "[weak goal: " << weakFairGoal << "]";
}

}
