/*
 * WeakFairHandler.cc
 *
 *  Created on: Jul 8, 2011
 *      Author: kquine
 */

#include "macros.hh"
#include "vector.hh"

#include "natSet.hh"
#include "WeakFairHandler.hh"

namespace modelChecker {

WeakFairHandler::WeakFairHandler(const Vector<Bdd>& fTable)
{
	fairConditions = fTable;	// copy
	for (int i = fairConditions.size() - 1; i >= 0; --i)
		allWeakFair.insert(i);
}

int
WeakFairHandler::getNrFairness() const
{
	return fairConditions.size();
}

bool
WeakFairHandler::satisfiedFairSet(const FairSet* f) const
{
	return allWeakFair == f->satisfiedWeakFair;
}

bool
WeakFairHandler::badFairSet(const FairSet* f, const FairBad* b)
{
	return false;
}

void
WeakFairHandler::mergeFairSet(FairSet* f, const FairSet* g)
{
	f->satisfiedWeakFair.insert(g->satisfiedWeakFair);
}

void
WeakFairHandler::mergeFairBad(FairBad* a, const FairBad* b) {}

bool
WeakFairHandler::mergeFairGoal(FairGoal* goal, const FairSet* f)
{
	if (goal->weakFairGoal.disjoint(f->satisfiedWeakFair))
		return false;
	else
	{
		goal->weakFairGoal.subtract(f->satisfiedWeakFair);
		return true;
	}
}

WeakFairHandler::FairGoal*
WeakFairHandler::makeFairGoal(const FairSet* f) const
{
	FairGoal* goal = new FairGoal();
	goal->weakFairGoal = f->satisfiedWeakFair;
	return goal;
}

WeakFairHandler::FairBad*
WeakFairHandler::makeBadGoal(const FairSet* f) const
{
	return new FairBad();
}

void
WeakFairHandler::updateFairSet(FairSet* d, DagNode* stateDag)
{

}

void
WeakFairHandler::updateFairSet(FairSet* d, RewriteTransitionState* transition)
{

}


bool
WeakFairHandler::FairSet::operator<(const FairSet& td)
{
	return satisfiedWeakFair < td.satisfiedWeakFair;
};

bool
WeakFairHandler::FairGoal::empty() const
{
	return weakFairGoal.empty();
}


bool
WeakFairHandler::FairBad::empty() const
{
	return true;
}


}
