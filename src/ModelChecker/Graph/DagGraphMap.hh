/*
 * DagGraph.hh
 *
 *  Created on: Aug 6, 2013
 *      Author: kquine
 */

#ifndef DAGGRAPH_HH_
#define DAGGRAPH_HH_

namespace modelChecker {

class DagGraphMap
{
public:
	typedef pair<int,int>	Edge;

	virtual ~DagGraphMap() {}
	virtual DagNode* getStateDag(int stateNr) const = 0;
	virtual DagNode* getTransitionDag(int stateNr, int index) const = 0;
};


} /* namespace modelChecker */
#endif /* DAGGRAPH_HH_ */
