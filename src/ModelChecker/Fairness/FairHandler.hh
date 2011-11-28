/*
 * FairHandler.hh
 *
 *  Created on: Jul 8, 2011
 *      Author: kquine
 */

#ifndef FAIRHANDLER_HH_
#define FAIRHANDLER_HH_
#include "Prop/PropHandler.hh"

namespace modelChecker {

class FairHandler
{
public:
	typedef PropHandler::PropSet	PropSet;
	struct FairData
	{
		virtual bool operator<(const FairData& td) const = 0;
	};
	virtual ~FairHandler() {}
	virtual int getNrFairness() const = 0;
	virtual void updateFairData(FairData* d, PropSet* stateProps, PropSet* eventProps = NULL) = 0;
};

}

#endif /* FAIRHANDLER_HH_ */
