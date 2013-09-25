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
#include "PropSetUtil.hh"

namespace modelChecker {

class ParamPropSet: public PropSet
{
	friend class PropSetUtil;
public:
	ParamPropSet(const ParamPropositionTable& propTable, PropSet&& base): PropSet(move(base)), propTable(propTable)  {}

	bool isParamProp(unsigned int propId) const;
	void setTrue(unsigned int propId) override;

	template <typename T> void setTrueParamSubst(unsigned int propId, const T& substs);
	const set<const ParamSubstitution*>& getTrueParamSubst(unsigned int propId) const;

	void dump(ostream& s) override;

protected:
	void setTrue(const PropSet& ps) override;

private:
	map<unsigned int,set<const ParamSubstitution*>> trueParamSubstRefs;	// paramId |-> a set of its realized substitutions

	const ParamPropositionTable& propTable;
	static const set<const ParamSubstitution*> emptySet;
};

} /* namespace modelChecker */
#endif /* PARAMPROPSET_HH_ */
