/*
 * KripkeStructure.hh
 *
 *  Created on: Aug 7, 2013
 *      Author: kquine
 */

#ifndef KRIPKESTRUCTURE_HH_
#define KRIPKESTRUCTURE_HH_
#include "bdd.hh"

namespace modelChecker {

class KripkeStructure
{
public:
	virtual ~KripkeStructure() {}

	virtual int getNrStates() const = 0;
	virtual int getNrTransitions(int stateNr) const = 0;
	virtual int getNextState(int stateNr, int index) = 0;

	bool satisfiesStateFormula(Bdd formula, int stateNr);
protected:
	virtual bool satisfiesStateProp(int propId, int stateNr) = 0;
};

} /* namespace modelChecker */
#endif /* KRIPKESTRUCTURE_HH_ */
