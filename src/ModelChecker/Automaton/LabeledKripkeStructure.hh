/*
 * LabeledKripkeStructure.hh
 *
 *  Created on: May 24, 2011
 *      Author: kquine
 */

#ifndef LABELEDKRIPKESTRUCTURE_HH_
#define LABELEDKRIPKESTRUCTURE_HH_
#include "KripkeStructure.hh"

namespace modelChecker {

class LabeledKripkeStructure: public modelChecker::KripkeStructure
{
public:
	virtual ~LabeledKripkeStructure() {}
	virtual bool satisfiesEventFormula(Bdd formula, int stateNr, int transitionNr) const = 0;
	virtual Bdd satisfiesEventFormula(Bdd formula, int stateNr) const = 0;	// return constrains by events
};

}

#endif /* LABELEDKRIPKESTRUCTURE_HH_ */
