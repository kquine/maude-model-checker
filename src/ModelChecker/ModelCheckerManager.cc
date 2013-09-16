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
		const PropEvaluator& stateEval, const PropEvaluator& eventEval, const ProofTermGenerator& ptg, RewritingContext& sysCxt):
				formula(formula), propTable(propTable), stateEval(stateEval), eventEval(eventEval), pGenerator(ptg), sysContext(sysCxt)
{
	statePropChecker = PropCheckerFactory::createChecker([&](unsigned int i){ return propTable.isStateProp(i); }, propTable, stateEval, sysContext);
	eventPropChecker = PropCheckerFactory::createChecker([&](unsigned int i){ return propTable.isEventProp(i); }, propTable,eventEval,sysContext);
	enalbedPropTransferer = PropCheckerFactory::createTransferer([&](unsigned int i){ return propTable.isEnabledProp(i) && !formula.formulaPropIds.contains(i); }, propTable);

	stateFairChecker = FairnessCheckerFactory::createChecker(true, fairTable.get());
	eventFairChecker = FairnessCheckerFactory::createChecker(false, fairTable.get());

	//cout << "PC: " << bool(statePropChecker) << ", " << bool(eventPropChecker) << ", " << bool(enalbedPropTransferer) << endl;
	//cout << "FC: " << bool(stateFairChecker) << ", " << bool(eventFairChecker) << endl;

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
			mc = makeModelChecker(move(sg), move(fairTable));
		}
		else
		{
			auto sg = makeGraph<StateSystemGraph>()(sysContext, *statePropChecker, pGenerator);
			mc = makeModelChecker(move(sg));
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

template <typename SA>
inline unique_ptr<ModelChecker>
ModelCheckerManager::makeModelChecker(unique_ptr<SA>&& sysGraph) const
{
	using Prod = ProductAutomaton<SA,BuchiAutomaton2>;
	unique_ptr<Prod> prod(new Prod(forward<unique_ptr<SA>>(sysGraph), unique_ptr<BuchiAutomaton2>(new BuchiAutomaton2(&formula.data,formula.top))));
	Verbose("ModelChecker: Use the NDFS algorithm with a Buchi automaton (" << prod->getPropertyAutomaton().getNrStates() << " states).");
	return unique_ptr<ModelChecker>(new NDFSModelChecker<Prod>(move(prod)));
}

template <typename SA>
inline unique_ptr<ModelChecker>
ModelCheckerManager::makeModelChecker(unique_ptr<SA>&& sysGraph, unique_ptr<AbstractFairnessTable>&& fairTable) const
{
	using Prod = FairProductAutomaton<SA,GenBuchiAutomaton>;
	unique_ptr<GenBuchiAutomaton> property(new GenBuchiAutomaton(&formula.data,formula.top));
	property->simplify();
	unique_ptr<Prod> prod(new Prod(forward<unique_ptr<SA>>(sysGraph), move(property), forward<unique_ptr<AbstractFairnessTable>>(fairTable)));

	if (prod->getFairnessTable().hasStrongFairness())
	{
		Verbose("ModelChecker: Use the Streett algorithm with a gen-Buchi automaton (" << prod->getPropertyAutomaton().getNrStates() << " states).");
		return unique_ptr<ModelChecker>(new StreettModelChecker<Prod>(move(prod)));
	}
	else
	{
		Verbose("ModelChecker: Use the SCC-Buchi algorithm with a gen-Buchi automaton (" << prod->getPropertyAutomaton().getNrStates() << " states).");
		return unique_ptr<ModelChecker>(new SCCBuchiModelChecker<Prod>(move(prod)));
	}
}




} /* namespace modelChecker */
