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
#include "temporal.hh"
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

#include "CounterExampleGenerator.hh"

//#define TDEBUG

namespace modelChecker {

template <typename SG>
CounterExampleGenerator<SG>::CounterExampleGenerator():
		counterexampleSymbol(NULL), transitionSymbol(NULL), transitionListSymbol(NULL), nilTransitionListSymbol(NULL) {}


template <typename SG> DagNode*
CounterExampleGenerator<SG>::makeCounterexample(const SG& dg,
								  	  	    const list<pair<int,int> >& path, const list<pair<int,int> >& cycle) const
{
#ifdef TDEBUG
	typedef pair<int,int> CTransition;
	cout << "counter example: " << endl;
	cout << "  path: ";
	FOR_EACH_CONST(i, list<CTransition>, path) cout << i->first << "=(" << i->second << ")=> ";
	cout << "\n  cycle: ";
	FOR_EACH_CONST(j, list<CTransition>, cycle) cout << j->first << "=(" << j->second << ")=> ";
	cout << endl;
#endif

	static Vector<DagNode*> args(2);
	args[0] = makeTransitionList(dg, path);
	args[1] = makeTransitionList(dg, cycle);
	return counterexampleSymbol->makeDagNode(args);
}


template <typename SG> DagNode*
CounterExampleGenerator<SG>::makeTransitionList(const SG& dg, const list<pair<int,int> >& path) const
{
	typedef pair<int,int> Transition;
    if (path.empty())
        return nilTransitionListSymbol->makeDagNode();
    else
    {
		Vector<DagNode*> args;
		FOR_EACH_CONST(i, list<Transition>, path)
			args.append(makeTransition(dg, i->first, i->second));
		return (args.length() == 1) ? args[0] : transitionListSymbol->makeDagNode(args);
    }
}

template <typename SG> DagNode*
CounterExampleGenerator<SG>::makeTransition(const SG& dg, int stateNr, int count) const
{
	static Vector<DagNode*> targs(2);
	targs[0] = dg.getStateDag(stateNr);
	targs[1] = dg.getTransitionDag(stateNr, count);
	return transitionSymbol->makeDagNode(targs);
}

template <typename SG> bool
CounterExampleGenerator<SG>::attachSymbol(const char* purpose, Symbol* symbol)
{
    BIND_SYMBOL(purpose, symbol, transitionSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, transitionListSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, nilTransitionListSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, counterexampleSymbol, Symbol*);
	return false;
}

template <typename SG> bool
CounterExampleGenerator<SG>::attachTerm(const char* purpose, Term* term)
{
    BIND_TERM(purpose, term, falseTerm);
    return false;
}

template <typename SG> void
CounterExampleGenerator<SG>::copyAttachments(CounterExampleGenerator* orig, SymbolMap* map)
{
    COPY_TERM(orig, falseTerm, map);
	COPY_SYMBOL(orig, transitionSymbol, map, Symbol*);
	COPY_SYMBOL(orig, transitionListSymbol, map, Symbol*);
	COPY_SYMBOL(orig, nilTransitionListSymbol, map, Symbol*);
	COPY_SYMBOL(orig, counterexampleSymbol, map, Symbol*);
}

template <typename SG> void
CounterExampleGenerator<SG>::getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols)
{
    APPEND_SYMBOL(purposes, symbols, transitionSymbol);
	APPEND_SYMBOL(purposes, symbols, transitionListSymbol);
	APPEND_SYMBOL(purposes, symbols, nilTransitionListSymbol);
	APPEND_SYMBOL(purposes, symbols, counterexampleSymbol);
}

template <typename SG> void
CounterExampleGenerator<SG>::getTermAttachments(Vector<const char*>& purposes, Vector<Term*>& terms)
{
    APPEND_TERM(purposes, terms, falseTerm);
}

template <typename SG> void
CounterExampleGenerator<SG>::postInterSymbolPass()
{
    PREPARE_TERM(falseTerm);
}

template <typename SG> void
CounterExampleGenerator<SG>::reset()
{
    falseTerm.reset();	// so false dag can be garbage collected
}

}

