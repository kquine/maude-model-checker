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
		const int propId;
		vector<int> varMap;			// local var id -> global var id
	};

	using ParamPropTable = vector<unique_ptr<PropVarInfo>>;

	ParamPropTable pidInfo;		// an "ordered" list of "param" prop ids
	const ParamPropositionTable& propTable;

	void computeParamSubstitutions(ParamPropTable::const_iterator pos, ParamSubstitution& subst, map<int,int>& propIdMap, const ParamPropSet& pps, vector<map<int,int>>& result) const;
	void init(const set<int>& propIds);
};

} /* namespace modelChecker */
#endif /* PARAMSUBSTITUTIONBUILDER_HH_ */
