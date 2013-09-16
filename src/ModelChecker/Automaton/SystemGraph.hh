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

    virtual unsigned int getNrStates() const = 0;
    virtual unsigned int getNrTransitions(unsigned int stateNr) const = 0;
    virtual int getNextState(unsigned int stateNr, unsigned int index) = 0;		// return -1 if no more transitions

    unsigned int getTotalTransitions() const;
};

inline unsigned int
SystemGraph::getTotalTransitions() const
{
	unsigned int nrStates = getNrStates();
	unsigned int total = 0;
	for (unsigned int i = 0; i < nrStates; ++i)
		total += getNrTransitions(i);
	return total;
}


} /* namespace modelChecker */
#endif /* SYSTEMGRAPH_HH_ */
