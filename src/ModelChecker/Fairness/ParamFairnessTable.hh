/*
 * ParamFairnessTable.hh
 *
 *  Created on: Sep 3, 2013
 *      Author: kquine
 */

#ifndef PARAMFAIRNESSTABLE_HH_
#define PARAMFAIRNESSTABLE_HH_
#include "Formula/ParamVarInfo.hh"
#include "FairnessTable.hh"

namespace modelChecker {

class ParamFairnessTable: public FairnessTable
{
public:
	ParamFairnessTable(PropositionTable& propTable, const FairnessDecoder& fDecoder, const FormulaBuilder& fBuilder);

	bool isParamWeakFairness(int weakFairId) const;
	bool isParamStrongFairness(int strongFairId) const;

private:
	vector<unique_ptr<ParamVarInfo> > varInfoTable;
};

inline bool
ParamFairnessTable::isParamWeakFairness(int weakFairId) const
{
	/* TODO */
	return false;
}

inline bool
ParamFairnessTable::isParamStrongFairness(int strongFairId) const
{
	/* TODO */
	return false;
}

} /* namespace modelChecker */
#endif /* PARAMFAIRNESSTABLE_HH_ */
