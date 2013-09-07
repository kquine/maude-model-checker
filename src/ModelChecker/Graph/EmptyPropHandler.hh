/*
 * EmptyPropHandler.hh
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#ifndef EMPTYPROPHANDLER_HH_
#define EMPTYPROPHANDLER_HH_
#include "PropHandler.hh"

namespace modelChecker {

class EmptyPropHandler: public PropHandler
{
public:
	struct Label: public PropHandler::Label
	{
		bool operator<(const PropHandler::Label& l) const				{ return false; }
	};

	bool satisfiesProp(int localId, const PropHandler::Label& l) const 	{ return false; }
	void updateLabel(const PropSet& t, PropHandler::Label& l) const		{ /* Do nothing */ }

	void transferTruth(const PropHandler::Label& l, PropSet& t) const	{ /* Do nothing */ }
	void clearExtra(PropHandler::Label& l) const						{ /* Do nothing */ }
};

} /* namespace modelChecker */
#endif /* EMPTYPROPHANDLER_HH_ */
