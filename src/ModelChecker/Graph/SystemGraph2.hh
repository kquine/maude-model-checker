/*
 * SystemGraph2.hh
 *
 *  Created on: Dec 6, 2011
 *      Author: kbae4
 */

#ifndef SYSTEMGRAPH2_HH_
#define SYSTEMGRAPH2_HH_
#include "Interface/CounterExampleGenerator.hh"

namespace modelChecker {

class SystemGraph2: public CounterExampleGenerator::DagGraph
{
public:
	virtual ~SystemGraph2() {}

	virtual int getNrStates() const = 0;
	virtual int getNrTransitions(int stateNr) const = 0;
	virtual int getNextState(int stateNr, int index) = 0;
	virtual int getStateParent(int stateNr) const = 0;	// debugging purpose
};

} /* namespace modelChecker */
#endif /* SYSTEMGRAPH2_HH_ */
