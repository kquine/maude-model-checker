/*
 * CounterExampleGenerator.hh
 *
 *  Created on: Aug 6, 2013
 *      Author: kquine
 */

#ifndef COUNTEREXAMPLEGENERATOR_HH_
#define COUNTEREXAMPLEGENERATOR_HH_
#include <list>
#include "Automaton/DagSystemGraph.hh"
#include "Search/ModelChecker.hh"

namespace modelChecker {

class CounterExampleGenerator
{
	using Edge = ModelChecker::Edge;

public:
	CounterExampleGenerator(Symbol* counterexampleSymbol, Symbol* transitionSymbol, Symbol* transitionListSymbol, Symbol* nilTransitionListSymbol, DagNode* deadlockDag);

	DagNode* makeCounterexample(const DagSystemGraph& dg, const list<Edge>& path, const list<Edge>& cycle) const;

private:
    DagNode* makeTransitionList(const DagSystemGraph& dg, const list<Edge>& path) const;
    DagNode* makeTransition(const DagSystemGraph& dg, unsigned int stateNr, unsigned int count) const;

    Symbol* counterexampleSymbol;
    Symbol* transitionSymbol;
    Symbol* transitionListSymbol;
    Symbol* nilTransitionListSymbol;
    DagNode* deadlockDag;
};

} /* namespace modelChecker */
#endif /* COUNTEREXAMPLEGENERATOR_HH_ */
