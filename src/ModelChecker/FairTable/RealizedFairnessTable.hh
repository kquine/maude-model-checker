/*
 * RealizedFairnessTable.hh
 *
 *  Created on: Sep 17, 2013
 *      Author: kquine
 */

#ifndef REALIZEDFAIRNESSTABLE_HH_
#define REALIZEDFAIRNESSTABLE_HH_
#include "AbstractFairnessTable.hh"
#include "ParamSubstitutionBuilder.hh"
#include "PropTable/ParamPropositionTable.hh"

namespace modelChecker {

class RealizedFairnessTable
{
public:
	using index_type = AbstractFairnessTable::index_type;

	const ParamSubstitutionBuilder& getSubstBuilder(index_type fairId) const;

	index_type getRealizedFairId(index_type fairId, const NatSet& realizedFair) const;	// find the realized fair id in realizedFair (i.e.,parameter abstraction)
	index_type insertFairnessInstance(index_type paramFairId, const ParamSubstitution& propSubst);

protected:
	using InstanceSubstMap = map<const ParamSubstitution,const index_type>;

	struct ParamInfo
	{
		ParamInfo(DagNode* fDag, const vector<index_type>& propIds, const ParamPropositionTable& pTable): builder(fDag,propIds,pTable) {}
		ParamSubstitutionBuilder builder;
		InstanceSubstMap substMap;				// realized substitution |-> an instance fair id  (its keys are ordered by the lexico-order)
	};

	struct InstanceInfo
	{
		InstanceInfo(index_type pfi, const ParamSubstitution* s): paramFairId(pfi), substRef(s) {}
		const index_type paramFairId;			// the base param fair id
		const ParamSubstitution* substRef;		// the corresponding substitution id
		set<index_type> directBase;				// the set of immediate instance fair ids (in fairTable)
	};

	//
	//	client table interface
	//
	virtual ParamInfo& getParamInfo(index_type fairId) const = 0;
	virtual InstanceInfo& getInstanceInfo(index_type fairId) const = 0;
	virtual index_type getNextFairIndex() const = 0;
	virtual void insertInstance(index_type pfi, const ParamSubstitution* s) = 0;

private:
	void updateInstanceBaseMap(const InstanceSubstMap& substMap, const InstanceSubstMap::const_iterator f);	// update the dependency map when f is newly added
};

} /* namespace modelChecker */
#endif /* REALIZEDFAIRNESSTABLE_HH_ */
