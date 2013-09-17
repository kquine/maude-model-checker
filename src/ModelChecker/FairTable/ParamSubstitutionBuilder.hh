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
	ParamSubstitutionBuilder(DagNode* fairnessDag, const set<unsigned int>& propIds, const ParamPropositionTable& propTable);

	NatSet trueParamProps(const ParamPropSet& pps, const ParamSubstitution& subst) const;	// returns a true param props for a given param substitution
	set<ParamSubstitution> generateParamSubstitutions(const ParamPropSet& pps) const;		// returns a set of param substitutions (NONE for bot)

private:
	struct PropVarInfo
	{
		PropVarInfo(unsigned int propId, const ParamPropositionTable& propTable, const ParamVarInfo& varInfo);
		const unsigned int propId;
		vector<unsigned int> varMap;	// local var id -> global var id
	};

	void computeParamSubstitutions(vector<unique_ptr<PropVarInfo>>::const_iterator pos, ParamSubstitution& t, const ParamPropSet& pps, set<ParamSubstitution>& result) const;

	void dumpParamSubst(const ParamSubstitution& s) const;
	void dumpPropSubst(unsigned int propId, const ParamSubstitution& s) const;

	vector<unique_ptr<PropVarInfo>> pidInfo;		// an "ordered" list of "param" prop ids
	const ParamPropositionTable& propTable;

	DagNode* fairDag;	//FIXME: only for test purpose
};

} /* namespace modelChecker */
#endif /* PARAMSUBSTITUTIONBUILDER_HH_ */
