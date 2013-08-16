/*
 * CounterExampleGenerator.cc
 *
 *  Created on: Aug 6, 2013
 *      Author: kquine
 */

//  utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "builtIn.hh"
#include "higher.hh"

// interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "dagArgumentIterator.hh"
#include "rewritingContext.hh"
#include "symbolMap.hh"
#include "dagNodeSet.hh"
#include "rule.hh"

// ltlr definitions
#include "CounterExampleGenerator.hh"

namespace modelChecker {

CounterExampleGenerator::CounterExampleGenerator(Symbol* counterexampleSymbol, Symbol* transitionSymbol,
		Symbol* transitionListSymbol, Symbol* nilTransitionListSymbol, DagNode* deadlockDag):
				counterexampleSymbol(counterexampleSymbol), transitionSymbol(transitionSymbol),
				transitionListSymbol(transitionListSymbol), nilTransitionListSymbol(nilTransitionListSymbol),
				deadlockDag(deadlockDag) {}

DagNode*
CounterExampleGenerator::makeCounterexample(const DagGraphMap& dg, const list<Edge>& path, const list<Edge>& cycle) const
{
#ifdef TDEBUG
	cout << "counter example: " << endl;
	cout << "  path: ";
	FOR_EACH_CONST(i, list<Edge>, path) cout << i->first << "=(" << i->second << ")=> ";
	cout << "\n  cycle: ";
	FOR_EACH_CONST(j, list<Edge>, cycle) cout << j->first << "=(" << j->second << ")=> ";
	cout << endl;
#endif

	static Vector<DagNode*> args(2);
	args[0] = makeTransitionList(dg, path);
	args[1] = makeTransitionList(dg, cycle);
	return counterexampleSymbol->makeDagNode(args);
}


DagNode*
CounterExampleGenerator::makeTransitionList(const DagGraphMap& dg, const list<Edge>& path) const
{
    if (path.empty())
        return nilTransitionListSymbol->makeDagNode();
    else
    {
		Vector<DagNode*> args;
		FOR_EACH_CONST(i, list<Edge>, path)
			args.append(makeTransition(dg, i->first, i->second));
		return (args.length() == 1) ? args[0] : transitionListSymbol->makeDagNode(args);
    }
}

DagNode*
CounterExampleGenerator::makeTransition(const DagGraphMap& dg, int stateNr, int count) const
{
	static Vector<DagNode*> targs(2);
	targs[0] = dg.getStateDag(stateNr);
	targs[1] = dg.getTransitionDag(stateNr, count);
	if (targs[1] == NULL)
		targs[1] = deadlockDag;
	return transitionSymbol->makeDagNode(targs);
}

} /* namespace modelChecker */
