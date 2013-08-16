/*
 * CounterExampleGenerator.hh
 *
 *  Created on: Aug 6, 2013
 *      Author: kquine
 */

#ifndef COUNTEREXAMPLEGENERATOR_HH_
#define COUNTEREXAMPLEGENERATOR_HH_
#include <list>
#include "Graph/DagGraphMap.hh"

namespace modelChecker {

class CounterExampleGenerator
{
	typedef DagGraphMap::Edge	Edge;

public:
	CounterExampleGenerator(Symbol* counterexampleSymbol, Symbol* transitionSymbol,
			Symbol* transitionListSymbol, Symbol* nilTransitionListSymbol, DagNode* deadlockDag);

	DagNode* makeCounterexample(const DagGraphMap& dg, const list<Edge>& path, const list<Edge>& cycle) const;

private:
    DagNode* makeTransitionList(const DagGraphMap& dg, const list<Edge>& path) const;
    DagNode* makeTransition(const DagGraphMap& dg, int stateNr, int count) const;

    Symbol* counterexampleSymbol;
    Symbol* transitionSymbol;
    Symbol* transitionListSymbol;
    Symbol* nilTransitionListSymbol;
    DagNode* deadlockDag;
};

} /* namespace modelChecker */
#endif /* COUNTEREXAMPLEGENERATOR_HH_ */
