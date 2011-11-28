/*
 * WeakFairHandler.hh
 *
 *  Created on: Jul 8, 2011
 *      Author: kquine
 */

#ifndef WEAKFAIRHANDLER_HH_
#define WEAKFAIRHANDLER_HH_
#include "BasicFairHandler.hh"

namespace modelChecker {

class WeakFairHandler: public BasicFairHandler
{
public:
	struct FairSet: public BasicFairHandler::FairSet
	{
		NatSet satisfiedWeakFair;
		bool operator<(const FairSet& td) const;
	};
	struct FairGoal: public BasicFairHandler::FairGoal
	{
		NatSet weakFairGoal;	// falsified entities
		bool empty() const;
	};
	struct FairBad: public BasicFairHandler::FairBad
	{
		bool empty() const;
	};

	WeakFairHandler(const Vector<Bdd>& fTable, ProofTermGenerator* pfg = NULL);
	int getNrFairness() const;

	bool satisfiedFairSet(const FairSet* f) const;
	bool badFairSet(const FairSet* f, const FairBad* b) const;

	void mergeFairSet(FairSet* f, const FairSet* g);
	void mergeFairBad(FairBad* a, const FairBad* b);
	bool mergeFairGoal(FairGoal* goal, const FairSet* f);

	FairGoal* makeFairGoal(const FairSet* f) const;
	FairBad* makeBadGoal(const FairSet* f) const;

	void updateFairSet(FairSet* d, DagNode* stateDag);
	void updateFairSet(FairSet* d, RewriteTransitionState* transition);

private:
	NatSet allWeakFair;
	Vector<Bdd> fairConditions;

};

}

#endif /* WEAKFAIRHANDLER_HH_ */
