/*
 * ModelCheckerFactory.cc
 *
 *  Created on: Aug 6, 2013
 *      Author: kquine
 */

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "temporal.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//	ltlr definitions
#include "Formula/ParamPropositionTable.hh"
#include "Formula/ParamPropChecker.hh"
#include "Formula/ParamEnabledPropTransferer.hh"
#include "Fairness/ParamFairnessTable.hh"
#include "Fairness/ParamWeakFairnessChecker.hh"
#include "Fairness/ParamStrongFairnessChecker.hh"
#include "ModelCheckerFactory.hh"

namespace modelChecker {

ModelCheckerFactory::ModelCheckerFactory(PropositionTable& propTable, FairnessTable& fairTable, const Formula& formula,
		const PropEvaluator& stateEval, const PropEvaluator& eventEval, ProofTermGenerator& ptg, RewritingContext& context):
				dagMap(nullptr), propTable(propTable), fairTable(fairTable), formula(formula), stateEval(stateEval), eventEval(eventEval), pGenerator(ptg), context(context)
{
	createPropCheckers();
	createFairnessCheckers();
}

ModelChecker&
ModelCheckerFactory::getModelChecker()
{
	return *mc;
}

const DagGraphMap&
ModelCheckerFactory::getDagGraphMap() const
{
	return *dagMap;
}

void
ModelCheckerFactory::createPropCheckers()
{
	vector<int> gSPids, gEPids, gFEPids;	// ground state/event/fair-enabled prop ids
	vector<int> pSPids, pEPids, pFEPids; 	// param state/event/fair-enabled prop ids

	for (int k = 0; k < propTable.cardinality(); ++k)
	{
		if (propTable.isStateProp(k))					(propTable.isParamProp(k) ? pSPids : gSPids).push_back(k);
		else if (propTable.isEventProp(k))				(propTable.isParamProp(k) ? pEPids : gEPids).push_back(k);
		else if (propTable.isEnabledProp(k) &&
				! formula.formulaPropIds.contains(k))	(propTable.isParamProp(k) ? pFEPids : gFEPids).push_back(k);
	}

	statePropChecker.reset(pSPids.empty() ? new PropChecker(gSPids,propTable,stateEval,context) : new ParamPropChecker(gSPids,pSPids,propTable,stateEval,context));
	eventPropChecker.reset(pEPids.empty() ? new PropChecker(gEPids,propTable,eventEval,context) : new ParamPropChecker(gEPids,pEPids,propTable,eventEval,context));
	enalbedPropTransferer.reset(pFEPids.empty() ? new EnabledPropTransferer(gFEPids,propTable) : new ParamEnabledPropTransferer(gFEPids,pFEPids,propTable));
}

void
ModelCheckerFactory::createFairnessCheckers()
{
	vector<int> gSWFids, gSSFids, gSEWFids, gSESFids;	// ground state-weak/state-strong/state-event-weak/state-event-strong fair ids;
	vector<int> pSWFids, pSSFids, pSEWFids, pSESFids;	// param state-weak/state-strong/state-event-weak/state-event-strong fair ids;

	for (int wi = 0; wi < fairTable.nrWeakFairness(); ++wi)
	{
		if (fairTable.isStateWeakFairness(wi))				(fairTable.isParamWeakFairness(wi) ? pSWFids : gSWFids).push_back(wi);
		else if (fairTable.isStateEventWeakFairness(wi))	(fairTable.isParamWeakFairness(wi) ? pSEWFids : gSEWFids).push_back(wi);
	}
	for (int si = 0; si < fairTable.nrStrongFairness(); ++si)
	{
		if (fairTable.isStateStrongFairness(si))			(fairTable.isParamStrongFairness(si) ? pSSFids : gSSFids).push_back(si);
		else if (fairTable.isStateEventStrongFairness(si))	(fairTable.isParamStrongFairness(si) ? pSESFids : gSESFids).push_back(si);
	}

	stateFairChecker = createFairnessChecker<WeakFairnessChecker,ParamWeakFairnessChecker>(gSWFids,gSEWFids,pSWFids,pSEWFids);
	eventFairChecker = createFairnessChecker<StrongFairnessChecker,ParamStrongFairnessChecker>(gSSFids,gSESFids,pSSFids,pSESFids);
}

void
ModelCheckerFactory::createSystemGraph()
{

}

void
ModelCheckerFactory::createModelChecker()
{

}


} /* namespace modelChecker */
