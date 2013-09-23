/*
 * EmptyFairLabel.hh
 *
 *  Created on: Sep 21, 2013
 *      Author: kquine
 */

#ifndef EMPTYFAIRLABEL_HH_
#define EMPTYFAIRLABEL_HH_

namespace modelChecker {

class EmptyFairLabel
{
public:
	struct Label
	{
		bool operator<(const Label&) const { return false; }
	};
};

} /* namespace modelChecker */
#endif /* EMPTYFAIRLABEL_HH_ */
