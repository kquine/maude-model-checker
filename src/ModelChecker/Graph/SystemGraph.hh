/*
 * SystemGraph.hh
 *
 *  Created on: Jul 7, 2011
 *      Author: kquine
 */

#ifndef SYSTEMGRAPH_HH_
#define SYSTEMGRAPH_HH_
#include "FairSet/FairSet.hh"

namespace modelChecker {

class SystemGraph
{
public:
	virtual ~SystemGraph() {}

	virtual int getNrStates() const = 0;
	virtual int getNrTransitions(int stateNr) const = 0;
	virtual int getNextState(int stateNr, int index) = 0;

	virtual bool satisfiesStateProp(int propId, int stateNr) const = 0;
	virtual bool satisfiesEventProp(int propId, int stateNr, int transitionNr) const = 0;

	virtual DagNode* getStateDag(int stateNr) const = 0;
	virtual DagNode* getTransitionDag(int stateNr, int index) const = 0;
};

}

#endif /* SYSTEMGRAPH_HH_ */
