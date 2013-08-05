/*
 * CounterExampleSymbol.cc
 *
 *  Created on: Nov 6, 2010
 *      Author: kquine
 */

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "builtIn.hh"
#include "higher.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "dagArgumentIterator.hh"
#include "rewritingContext.hh"
#include "symbolMap.hh"
#include "dagNodeSet.hh"
#include "rule.hh"

//      built in class definitions
#include "bindingMacros.hh"

#include "CounterExampleSymbol.hh"

//#define TDEBUG

namespace modelChecker {

CounterExampleSymbol::CounterExampleSymbol(int id): FreeSymbol(id, 2) {}

DagNode*
CounterExampleSymbol::makeCounterexample(const DagGraph& dg, const list<Edge>& path, const list<Edge>& cycle)
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
	return this->makeDagNode(args);
}


DagNode*
CounterExampleSymbol::makeTransitionList(const DagGraph& dg, const list<Edge>& path) const
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
CounterExampleSymbol::makeTransition(const DagGraph& dg, int stateNr, int count) const
{
	static Vector<DagNode*> targs(2);
	targs[0] = dg.getStateDag(stateNr);
	targs[1] = dg.getTransitionDag(stateNr, count);
	return transitionSymbol->makeDagNode(targs);
}

bool
CounterExampleSymbol::attachData(const Vector<Sort*>& opDeclaration, const char* purpose, const Vector<const char*>& data)
{
    NULL_DATA(purpose, LTLRCounterExampleSymbol, data);
    return  FreeSymbol::attachData(opDeclaration, purpose, data);
}

bool
CounterExampleSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
    BIND_SYMBOL(purpose, symbol, transitionSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, transitionListSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, nilTransitionListSymbol, Symbol*);
	return false;
}

bool
CounterExampleSymbol::attachTerm(const char* purpose, Term* term)
{
    BIND_TERM(purpose, term, falseTerm);
    return false;
}

void
CounterExampleSymbol::copyAttachments(CounterExampleSymbol* orig, SymbolMap* map)
{
	COPY_SYMBOL(orig, transitionSymbol, map, Symbol*);
	COPY_SYMBOL(orig, transitionListSymbol, map, Symbol*);
	COPY_SYMBOL(orig, nilTransitionListSymbol, map, Symbol*);
    COPY_TERM(orig, falseTerm, map);
}

void
CounterExampleSymbol::getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols)
{
    APPEND_SYMBOL(purposes, symbols, transitionSymbol);
	APPEND_SYMBOL(purposes, symbols, transitionListSymbol);
	APPEND_SYMBOL(purposes, symbols, nilTransitionListSymbol);
}

void
CounterExampleSymbol::getTermAttachments(Vector<const char*>& purposes, Vector<Term*>& terms)
{
    APPEND_TERM(purposes, terms, falseTerm);
}

void
CounterExampleSymbol::postInterSymbolPass()
{
    PREPARE_TERM(falseTerm);
}

void
CounterExampleSymbol::reset()
{
    falseTerm.reset();	// so false dag can be garbage collected
}

}

