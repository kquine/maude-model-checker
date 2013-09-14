/*
 * ModelCheckerManager.cc
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
#include "Automaton/FairProductAutomaton.hh"
#include "PropTable/ParamPropositionTable.hh"
#include "PropChecker/PropCheckerFactory.hh"
#include "FairTable/ParamFairnessTable.hh"
#include "FairChecker/FairnessCheckerFactory.hh"
#include "Search/NDFSModelChecker.hh"
#include "ModelCheckerManager.hh"

namespace modelChecker {

ModelCheckerManager::ModelCheckerManager(Formula& formula, PropositionTable& propTable, unique_ptr<AbstractFairnessTable> fairTable,
		const PropEvaluator& stateEval, const PropEvaluator& eventEval, ProofTermGenerator& ptg, RewritingContext& sysCxt):
				formula(formula), propTable(propTable), stateEval(stateEval), eventEval(eventEval), pGenerator(ptg), sysContext(sysCxt)
{
	statePropChecker = PropCheckerFactory::createChecker([&](int i){ return propTable.isStateProp(i); }, propTable, stateEval, sysContext);
	eventPropChecker = PropCheckerFactory::createChecker([&](int i){ return propTable.isEventProp(i); }, propTable,eventEval,sysContext);
	enalbedPropTransferer = PropCheckerFactory::createTransferer([&](int i){ return ! formula.formulaPropIds.contains(i); }, propTable);

	stateFairChecker = FairnessCheckerFactory::createChecker(true, fairTable.get());
	eventFairChecker = FairnessCheckerFactory::createChecker(false, fairTable.get());

	createSystemGraph(move(fairTable));
}


void
ModelCheckerManager::createSystemGraph(unique_ptr<AbstractFairnessTable> fairTable)
{
	if ( ! eventPropChecker )  // no event (and enabled) props
	{
		if ( stateFairChecker )
		{
			auto sg = makeGraph<FairStateSystemGraph>()(sysContext,*statePropChecker,formula.formulaPropIds, *stateFairChecker,pGenerator);
			mc = makeFairModelChecker(move(sg), move(fairTable));
		}
		else
		{
			auto sg = makeGraph<StateSystemGraph>()(sysContext, *statePropChecker, pGenerator);
			mc = makeNDFSModelChecker(move(sg));
		}
	}
	else if ( ! enalbedPropTransferer)  // no enabled props
	{
		if (statePropChecker->nrProps() > 0)
		{

		}
		else  // only event props
		{

		}
		//    StateEventSystemGraph<LabelPropHandler,LabelPropHandler> s3(*sysCxt, stateChecker, eventChecker, prGenerator);
		//    FairStateEventSystemGraph<LabelPropHandler,LabelPropHandler,LabelFairHandler,LabelFairHandler> s4(*sysCxt, stateChecker, eventChecker, swfc, sewfc, prGenerator);
	}
	else
	{
		//    StateEventEnabledSystemGraph<LabelPropHandler,LabelPropHandler> s5(*sysCxt, stateChecker, eventChecker, prGenerator);
		//    FairStateEventEnabledSystemGraph<LabelPropHandler,LabelPropHandler,LabelFairHandler,LabelFairHandler>
		//			s6(*sysCxt, stateChecker, eventChecker, enpc, swfc, sewfc, prGenerator);
	}
}


} /* namespace modelChecker */
