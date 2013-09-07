/*
 * ParamSubstitutionBuilder.hh
 *
 *  Created on: Aug 29, 2013
 *      Author: kquine
 */

#ifndef PARAMSUBSTITUTIONBUILDER_HH_
#define PARAMSUBSTITUTIONBUILDER_HH_
#include "variableDagNode.hh"
#include "Utility/indexedSet.hh"
#include "ParamPropositionTable.hh"
#include "ParamPropSet.hh"
#include "ParamSubstitution.hh"
#include "ParamVarInfo.hh"

namespace modelChecker {

class ParamSubstitutionBuilder: private ParamVarInfo
{
public:
	ParamSubstitutionBuilder(DagNode* fairnessDag, const set<int>& propIds, const ParamPropositionTable& propTable);

	unique_ptr<ParamSubstitution> createParamSubstitution(const ParamPropSet& pps) const;

	void genRealizedSubstitutions(const ParamPropSet& pps) const;

private:
	struct PropVarInfo
	{
		PropVarInfo(int propId, const ParamPropositionTable& propTable, const ParamVarInfo& varInfo);
		int propId;
		vector<int> varMap;			// local var id -> global var id
	};

	void computeParamSubstitutions(list<unique_ptr<PropVarInfo>>::const_iterator pos, ParamSubstitution& subst, map<int,int>& propIdMap, const ParamPropSet& pps) const;
	void init(const set<int>& propIds);

	list<unique_ptr<PropVarInfo>> pidLists;		// an "ordered" list of "param" prop ids
	const ParamPropositionTable& propTable;
};

} /* namespace modelChecker */
#endif /* PARAMSUBSTITUTIONBUILDER_HH_ */
