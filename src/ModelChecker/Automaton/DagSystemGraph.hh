/*
 * DagSystemGraph.hh
 *
 *  Created on: Sep 10, 2013
 *      Author: kquine
 */

#ifndef DAGSYSTEMGRAPH_HH_
#define DAGSYSTEMGRAPH_HH_
#include "SystemGraph.hh"

namespace modelChecker {

class DagSystemGraph: public SystemGraph
{
public:
	virtual ~DagSystemGraph()	{}

	virtual DagNode* getStateDag(int stateNr) const = 0;
	virtual DagNode* getTransitionDag(int stateNr, int index) const = 0;
};

} /* namespace modelChecker */
#endif /* DAGSYSTEMGRAPH_HH_ */
