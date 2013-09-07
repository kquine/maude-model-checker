/*
 * ModelCheckerFactory.hh
 *
 *  Created on: Aug 6, 2013
 *      Author: kquine
 */

#ifndef MODELCHECKERFACTORY_HH_
#define MODELCHECKERFACTORY_HH_
#include "logicFormula.hh"
#include "ModelChecker.hh"
#include "Graph/SystemGraph.hh"
#include "Graph/DagGraphMap.hh"
#include "Formula/PropositionTable.hh"
#include "Formula/PropChecker.hh"
#include "Formula/EnabledPropTransferer.hh"
#include "Fairness/FairnessTable.hh"
#include "Fairness/FairnessChecker.hh"
#include "Fairness/CompositeFairnessChecker.hh"
#include "Interface/ProofTermGenerator.hh"

namespace modelChecker {

class ModelCheckerFactory
{
public:
	struct Formula { LogicFormula data;  int top;  NatSet formulaPropIds; };

	ModelCheckerFactory(PropositionTable& propTable, FairnessTable& fairTable, const Formula& formula,
			const PropEvaluator& stateEval, const PropEvaluator& eventEval, ProofTermGenerator& ptg, RewritingContext& context);
	virtual ~ModelCheckerFactory() {}

	ModelChecker& getModelChecker();
	const DagGraphMap& getDagGraphMap() const;

private:
	void createPropCheckers();
	void createFairnessCheckers();
	void createSystemGraph();
	void createModelChecker();

	template <typename FC, typename PFC>
	unique_ptr<FairnessChecker> createFairnessChecker(const vector<int>& gSFids, const vector<int>& gSEFids, const vector<int>& pSFids, const vector<int>& pSEFids) const;

	unique_ptr<SystemGraph> graph;
	unique_ptr<ModelChecker> mc;
	DagGraphMap* dagMap;

	unique_ptr<PropChecker> statePropChecker;
	unique_ptr<PropChecker> eventPropChecker;
	unique_ptr<EnabledPropTransferer> enalbedPropTransferer;

	unique_ptr<FairnessChecker> stateFairChecker;
	unique_ptr<FairnessChecker> eventFairChecker;

	PropositionTable& propTable;
	FairnessTable& fairTable;
	const Formula& formula;

	const PropEvaluator& stateEval;
	const PropEvaluator& eventEval;
	ProofTermGenerator& pGenerator;
	RewritingContext& context;
};

template <typename FC, typename PFC>
inline unique_ptr<FairnessChecker>
ModelCheckerFactory::createFairnessChecker(const vector<int>& gSFids, const vector<int>& gSEFids, const vector<int>& pSFids, const vector<int>& pSEFids) const
{
	unique_ptr<FairnessChecker> sfc, sefc;
	if (!gSFids.empty()  || !pSFids.empty())	sfc.reset(pSFids.empty()   ? new PFC(pSFids,gSFids,fairTable)   : new FC(gSFids,fairTable));
	if (!gSEFids.empty() || !pSEFids.empty())	sefc.reset(pSEFids.empty() ? new PFC(pSEFids,gSEFids,fairTable) : new FC(gSEFids,fairTable));

	if (sfc && sefc)	return unique_ptr<FairnessChecker>(new CompositeFairnessChecker{sfc.release(),sefc.release()});
	else if (sfc)		return sfc;
	else if (sefc)		return sefc;
	else				return nullptr;
}

} /* namespace modelChecker */
#endif /* MODELCHECKERFACTORY_HH_ */
