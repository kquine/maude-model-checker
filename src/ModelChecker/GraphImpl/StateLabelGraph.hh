/*
 * StateLabelGraph.hh
 *
 *  Created on: Sep 11, 2013
 *      Author: kquine
 */

#ifndef STATELABELGRAPH_HH_
#define STATELABELGRAPH_HH_
#include "bdd.hh"

namespace modelChecker {

class StateLabelGraph
{
public:
	bool satisfiesStateFormula(Bdd formula, int stateNr) const;

protected:
	virtual bool satisfiesStateProp(int propId, int stateNr) const = 0;
};

} /* namespace modelChecker */
#endif /* STATELABELGRAPH_HH_ */
