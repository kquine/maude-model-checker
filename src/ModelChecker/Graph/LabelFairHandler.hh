/*
 * LabelFairHandler.hh
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#ifndef LABELFAIRHANDLER_HH_
#define LABELFAIRHANDLER_HH_
#include "FairHandler.hh"

namespace modelChecker {

class LabelFairHandler: public FairHandler
{
public:
	struct Label: public FairHandler::Label
	{
		bool operator<(const FairHandler::Label& l) const;
	};

	void updateLabel(const FairSet& f, FairHandler::Label& l) const;

};

} /* namespace modelChecker */
#endif /* LABELFAIRHANDLER_HH_ */
