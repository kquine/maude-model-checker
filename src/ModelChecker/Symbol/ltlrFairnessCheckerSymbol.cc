

//      utility stuff
#include <map>
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "temporal.hh"
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "builtIn.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

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
#include "rewriteSequenceSearch.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//		variable indexing
#include "variableDagNode.hh"
#include "narrowingVariableInfo.hh"

//		built in class definitions
#include "bindingMacros.hh"

//		temporal class definitions
#include "logicFormula.hh"

//		ltlr definitions
#include "ltlrFairnessCheckerSymbol.hh"


//#define TDEBUG

namespace modelChecker {

LTLRFairnessCheckerSymbol::LTLRFairnessCheckerSymbol(int id, int arity): TemporalSymbol(id, arity) {}

bool
LTLRFairnessCheckerSymbol::attachData(const Vector<Sort*>& opDeclaration,
                              const char* purpose,
                              const Vector<const char*>& data)
{
    NULL_DATA(purpose, LTLRFairnessCheckerSymbol, data);
    return  TemporalSymbol::attachData(opDeclaration, purpose, data);
}

bool
LTLRFairnessCheckerSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
    BIND_SYMBOL(purpose, symbol, fairnessSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, strongFairTypeSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, weakFairTypeSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, fairnessSetSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, emptyFairnessSetSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, satisfiesSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, actionmatchSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, prooftermSymbol, ProofTermSymbol*);
    BIND_SYMBOL(purpose, symbol, enabledSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, counterexampleSymbol, CounterExampleSymbol*);
	return TemporalSymbol::attachSymbol(purpose, symbol);
}

bool
LTLRFairnessCheckerSymbol::attachTerm(const char* purpose, Term* term)
{
	BIND_TERM(purpose, term, deadlockTerm);
	BIND_TERM(purpose, term, trueTerm);
    return TemporalSymbol::attachTerm(purpose, term);
}

void
LTLRFairnessCheckerSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
	LTLRFairnessCheckerSymbol* orig = safeCast(LTLRFairnessCheckerSymbol*, original);
	COPY_SYMBOL(orig, fairnessSymbol, map, Symbol*);
	COPY_SYMBOL(orig, strongFairTypeSymbol, map, Symbol*);
	COPY_SYMBOL(orig, weakFairTypeSymbol, map, Symbol*);
	COPY_SYMBOL(orig, fairnessSetSymbol, map, Symbol*);
	COPY_SYMBOL(orig, emptyFairnessSetSymbol, map, Symbol*);
	COPY_SYMBOL(orig, satisfiesSymbol, map, Symbol*);
	COPY_SYMBOL(orig, actionmatchSymbol, map, Symbol*);
	COPY_SYMBOL(orig, prooftermSymbol, map, ProofTermSymbol*);
	COPY_SYMBOL(orig, enabledSymbol, map, Symbol*);
	COPY_SYMBOL(orig, counterexampleSymbol, map, CounterExampleSymbol*);
	COPY_TERM(orig, deadlockTerm, map);
	COPY_TERM(orig, trueTerm, map);
	TemporalSymbol::copyAttachments(original, map);
}

void
LTLRFairnessCheckerSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
                                       Vector<const char*>& purposes,
                                       Vector<Vector<const char*> >& data)
{
    APPEND_DATA(purposes, data, LTLRFairnessCheckerSymbol);
    TemporalSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
LTLRFairnessCheckerSymbol::getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols)
{
	APPEND_SYMBOL(purposes, symbols, fairnessSymbol);
	APPEND_SYMBOL(purposes, symbols, strongFairTypeSymbol);
	APPEND_SYMBOL(purposes, symbols, weakFairTypeSymbol);
	APPEND_SYMBOL(purposes, symbols, fairnessSetSymbol);
	APPEND_SYMBOL(purposes, symbols, emptyFairnessSetSymbol);
	APPEND_SYMBOL(purposes, symbols, satisfiesSymbol);
	APPEND_SYMBOL(purposes, symbols, actionmatchSymbol);
	APPEND_SYMBOL(purposes, symbols, prooftermSymbol);
	APPEND_SYMBOL(purposes, symbols, enabledSymbol);
	APPEND_SYMBOL(purposes, symbols, counterexampleSymbol);
	TemporalSymbol::getSymbolAttachments(purposes, symbols);
}

void
LTLRFairnessCheckerSymbol::getTermAttachments(Vector<const char*>& purposes, Vector<Term*>& terms)
{
	APPEND_TERM(purposes, terms, deadlockTerm);
	APPEND_TERM(purposes, terms, trueTerm);
	TemporalSymbol::getTermAttachments(purposes, terms);
}

void
LTLRFairnessCheckerSymbol::postInterSymbolPass()
{
	PREPARE_TERM(deadlockTerm);
	PREPARE_TERM(trueTerm);
	TemporalSymbol::postInterSymbolPass();
}

void
LTLRFairnessCheckerSymbol::reset()
{
	deadlockTerm.reset();  // so deadlock dag can be garbage collected
	trueTerm.reset();  // so true dag can be garbage collected
	TemporalSymbol::reset();  // parents reset() tasks
}


bool
LTLRFairnessCheckerSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
    Assert(this == subject->symbol(), "bad symbol");
    FreeDagNode* d = safeCast(FreeDagNode*, subject);

    auto_ptr<RewritingContext> sysContext(context.makeSubcontext(d->getArgument(0)));

    DagNode* resultDag = trueTerm.getDag();

    context.addInCount(*sysContext);
    return context.builtInReplace(subject, resultDag);
}

}
