/*
 * FairnessDecoder.hh
 *
 *  Created on: Dec 4, 2010
 *      Author: kquine
 */

#ifndef FAIRNESSDECODER_HH_
#define FAIRNESSDECODER_HH_
#include "logicFormula.hh"

namespace modelChecker {

class FairnessDecoder
{
public:
	enum FairnessType	{ STRONG_FAIRNESS, WEAK_FAIRNESS };
	struct Fairness		{ FairnessType type;  DagNode* prem;  DagNode* cons; };

	FairnessDecoder(Symbol* fairnessSymbol, Symbol* strongFairTypeSymbol, Symbol* weakFairTypeSymbol, Symbol* fairnessSetSymbol, Symbol* emptyFairnessSetSymbol);

	Fairness decodeFairnessDag(DagNode* fairnessDag) const;
	bool interpreteFairnessSet(DagNode* fairessSetDag, std::function<int(DagNode*)> func) const;	// return true if there is no problem

private:

    Symbol* fairnessSymbol;
    Symbol* strongFairTypeSymbol;
    Symbol* weakFairTypeSymbol;
    Symbol* fairnessSetSymbol;
    Symbol* emptyFairnessSetSymbol;
};

}

#endif /* FAIRNESSDECODER_HH_ */
