/*
 * BasicFairHandler.hh
 *
 *  Created on: Jul 7, 2011
 *      Author: kquine
 */

#ifndef BASICFAIRHANDLER_HH_
#define BASICFAIRHANDLER_HH_

namespace modelChecker {

/*
 * A basic fairSet handler interface.
 */
class BasicFairHandler
{
	typedef BasicPropHandler::Data	PropSet;
public:
	struct FairSet
	{
		virtual bool operator<(const Data& td) const = 0;
	};
	struct FairGoal
	{
		virtual bool empty() const = 0;
	};
	struct FairBad
	{
		virtual bool empty() const = 0;
	};

	virtual int getNrFairness() const = 0;

	virtual bool satisfiedFairSet(const FairSet* f) const = 0;
	virtual bool badFairSet(const FairSet* f, const FairBad* b) const = 0;

	virtual void mergeFairSet(FairSet* f, const FairSet* g) = 0;
	virtual void mergeFairBad(FairBad* a, const Bad* b) = 0;
	virtual bool mergeFairGoal(FairGoal* goal, const FairSet* f) = 0;

	virtual FairGoal* makeFairGoal(const FairSet* f) const = 0;
	virtual FairBad* makeBadGoal(const FairSet* f) const = 0;

	virtual void updateFairSet(FairSet* d, PropSet* stateProps, PropSet* eventProps = NULL) = 0;
};

}

#endif /* BASICFAIRHANDLER_HH_ */
