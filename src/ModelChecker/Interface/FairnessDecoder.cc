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
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "dagArgumentIterator.hh"

//		temporal class definitions
#include "logicFormula.hh"

//      free theory class definitions
#include "freeDagNode.hh"

// ltlr definitions
#include "FairnessDecoder.hh"


namespace modelChecker {

FairnessDecoder::FairnessDecoder(Symbol* fairnessSymbol, Symbol* strongFairTypeSymbol, Symbol* weakFairTypeSymbol,
		Symbol* fairnessSetSymbol, Symbol* emptyFairnessSetSymbol):
				fairnessSymbol(fairnessSymbol), strongFairTypeSymbol(strongFairTypeSymbol), weakFairTypeSymbol(weakFairTypeSymbol),
				fairnessSetSymbol(fairnessSetSymbol), emptyFairnessSetSymbol(emptyFairnessSetSymbol) {}

bool
FairnessDecoder::interpreteFairnessSet(DagNode* fairnessSetDag, std::function<int(DagNode*)> func) const
{
	if (fairnessSetDag->symbol() == fairnessSymbol)
	{
		func(fairnessSetDag);
		return true;
	}
	else if (fairnessSetDag->symbol() == fairnessSetSymbol)
	{
		for (DagArgumentIterator fi(fairnessSetDag); fi.valid(); fi.next())
			func(fi.argument());
		return true;
	}
	else
	{
		return fairnessSetDag->symbol() == emptyFairnessSetSymbol;
	}
}

FairnessDecoder::Fairness
FairnessDecoder::decodeFairnessDag(DagNode* fairnessDag) const
{
	Assert(fairnessDag->symbol() == fairnessSymbol, "FairnessDecoder::interpreteFairnessDag: invalid fairness formula");
	FreeDagNode* d = safeCast(FreeDagNode*, fairnessDag);
	return { (d->getArgument(0)->symbol() == strongFairTypeSymbol ? STRONG_FAIRNESS : WEAK_FAIRNESS), d->getArgument(1), d->getArgument(2) };
}


}
