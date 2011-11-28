/*
 * SystemGraph.hh
 *
 *  Created on: Oct 18, 2010
 *      Author: kquine
 */

#ifndef SYSTEMGRAPH_HH_
#define SYSTEMGRAPH_HH_

namespace ltlrModelChecker2 {

//
//	System Automaton Interface
//
struct SystemGraph
{
	virtual int getNrStates() const = 0;
	virtual int getNrTransitions(int stateNr) const = 0;
	virtual int getNextState(int stateNr, int transitionNr) = 0;

	bool satisfiesFormula(Bdd formula, int stateNr);
	virtual bool satisfiesFormula(Bdd formula, int stateNr, int transitionNr);
protected:
	virtual bool checkStateLabel(int propId, int stateNr) = 0;
};

}

#endif /* SYSTEMGRAPH_HH_ */
