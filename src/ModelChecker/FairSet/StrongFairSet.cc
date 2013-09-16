/*
 * StrongFairSet.cc
 *
 *  Created on: Dec 10, 2010
 *      Author: kquine
 */

#include "macros.hh"
#include "vector.hh"

#include "natSet.hh"
#include "StrongFairSet.hh"

namespace modelChecker {

void
StrongFairSet::setSuppFalse(unsigned int fairId)
{
	falsifiedSupp.insert(fairId);
}

void
StrongFairSet::setConsFalse(unsigned int fairId)
{
	falsifiedCons.insert(fairId);
}

void
StrongFairSet::merge(const FairSet& f, const AbstractFairnessTable&)
{
	const StrongFairSet& sf = static_cast<const StrongFairSet&>(f);
	falsifiedSupp.intersect(sf.falsifiedSupp);
	falsifiedCons.intersect(sf.falsifiedCons);
}

bool
StrongFairSet::isSatisfied() const
{
	return falsifiedSupp.contains(falsifiedCons);
}

bool
StrongFairSet::operator<(const FairSet& f) const
{
	const StrongFairSet& sf = static_cast<const StrongFairSet&>(f);
	return falsifiedSupp < sf.falsifiedSupp ||  falsifiedCons < sf.falsifiedCons;
}

unique_ptr<FairSet>
StrongFairSet::clone() const
{
	return unique_ptr<FairSet>(new StrongFairSet(*this));
}

unique_ptr<FairSet::Goal>
StrongFairSet::makeFairGoal() const
{
	return unique_ptr<FairSet::Goal>(new Goal(*this));
}

unique_ptr<FairSet::Bad>
StrongFairSet::makeBadGoal() const
{
	return unique_ptr<FairSet::Bad>(new Bad(*this));
}

void
StrongFairSet::dump(ostream& o) const
{
	o << "(strong: " << falsifiedSupp << " , " << falsifiedCons << ")";
}

StrongFairSet::Goal::Goal(const StrongFairSet& f): strongFairGoal(f.falsifiedCons)
{
	strongFairGoal.subtract(f.falsifiedSupp);	// only (true => false)
}

bool
StrongFairSet::Goal::empty() const
{
	return strongFairGoal.empty();
}

bool
StrongFairSet::Goal::update(const FairSet& f)
{
	const StrongFairSet& sf = static_cast<const StrongFairSet&>(f);
	if (sf.falsifiedCons.contains(strongFairGoal))	// all still falsified
		return false;
	else
	{
		strongFairGoal.intersect(sf.falsifiedCons);	// goal = still falsified
		return true;
	}
}

void
StrongFairSet::Goal::dump(ostream& o) const
{
	o << "[strong goal: " << strongFairGoal << "]";
}

StrongFairSet::Bad::Bad(const StrongFairSet& f): badFairs(f.falsifiedCons)
{
	badFairs.subtract(f.falsifiedSupp);		// only (true => false)
}

bool
StrongFairSet::Bad::isBad(const FairSet& f) const
{
	const StrongFairSet& sf = static_cast<const StrongFairSet&>(f);
	return ! sf.falsifiedSupp.contains(badFairs);
}

bool
StrongFairSet::Bad::empty() const
{
	return badFairs.empty();
}

void
StrongFairSet::Bad::merge(const FairSet::Bad& b)
{
	const Bad& nb = static_cast<const Bad&>(b);
	badFairs.insert(nb.badFairs);
}

}
