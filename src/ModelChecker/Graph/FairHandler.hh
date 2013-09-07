/*
 * FairHandler.hh
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#ifndef FAIRHANDLER_HH_
#define FAIRHANDLER_HH_
#include "Fairness/FairSet.hh"

namespace modelChecker {

class FairHandler
{
public:
	struct Label
	{
		virtual bool operator<(const Label& l) const = 0;
	};

	virtual ~FairHandler() {}

	virtual void updateLabel(const FairSet& f, Label& l) const = 0;
};

} /* namespace modelChecker */
#endif /* FAIRHANDLER_HH_ */
