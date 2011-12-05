/*
 * StrongFairSet.hh
 *
 *  Created on: Dec 10, 2010
 *      Author: kquine
 */

#ifndef STRONGFAIRSET_HH_
#define STRONGFAIRSET_HH_

#include "FairSet.hh"

namespace modelChecker {

class StrongFairSet: public modelChecker::FairSet
{
public:
	struct Goal;
	struct Bad;

	StrongFairSet(const NatSet& supp, const NatSet& concl);
	void merge(const StrongFairSet* sf);

	bool isSatisfied() const;
	FairSet::Goal* makeFairGoal() const;
	FairSet::Bad* makeBadGoal() const;
	void dump(ostream& o) const;
protected:
	NatSet supp_fair;	// G of fairness (G,H)
	NatSet concl_fair; 	// H of fairness (G,H)
};


class StrongFairSet::Goal: public FairSet::Goal
{
public:
	Goal(const StrongFairSet* fs);
	bool empty() const;
	bool update(const FairSet* f);
	void dump(ostream& o) const;
protected:
	NatSet strongFairGoal;
};

struct StrongFairSet::Bad: public FairSet::Bad
{
public:
	Bad(const StrongFairSet* fs);
	virtual ~Bad() {}
	bool isBad(const FairSet* f) const;
	bool empty() const;
	void merge(const FairSet::Bad* b);
private:
	NatSet badFairs;
};


}

#endif /* STRONGFAIRSET_HH_ */
