/*
 * StateFoldingChecker.cc
 *
 *  Created on: Aug 7, 2011
 *      Author: kquine
 */

//      utility stuff
#include <memory>
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

#include "StateFoldingChecker.hh"

namespace modelChecker {

StateFoldingChecker::StateFoldingChecker(): foldingRelSymbol(NULL) {}

bool
StateFoldingChecker::attachSymbol(const char* purpose, Symbol* symbol)
{
	BIND_SYMBOL(purpose, symbol, foldingRelSymbol, Symbol*);
    return false;
}

void
StateFoldingChecker::copyAttachments(StateFoldingChecker* orig, SymbolMap* map)
{
	COPY_SYMBOL(orig, foldingRelSymbol, map, Symbol*);
}

void
StateFoldingChecker::getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols)
{
	APPEND_SYMBOL(purposes, symbols, foldingRelSymbol);
}

bool
StateFoldingChecker::fold(DagNode* s, DagNode* t, RewritingContext* context) const
{
	Assert(trueDag != NULL, "StateFoldingChecker::fold: trueDag not set");
	static Vector<DagNode*> args(2);
	args[0] = s;
	args[1] = t;

	DagNode* checkDag = foldingRelSymbol->makeDagNode(args);
#ifdef TDEBUG
	cout << checkDag << " is reduced to ";
#endif
	const auto_ptr<RewritingContext> testContext(context->makeSubcontext(checkDag));
	testContext->reduce();
	bool result = trueDag->equal(testContext->root());
#ifdef TDEBUG
	cout << testContext->root() << endl;
#endif
	context->addInCount(*testContext);
	return result;
}

}
