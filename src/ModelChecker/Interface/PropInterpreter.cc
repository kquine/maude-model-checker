/*
 * PropInterpreter.cc
 *
 *  Created on: Jun 2, 2011
 *      Author: kquine
 */

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      core class definitions
#include "symbolMap.hh"

//		built in class definitions
#include "bindingMacros.hh"

#include "PropInterpreter.hh"

namespace modelChecker {

PropInterpreter::PropInterpreter(): tauSymbol(NULL) {}

bool
PropInterpreter::isEventProp(DagNode* propDag) const
{
	Assert(tauSymbol != NULL, "No tau symbol attached");
	Assert(propDag->getSortIndex() != Sort::SORT_UNKNOWN, "The sort of " << QUOTE(propDag) << " is not computed yet.");
	return propDag->leq(tauSymbol->getOpDeclarations()[0].getDomainAndRange()[tauSymbol->arity()]);
}

bool
PropInterpreter::isParamProp(DagNode* propDag) const
{
	// any proposition that contains variables are parametric
	return !propDag->isGround();
}

bool
PropInterpreter::attachSymbol(const char* purpose, Symbol* symbol)
{
	BIND_SYMBOL(purpose, symbol, tauSymbol, Symbol*);
	return false;
}

void
PropInterpreter::copyAttachments(PropInterpreter* orig, SymbolMap* map)
{
	COPY_SYMBOL(orig, tauSymbol, map, Symbol*);
}

void
PropInterpreter::getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols)
{
	APPEND_SYMBOL(purposes, symbols, tauSymbol);
}

}
