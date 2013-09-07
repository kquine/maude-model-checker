/*
 * EmptyFairHandler.hh
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#ifndef EMPTYFAIRHANDLER_HH_
#define EMPTYFAIRHANDLER_HH_
#include "FairHandler.hh"

namespace modelChecker {

class EmptyFairHandler: public FairHandler
{
public:
	struct Label: public FairHandler::Label
	{
		bool operator<(const FairHandler::Label& l) const 			{ return false; }
	};

	void updateLabel(const FairSet& f, FairHandler::Label& l) const	{ /* Do nothing */ };
};

} /* namespace modelChecker */
#endif /* EMPTYFAIRHANDLER_HH_ */
