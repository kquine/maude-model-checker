/*
 * EmptyFairHandler.hh
 *
 *  Created on: Jun 30, 2011
 *      Author: kquine
 */

#ifndef EMPTYFAIRHANDLER_HH_
#define EMPTYFAIRHANDLER_HH_
#include "FairHandler.hh"

namespace modelChecker {

class EmptyFairHandler: public FairHandler
{
public:
	typedef FairHandler::PropSet	PropSet;
	struct FairData: public FairHandler::FairData
	{
		bool operator<(const FairHandler::FairData& td) const	{ return false; }
	};

	EmptyFairHandler() {}
	int getNrFairness() const	{ return 0; };
	void updateFairData(FairHandler::FairData* d, PropSet* s, PropSet* e = NULL)	{ /* Do nothing */ }
};

}

#endif /* EMPTYFAIRHANDLER_HH_ */
