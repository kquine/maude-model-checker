/*
 * FairnessDecoder.cc
 *
 *  Created on: Dec 4, 2010
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
#include "higher.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "dagArgumentIterator.hh"

//		temporal class definitions
#include "logicFormula.hh"

//		higher class definitions
#include "stateTransitionGraph.hh"

//		core class definitions
#include "symbolMap.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//		built in class definitions
#include "bindingMacros.hh"

#include "FairnessDecoder.hh"


namespace modelChecker {

FairnessDecoder::FairnessDecoder():
		fairnessSymbol(NULL), fairnessSetSymbol(NULL), emptyFairnessSetSymbol(NULL),
		strongFairTypeSymbol(NULL), weakFairTypeSymbol(NULL) {}

Vector<FairnessDecoder::Fairness>
FairnessDecoder::interpreteFairnessSet(DagNode* fairnessSetDag) const
{
	Vector<Fairness> result;
	if (fairnessSetDag->symbol() == fairnessSymbol)
		result.append(interpreteFairnessDag(fairnessSetDag));
	else if (fairnessSetDag->symbol() == fairnessSetSymbol)
	{
		DagArgumentIterator fi(fairnessSetDag);
		while (fi.valid())
		{
			result.append(interpreteFairnessDag(fi.argument()));
			fi.next();
		}
	}
	else
		Assert(fairnessSetDag->symbol() == emptyFairnessSetSymbol,
				"FairnessDecoder::interpreteFairnessSet: invalid fairness set");
	return result;
}

FairnessDecoder::Fairness
FairnessDecoder::interpreteFairnessDag(DagNode* fairnessDag) const
{
	Assert(fairnessDag->symbol() == fairnessSymbol,
			"FairnessDecoder::interpreteFairnessDag: invalid fairness formula");
	FreeDagNode* d = safeCast(FreeDagNode*, fairnessDag);
	FairnessType t = d->getArgument(0)->symbol() == strongFairTypeSymbol ? STRONG_FAIRNESS : WEAK_FAIRNESS;
	Fairness res = { t, d->getArgument(1), d->getArgument(2) };
	return res;
}


Bdd
FairnessDecoder::translateFormula(int subformulaIndex, const LogicFormula& formula) const
{
	switch (formula.getOp(subformulaIndex))
	{
	case LogicFormula::PROPOSITION:	return ithvar(formula.getProp(subformulaIndex));
	case LogicFormula::LTL_TRUE:	return bdd_true();
	case LogicFormula::LTL_FALSE:	return bdd_false();
	case LogicFormula::NOT:			return nithvar(formula.getProp(formula.getArg(subformulaIndex, 0)));
	case LogicFormula::AND:			return translateFormula(formula.getArg(subformulaIndex,0), formula)
										 & translateFormula(formula.getArg(subformulaIndex,1), formula);
	case LogicFormula::OR:			return translateFormula(formula.getArg(subformulaIndex,0), formula)
										 | translateFormula(formula.getArg(subformulaIndex,1), formula);
	default:						CantHappen("Fairness formula cannot contains temporal operator");
	}
}


bool
FairnessDecoder::attachSymbol(const char* purpose, Symbol* symbol)
{
    BIND_SYMBOL(purpose, symbol, fairnessSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, strongFairTypeSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, weakFairTypeSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, fairnessSetSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, emptyFairnessSetSymbol, Symbol*);
    return false;
}

void
FairnessDecoder::copyAttachments(FairnessDecoder* orig, SymbolMap* map)
{
	COPY_SYMBOL(orig, fairnessSymbol, map, Symbol*);
	COPY_SYMBOL(orig, strongFairTypeSymbol, map, Symbol*);
	COPY_SYMBOL(orig, weakFairTypeSymbol, map, Symbol*);
	COPY_SYMBOL(orig, fairnessSetSymbol, map, Symbol*);
	COPY_SYMBOL(orig, emptyFairnessSetSymbol, map, Symbol*);
}

void
FairnessDecoder::getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols)
{
    APPEND_SYMBOL(purposes, symbols, fairnessSymbol);
    APPEND_SYMBOL(purposes, symbols, strongFairTypeSymbol);
    APPEND_SYMBOL(purposes, symbols, weakFairTypeSymbol);
    APPEND_SYMBOL(purposes, symbols, fairnessSetSymbol);
    APPEND_SYMBOL(purposes, symbols, emptyFairnessSetSymbol);
}


}
