/*
 * PropHandler.hh
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#ifndef PROPHANDLER_HH_
#define PROPHANDLER_HH_
#include "Formula/PropSet.hh"

namespace modelChecker {

class PropHandler
{
public:
	struct Label
	{
		virtual bool operator<(const Label& l) const = 0;
	};

	virtual ~PropHandler() {}

	virtual bool satisfiesProp(int localId, const Label& l) const = 0;
	virtual void updateLabel(const PropSet& t, Label& l) const = 0;

	virtual void transferTruth(const Label& l, PropSet& t) const = 0;
	virtual void clearExtra(Label& l) const = 0;
};

} /* namespace modelChecker */
#endif /* PROPHANDLER_HH_ */
