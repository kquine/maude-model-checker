/*
 * SystemEventGraph.hh
 *
 *  Created on: Dec 2, 2010
 *      Author: kquine
 */

#ifndef SYSTEMEVENTGRAPH_HH_
#define SYSTEMEVENTGRAPH_HH_
#include "SystemGraph.hh"

namespace modelChecker {

//
//	State/Event Automaton Interface
//
class SystemEventGraph: public SystemGraph
{
public:
	// we assume that the formula contains event props.
	pair<bool,Bdd> satisfiesFormula(Bdd formula, int stateNr);
	bool satisfiesFormula(Bdd formula, int stateNr, int transitionNr);
protected:
	virtual bool checkEventLabel(int propId, int stateNr, int transitionNr) = 0;
	virtual bool isEventProp(int propId) const = 0;
};

}

#endif /* SYSTEMEVENTGRAPH_HH_ */
