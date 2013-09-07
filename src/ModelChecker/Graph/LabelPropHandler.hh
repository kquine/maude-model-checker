/*
 * LabelPropHandler.hh
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#ifndef LABELPROPHANDLER_HH_
#define LABELPROPHANDLER_HH_
#include "PropHandler.hh"

namespace modelChecker {

class LabelPropHandler: public PropHandler
{
public:
	struct Label: public PropHandler::Label
	{
		NatSet label;
		bool operator<(const PropHandler::Label& l) const;
	};

	bool satisfiesProp(int localId, const PropHandler::Label& l) const;
	void updateLabel(const PropSet& t, PropHandler::Label& l) const;

	void transferTruth(const PropHandler::Label& l, PropSet& t) const;
	void clearExtra(PropHandler::Label& l) const;

private:
	Vector<int> propIds;
	Vector<int> extra;
};

} /* namespace modelChecker */
#endif /* LABELPROPHANDLER_HH_ */
