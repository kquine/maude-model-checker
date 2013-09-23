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
#include "GraphLabel/StateFairLabel.hh"
#include "GraphLabel/EventFairLabel.hh"
#include "GraphLabel/StateEventFairLabel.hh"
#include "PropTable/ParamPropositionTable.hh"
#include "PropChecker/PropCheckerFactory.hh"
#include "FairTable/ParamFairnessTable.hh"
#include "FairChecker/FairnessCheckerFactory.hh"
#include "Search/NDFSModelChecker.hh"
#include "Search/StreettModelChecker.hh"
#include "Search/SCCBuchiModelChecker.hh"
#include "Utility/StringStream.hh"
#include "ModelCheckerManager.hh"

//#define TDEBUG

namespace modelChecker {

ModelCheckerManager::ModelCheckerManager(Formula& f, PropositionTable& props, unique_ptr<AbstractFairnessTable>&& fairTable,
	const PropEvaluator& sev, const PropEvaluator& eev, const ProofTermGenerator& ptg, RewritingContext& sysCxt):
			formula(f), propTable(props), stateEval(sev), eventEval(eev), pGenerator(ptg), sysContext(sysCxt)
{
	for (int i = 0; i < props.cardinality(); ++i)
	{
		if (props.isStateProp(i))	stateProps.push_back(i);
		if (props.isEventProp(i))	eventProps.push_back(i);
		if (props.isEnabledProp(i))	enabledProps.push_back(i);
	}
	statePC = PropCheckerFactory::createChecker(stateProps, props, sev, sysContext);
	eventPC = PropCheckerFactory::createChecker(eventProps, props, eev, sysContext);
	stateFC = FairnessCheckerFactory::createChecker(true, fairTable.get());
	eventFC = FairnessCheckerFactory::createChecker(false, fairTable.get());
#ifdef TDEBUG
	cout << "#Prop Checkers: " << bool(statePC) << ", " << bool(eventPC) << endl;
	cout << "#Fair Checkers: " << bool(stateFC) << ", " << bool(eventFC) << endl;
#endif
	initModelChecker(move(fairTable));
}

//
// First, choose a prop labeler; TODO: need to create a labeler according to actual formula props (also, "extra" props should be set later)...
//
void
ModelCheckerManager::initModelChecker(unique_ptr<AbstractFairnessTable>&& fairTable)
{
	if ( !stateProps.empty() && !eventProps.empty() )
	{
		initModelChecker(unique_ptr<StateEventPropLabel>(new StateEventPropLabel(formula.formulaPropIds,*statePC,*eventPC, true)), move(fairTable));
	}
	else if ( !stateProps.empty() )	// only state props
	{
		initModelChecker(unique_ptr<StatePropLabel>(new StatePropLabel(formula.formulaPropIds,*statePC)), move(fairTable));
	}
	else if ( !eventProps.empty() )	// only event props
	{
		initModelChecker(unique_ptr<EventPropLabel>(new EventPropLabel(formula.formulaPropIds,*eventPC)), move(fairTable));
	}
	else
	{
		throw invalid_argument("no propositions are provided.");
	}
}

//
// Next, choose a fair labeler
//
template <typename PL> void
ModelCheckerManager::initModelChecker(unique_ptr<PL>&& pl, unique_ptr<AbstractFairnessTable>&& fairTable)
{
	if ( stateFC && eventFC )
		initModelChecker(move(pl), unique_ptr<StateEventFairLabel>(new StateEventFairLabel(*stateFC,*eventFC)), move(fairTable));
	else if (stateFC)
		initModelChecker(move(pl), unique_ptr<StateFairLabel>(new StateFairLabel(*stateFC)), move(fairTable));
	else if (eventFC)
		initModelChecker(move(pl), unique_ptr<EventFairLabel>(new EventFairLabel(*eventFC)), move(fairTable));
	else
		initModelChecker(move(pl), unique_ptr<StateFairLabel>(nullptr), move(fairTable));
}

//
// Finally, choose a graph type and create a model checker
//
template <typename PL, typename FL> void
ModelCheckerManager::initModelChecker(unique_ptr<PL>&& pl, unique_ptr<FL>&& fl, unique_ptr<AbstractFairnessTable>&& fTable)
{
	if ( eventProps.empty() )
	{
		fl ? makeMC<FairStateSystemGraph>(move(fTable),move(pl),move(fl)) : makeMC<StateSystemGraph>(move(pl));
	}
	else if ( enabledProps.empty() )
	{
		fl ? makeMC<FairStateEventSystemGraph>(move(fTable),move(pl),move(fl)) : makeMC<StateEventSystemGraph>(move(pl));
	}
	else
	{
		unique_ptr<EnabledPropTransferer> enph = PropCheckerFactory::createTransferer(enabledProps,formula.formulaPropIds, propTable);
		fl ? makeMC<FairStateEventEnabledSystemGraph>(move(fTable),move(pl),move(fl),move(enph)) : makeMC<StateEventEnabledSystemGraph>(move(pl),move(enph));
	}
}


template <template <typename> class Graph, typename PL, typename... Args> void
ModelCheckerManager::makeMC(unique_ptr<PL>&& pl, Args&&... args)
{
	unique_ptr<Graph<PL>> sysGraph(new Graph<PL>(move(pl), std::forward<Args>(args)..., sysContext, pGenerator));
	sysGraph->init();
	dagGraph = sysGraph.get();	// need to be set before move

	using Prod = ProductAutomaton<Graph<PL>,BuchiAutomaton2>;
	unique_ptr<Prod> prod(new Prod(move(sysGraph), unique_ptr<BuchiAutomaton2>(new BuchiAutomaton2(&formula.data,formula.top))));

	Verbose("ModelChecker: Use the NDFS algorithm with a Buchi automaton (" << prod->getPropertyAutomaton().getNrStates() << " states).");
	modelChecker.reset(new NDFSModelChecker<BuchiAutomaton2>(move(prod)));
}


template <template <typename,typename> class Graph, typename PL, typename FL, typename... Args> void
ModelCheckerManager::makeMC(unique_ptr<AbstractFairnessTable>&& fairTable, unique_ptr<PL>&& pl, unique_ptr<FL>&& fl, Args&&... args)
{
	unique_ptr<Graph<PL,FL>> sysGraph(new Graph<PL,FL>(move(pl), move(fl), std::forward<Args>(args)..., sysContext, pGenerator));
	sysGraph->init();
	dagGraph = sysGraph.get();	// need to be set before move

	unique_ptr<GenBuchiAutomaton> property(new GenBuchiAutomaton(&formula.data,formula.top));
	property->simplify();

	using Prod = FairProductAutomaton<Graph<PL,FL>,GenBuchiAutomaton>;
	unique_ptr<Prod> prod(new Prod(move(sysGraph), move(property), move(fairTable)));

	if (prod->getFairnessTable().hasStrongFairness())
	{
		Verbose("ModelChecker: Use the Streett algorithm with a gen-Buchi automaton (" << prod->getPropertyAutomaton().getNrStates() << " states).");
		modelChecker.reset(new StreettModelChecker<GenBuchiAutomaton>(move(prod)));
	}
	else
	{
		Verbose("ModelChecker: Use the SCC-Buchi algorithm with a gen-Buchi automaton (" << prod->getPropertyAutomaton().getNrStates() << " states).");
		modelChecker.reset(new SCCBuchiModelChecker<GenBuchiAutomaton>(move(prod)));
	}
}


} /* namespace modelChecker */
