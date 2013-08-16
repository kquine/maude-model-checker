/*
 * SystemGraph.hh
 *
 *  Created on: Aug 13, 2013
 *      Author: kquine
 */

#ifndef SYSTEMGRAPH_HH_
#define SYSTEMGRAPH_HH_

namespace modelChecker {

class SystemGraph
{
public:
	virtual ~SystemGraph() {}

    virtual int getNrStates() const = 0;
    virtual int getNrTransitions(int stateNr) const = 0;
    virtual int getNextState(int stateNr, int index) = 0;
};

} /* namespace modelChecker */
#endif /* SYSTEMGRAPH_HH_ */
