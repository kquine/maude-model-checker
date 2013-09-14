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
	typedef map<int,set<int> > ParamMatchMap;
public:
	ParamPropSet(const ParamPropositionTable& propTable, const PropSet& base);

	void setTrue(int propId);
	void setTrue(const PropSet& ps);

	void setTrueParamSubst(int propId, const set<int>& substIds);
	const set<int>& getTrueParamSubst(int propId) const;
	const ParamPropositionTable& getPropTable() const;

	void dump(ostream& s);

private:
	ParamMatchMap trueParamSubstIds;	// paramId |-> a set of its realized substitution ids

	const ParamPropositionTable& propTable;
	static const set<int> emptySet;
};

} /* namespace modelChecker */
#endif /* PARAMPROPSET_HH_ */
