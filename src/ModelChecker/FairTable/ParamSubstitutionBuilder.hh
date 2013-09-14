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
#include "PropTable/ParamPropositionTable.hh"
#include "PropSet/ParamPropSet.hh"
#include "ParamVarInfo.hh"

namespace modelChecker {

class ParamSubstitutionBuilder: private ParamVarInfo
{
public:
	ParamSubstitutionBuilder(DagNode* fairnessDag, const set<int>& propIds, const ParamPropositionTable& propTable);

	vector<map<int,int>> generateRealizedSubstitutions(const ParamPropSet& pps) const;	// return: propId |-> a substitution id in ParamPropTable (NONE for bot)

private:
	struct PropVarInfo
	{
		PropVarInfo(int propId, const ParamPropositionTable& propTable, const ParamVarInfo& varInfo);
		int propId;
		vector<int> varMap;			// local var id -> global var id
	};

	void computeParamSubstitutions(vector<unique_ptr<PropVarInfo>>::const_iterator pos,
			ParamSubstitution& subst, map<int,int>& propIdMap, const ParamPropSet& pps, vector<map<int,int>>& result) const;
	void init(const set<int>& propIds);

	vector<unique_ptr<PropVarInfo>> pidInfo;		// an "ordered" list of "param" prop ids
	const ParamPropositionTable& propTable;
};

} /* namespace modelChecker */
#endif /* PARAMSUBSTITUTIONBUILDER_HH_ */
