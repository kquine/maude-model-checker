/*
 * NarrowingStateGenerator.cc
 *
 *  Created on: Aug 3, 2011
 *      Author: kquine
 */

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "dagArgumentIterator.hh"

//		core class definitions
#include "symbolMap.hh"
#include "rewritingContext.hh"

//		built in class definitions
#include "bindingMacros.hh"

#include "NarrowingStateGenerator.hh"

//#define TDEBUG

namespace modelChecker {

NarrowingStateGenerator::NarrowingStateGenerator():
		nextStatesSymbol(NULL), variantStatesSymbol(NULL),
		stateWrapperSymbol(NULL), stateWrapperSetSymbol(NULL), emptyWrapperSetSymbol(NULL)
{}

bool
NarrowingStateGenerator::attachSymbol(const char* purpose, Symbol* symbol)
{
	BIND_SYMBOL(purpose, symbol, nextStatesSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, variantStatesSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, stateWrapperSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, stateWrapperSetSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, emptyWrapperSetSymbol, Symbol*);
    return false;
}

void
NarrowingStateGenerator::copyAttachments(NarrowingStateGenerator* orig, SymbolMap* map)
{
	COPY_SYMBOL(orig, nextStatesSymbol, map, Symbol*);
	COPY_SYMBOL(orig, variantStatesSymbol, map, Symbol*);
	COPY_SYMBOL(orig, stateWrapperSymbol, map, Symbol*);
	COPY_SYMBOL(orig, stateWrapperSetSymbol, map, Symbol*);
	COPY_SYMBOL(orig, emptyWrapperSetSymbol, map, Symbol*);
}

void
NarrowingStateGenerator::getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols)
{
	APPEND_SYMBOL(purposes, symbols, nextStatesSymbol);
	APPEND_SYMBOL(purposes, symbols, variantStatesSymbol);
    APPEND_SYMBOL(purposes, symbols, stateWrapperSymbol);
    APPEND_SYMBOL(purposes, symbols, stateWrapperSetSymbol);
    APPEND_SYMBOL(purposes, symbols, emptyWrapperSetSymbol);
}

void
NarrowingStateGenerator::generateStates(RewritingContext* context, DagNode* stateDag,
										Symbol* genSymbol, Vector<DagNode*>& result) const
{
	static Vector<DagNode*> args(1);
	args[0] = stateDag;

	DagNode* genDag = genSymbol->makeDagNode(args);
#ifdef TDEBUG
	cout << genDag << " is reduced to ";
#endif
	const auto_ptr<RewritingContext> genContext(context->makeSubcontext(genDag));
	genContext->reduce();
	context->addInCount(*genContext);
#ifdef TDEBUG
	cout << genContext->root() << endl;
#endif

	parseStateSet(genContext->root(), result);
}

void
NarrowingStateGenerator::parseStateSet(DagNode* dag, Vector<DagNode*>& result) const
{
	Symbol* top = dag->symbol();
	if (top != emptyWrapperSetSymbol)
	{
		if (top == stateWrapperSetSymbol)
			for (DagArgumentIterator i(*dag); i.valid(); i.next())
				result.append(parseStateItem(i.argument()));
		else
			result.append(parseStateItem(dag));
	}
}

}
