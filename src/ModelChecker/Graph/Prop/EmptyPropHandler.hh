/*
 * EmptyPropHandler.hh
 *
 *  Created on: Jun 30, 2011
 *      Author: kquine
 */

#ifndef EMPTYPROPHANDLER_HH_
#define EMPTYPROPHANDLER_HH_
#include "PropHandler.hh"

namespace modelChecker {

/*
 * A special prop handler with no prop.
 */
class EmptyPropHandler: public PropHandler
{
public:
	struct PropSet: PropHandler::PropSet
	{
		bool operator<(const PropHandler::PropSet& td) const	{ return false; }
	};
	EmptyPropHandler() {}
	bool isSatisfied(const PropHandler::PropSet* d, int propId) const	{ CantHappen("EmptyPropHandler::isSatisfied");
	     	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	  return false; }
	bool isRelated(int propId) const									{ return false; }
	void updateProp(PropHandler::PropSet* d, DagNode* t)				{ /* Do nothing */ }
	void updateProp(PropHandler::PropSet* d, RewriteTransitionState* t)	{ /* Do nothing */ }
};

}

#endif /* EMPTYPROPHANDLER_HH_ */
