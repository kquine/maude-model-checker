/*
 * TermUtil.hh
 *
 *  Created on: Jul 27, 2011
 *      Author: kquine
 */

#ifndef TERMUTIL_HH_
#define TERMUTIL_HH_

namespace modelChecker {

class TermUtil
{
public:
	static Term* constructTerm(DagNode* dag);

private:
	static const int nrPreallocatedArgs = 3;
};

}

#endif /* TERMUTIL_HH_ */
