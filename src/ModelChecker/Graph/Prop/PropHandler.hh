/*
 * PropHandler.hh
 *
 *  Created on: Jul 8, 2011
 *      Author: kquine
 */

#ifndef PROPHANDLER_HH_
#define PROPHANDLER_HH_

namespace modelChecker {

class PropHandler
{
public:
	struct PropSet
	{
		virtual bool operator<(const PropSet& td) const = 0;
	};
	virtual ~PropHandler() {}
	virtual bool isSatisfied(const PropSet* d, int propId) const = 0;
	virtual bool isRelated(int propId) const = 0;
	virtual void updateProp(PropSet* d, DagNode* t) = 0;
	virtual void updateProp(PropSet* d, RewriteTransitionState* t) = 0;
};

}

#endif /* PROPHANDLER_HH_ */
