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

	void setSuppFalse(unsigned int fairId);
	void setConsFalse(unsigned int fairId);

	virtual void merge(const FairSet& f, const AbstractFairnessTable& table) override;
	bool isSatisfied() const override;
	bool operator<(const FairSet& fs) const override;

	virtual unique_ptr<FairSet> clone() const override;
	unique_ptr<FairSet::Goal> makeFairGoal() const override;
	unique_ptr<FairSet::Bad> makeBadGoal() const override;

	void dump(ostream& o) const override;

protected:
	NatSet falsifiedSupp;
	NatSet falsifiedCons;
};


class StrongFairSet::Goal: public FairSet::Goal
{
public:
	Goal(const StrongFairSet& fs);
	virtual ~Goal() {}
	bool empty() const override;
	bool update(const FairSet& f) override;
	void dump(ostream& o) const override;
protected:
	NatSet strongFairGoal;
};

struct StrongFairSet::Bad: public FairSet::Bad
{
public:
	Bad(const StrongFairSet& fs);
	virtual ~Bad() {}
	bool isBad(const FairSet& f) const override;
	bool empty() const override;
	void merge(const FairSet::Bad& b) override;
private:
	NatSet badFairs;
};


}

#endif /* STRONGFAIRSET_HH_ */
