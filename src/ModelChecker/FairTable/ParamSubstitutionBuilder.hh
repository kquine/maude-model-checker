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
	using RealizedSubst =	map<unsigned int,unsigned int>;

	ParamSubstitutionBuilder(DagNode* fairnessDag, const set<unsigned int>& propIds, const ParamPropositionTable& propTable);

	vector<RealizedSubst> generateRealizedSubstitutions(const ParamPropSet& pps) const;	// return: propId |-> a substitution id in ParamPropTable (NONE for bot)

private:
	struct PropVarInfo
	{
		PropVarInfo(unsigned int propId, const ParamPropositionTable& propTable, const ParamVarInfo& varInfo);
		const unsigned int propId;
		vector<unsigned int> varMap;			// local var id -> global var id
	};

	using ParamList = vector<unique_ptr<PropVarInfo>>;

	void computeParamSubsts(ParamList::const_iterator pos, ParamSubstitution& subst, RealizedSubst& rsubst, const ParamPropSet& pps, vector<RealizedSubst>& result) const;
	void init(const set<unsigned int>& propIds);

	ParamList pidInfo;		// an "ordered" list of "param" prop ids
	const ParamPropositionTable& propTable;

	DagNode* fairDag;	//FIXME: only for test purpose
};

} /* namespace modelChecker */
#endif /* PARAMSUBSTITUTIONBUILDER_HH_ */
