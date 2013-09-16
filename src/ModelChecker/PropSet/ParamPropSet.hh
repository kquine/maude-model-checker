/*
 * ParamPropSet.hh
 *
 *  Created on: Aug 26, 2013
 *      Author: kquine
 */

#ifndef PARAMPROPSET_HH_
#define PARAMPROPSET_HH_
#include "PropSet.hh"
#include "PropTable/ParamPropositionTable.hh"

namespace modelChecker {

class ParamPropSet: public PropSet
{
	using ParamMatchMap =  map<unsigned int,set<unsigned int>>;
public:
	ParamPropSet(const ParamPropositionTable& propTable, PropSet&& base): PropSet(forward<PropSet>(base)), propTable(propTable)  {}

	bool isParamProp(unsigned int propId) const		{ return propTable.isParamProp(propId); }

	void setTrue(unsigned int propId) override;
	void setTrue(const PropSet& ps) override;

	void setTrueParamSubst(unsigned int propId, const set<unsigned int>& substIds);
	const set<unsigned int>& getTrueParamSubst(unsigned int propId) const;

	void dump(ostream& s) override;

private:
	ParamMatchMap trueParamSubstIds;	// paramId |-> a set of its realized substitution ids

	const ParamPropositionTable& propTable;
	static const set<unsigned int> emptySet;
};

} /* namespace modelChecker */
#endif /* PARAMPROPSET_HH_ */
