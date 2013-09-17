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
	void computeParamSubstitutions(vector<unsigned int>::const_iterator pos, ParamSubstitution& t, const ParamPropSet& pps, set<ParamSubstitution>& result) const;

	void dumpParamSubst(const ParamSubstitution& s) const;
	void dumpPropSubst(unsigned int propId, const ParamSubstitution& s) const;

	vector<unsigned int> initPropVarInfo(unsigned int propId, const ParamPropositionTable& propTable) const;

	vector<unsigned int> paramPids;						// an "ordered" list of "param" prop ids
	map<unsigned int,vector<unsigned int>> varMaps;		// param propId |-> (local var id -> global var id)

	const ParamPropositionTable& propTable;

	DagNode* fairDag;	//FIXME: only for test purpose
};

} /* namespace modelChecker */
#endif /* PARAMSUBSTITUTIONBUILDER_HH_ */
