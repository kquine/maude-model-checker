/*
 * FairnessDecoder.hh
 *
 *  Created on: Dec 4, 2010
 *      Author: kquine
 */

#ifndef FAIRNESSDECODER_HH_
#define FAIRNESSDECODER_HH_
#include "bddUser.hh"
#include "logicFormula.hh"

namespace modelChecker {

class FairnessDecoder : private BddUser
{
public:
	enum FairnessType {STRONG_FAIRNESS, WEAK_FAIRNESS};
	struct Fairness
	{
		FairnessType type;
		DagNode* prem;
		DagNode* cons;
	};

	FairnessDecoder(Symbol* fairnessSymbol, Symbol* strongFairTypeSymbol, Symbol* weakFairTypeSymbol,
			Symbol* fairnessSetSymbol, Symbol* emptyFairnessSetSymbol);
	Vector<Fairness> interpreteFairnessSet(DagNode* fairessSetDag) const;
	Bdd translateFormula(int subformulaIndex, const LogicFormula& formula) const;

private:
    Fairness interpreteFairnessDag(DagNode* fairnessDag) const;

    Symbol* fairnessSymbol;
    Symbol* strongFairTypeSymbol;
    Symbol* weakFairTypeSymbol;
    Symbol* fairnessSetSymbol;
    Symbol* emptyFairnessSetSymbol;
};

}

#endif /* FAIRNESSDECODER_HH_ */
