/*
 * KripkeStructure.hh
 *
 *  Created on: May 24, 2011
 *      Author: kquine
 */

#ifndef KRIPKESTRUCTURE_HH_
#define KRIPKESTRUCTURE_HH_

namespace modelChecker {

class KripkeStructure
{
public:
	virtual ~KripkeStructure() {}
	virtual int getNrStates() const = 0;
	virtual int getNrTransitions(int stateNr) const = 0;
	virtual int getNextState(int stateNr, int index) = 0;
	virtual bool satisfiesStateFormula(Bdd formula, int stateNr) = 0;
};

}

#endif /* KRIPKESTRUCTURE_HH_ */
