/*
 * ParamPropositionTable.hh
 *
 *  Created on: Aug 23, 2013
 *      Author: kquine
 */

#ifndef PARAMPROPOSITIONTABLE_HH_
#define PARAMPROPOSITIONTABLE_HH_
#include <map>
#include "pattern.hh"
#include "PropTable/PropositionTable.hh"
#include "ParamSubstitution.hh"

namespace modelChecker {

class ParamPropositionTable: public PropositionTable
{
public:
	explicit ParamPropositionTable(const PropInterpreter& pi);

	bool hasParamProp() const override;
	bool isParamProp(int propId) const override;

	int getParamNrVars(int propId) const;
	Term* getParamVariable(int propId, int varId) const;

	const ParamSubstitution& getParamSubst(int propId, int substId) const;
	const map<int,set<int> >* getParamMatches(int propId) const;

	int insertInstanceAndUpdate(DagNode* propDag, RewritingContext& parentContext);

private:
	struct ParamInfo
	{
		explicit ParamInfo(Term* t): pattern(new Pattern(t,false)) {}

		const unique_ptr<Pattern> pattern;
		vector<unique_ptr<ParamSubstitution> > substitutions;
	};

	struct InstancePropInfo: public PropInfo
	{
		InstancePropInfo(const PropInfo& opi): PropInfo(opi) {}
		map<int,set<int> > matchingPropNSubstIds;
	};

	struct ParamPropInfo: public PropInfo
	{
		ParamPropInfo(const PropInfo& opi, unsigned int paramInfoId): PropInfo(opi), paramInfoId(paramInfoId) {}
		const unsigned int paramInfoId;
	};

	void updatePropInfo(int propId);
	void computeMatchingProps(DagNode* propDag, RewritingContext& parentContext, map<int,set<int> >& match);

	const ParamPropInfo* getParamPropInfo(int propId) const;
	const InstancePropInfo* getInstancePropInfo(int propId) const;

	vector<unique_ptr<ParamInfo>> paramInfoTable;
	map<const Symbol*,Vector<int> > paramPropSymbolMap;		// top symbol |-> a set of the corresponding param prop ids
};

inline const ParamPropositionTable::ParamPropInfo*
ParamPropositionTable::getParamPropInfo(int propId) const
{
	return dynamic_cast<const ParamPropInfo*>(propInfoTable[propId].get());
}

inline const ParamPropositionTable::InstancePropInfo*
ParamPropositionTable::getInstancePropInfo(int propId) const
{
	return dynamic_cast<const InstancePropInfo*>(propInfoTable[propId].get());
}

} /* namespace modelChecker */
#endif /* PARAMPROPOSITIONTABLE_HH_ */
