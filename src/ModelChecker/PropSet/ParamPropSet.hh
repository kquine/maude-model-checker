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
	using ParamMatchMap =  map<int,set<int>>;
public:
	ParamPropSet(const ParamPropositionTable& propTable, PropSet&& base);

	void setTrue(int propId) override;
	void setTrue(const PropSet& ps) override;

	void setTrueParamSubst(int propId, const set<int>& substIds);
	const set<int>& getTrueParamSubst(int propId) const;
	const ParamPropositionTable& getPropTable() const;

	void dump(ostream& s) override;

private:
	ParamMatchMap trueParamSubstIds;	// paramId |-> a set of its realized substitution ids

	const ParamPropositionTable& propTable;
	static const set<int> emptySet;
};

} /* namespace modelChecker */
#endif /* PARAMPROPSET_HH_ */
