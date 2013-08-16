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

class LabeledKripkeStructure: public KripkeStructure
{
public:
	virtual ~LabeledKripkeStructure() {}
	bool satisfiesStateEventFormula(Bdd formula, int stateNr, int transitionNr);
protected:
	virtual bool isEventProp(int propId) const = 0;
	virtual bool satisfiesEventProp(int propId, int stateNr, int transitionNr) = 0;
};

}

#endif /* LABELEDKRIPKESTRUCTURE_HH_ */
