/*
 * EmptyPropLabel.hh
 *
 *  Created on: Sep 21, 2013
 *      Author: kquine
 */

#ifndef EMPTYPROPLABEL_HH_
#define EMPTYPROPLABEL_HH_
#include "PropSet/PropSet.hh"

namespace modelChecker {

class EmptyPropLabel
{
public:
	struct Label
	{
		bool operator<(const Label&) const { return false; }
	};
};

} /* namespace modelChecker */
#endif /* EMPTYPROPLABEL_HH_ */
