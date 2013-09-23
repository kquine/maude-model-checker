/*
 * FairProductAutomaton.cc
 *
 *  Created on: Sep 10, 2013
 *      Author: kquine
 */

//	utility stuff
#include <memory>
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "temporal.hh"

//	interface class definitions
#include "buchiAutomaton2.hh"
#include "genBuchiAutomaton.hh"

//	automaton definitions
#include "Graph/FairStateSystemGraph.hh"
#include "Graph/FairStateEventSystemGraph.hh"
#include "Graph/FairStateEventEnabledSystemGraph.hh"
#include "FairTable/FormulaFairnessTable.hh"
#include "FairSet/CompositeFairSet.hh"
#include "FairProductAutomaton.hh"

namespace modelChecker {


template <typename SA, typename PA>
FairProductAutomaton<SA,PA>::FairProductAutomaton(unique_ptr<SA>&& system, unique_ptr<PA>&& property, unique_ptr<AbstractFairnessTable>&& systemFairTable):
	ProductAutomaton<SA,PA>(move(system),move(property)),
	fairTable(makeInitFairTable(move(systemFairTable))),
	formulaRef(&static_cast<FormulaFairnessTable&>(fairTable->getComponent(fairTable->nrComponents()-1))) {}


template <typename SA, typename PA>
unique_ptr<FairSet>
FairProductAutomaton<SA,PA>::makeFairSet(const Transition& t)
{
	unique_ptr<FairSet> sysf = this->getSystemAutomaton().makeFairSet(t.source.first, t.systemIndex);
	unique_ptr<FairSet> forf = formulaRef->makeFairSet(t.propertyIndex);
	if (CompositeFairSet* cfs = dynamic_cast<CompositeFairSet*>(sysf.get()))
	{
		cfs->addComponent(move(forf));
		return move(sysf);
	}
	else
	{
		cfs = new CompositeFairSet;
		cfs->addComponent(move(sysf));
		cfs->addComponent(move(forf));
		return unique_ptr<FairSet>(cfs);
	}
}

template <typename SA, typename PA>
CompositeFairnessTable*
FairProductAutomaton<SA,PA>::makeInitFairTable(unique_ptr<AbstractFairnessTable>&& systemFairTable) const
{
	CompositeFairnessTable* result;
	if (dynamic_cast<CompositeFairnessTable*>(systemFairTable.get()))
	{
		result = static_cast<CompositeFairnessTable*>(systemFairTable.release());
	}
	else
	{
		result = new CompositeFairnessTable;
		result->addComponent(move(systemFairTable));
	}
	result->addComponent(unique_ptr<AbstractFairnessTable>(new FormulaFairnessTable(this->getPropertyAutomaton())));
	return result;
}


//
// transition iterator specializations
//
template <typename PL,typename FL>
struct ProductTransitionIteratorTraits<FairStateSystemGraph<PL,FL>>:
	public StateOnlyProductTransitionIteratorTraits<FairStateSystemGraph<PL,FL>> {};

template <typename FL>
struct ProductTransitionIteratorTraits<FairStateEventSystemGraph<StatePropLabel,FL>>:
	public StateOnlyProductTransitionIteratorTraits<FairStateEventSystemGraph<StatePropLabel,FL>> {};

template <typename FL>
struct ProductTransitionIteratorTraits<FairStateEventSystemGraph<EventPropLabel,FL>>:
	public EventOnlyProductTransitionIteratorTraits<FairStateEventSystemGraph<EventPropLabel,FL>> {};

template <typename FL>
struct ProductTransitionIteratorTraits<FairStateEventSystemGraph<StateEventPropLabel,FL>>:
	public StateEventProductTransitionIteratorTraits<FairStateEventSystemGraph<StateEventPropLabel,FL>> {};

template <typename FL>
struct ProductTransitionIteratorTraits<FairStateEventEnabledSystemGraph<StatePropLabel,FL>>:
	public StateOnlyProductTransitionIteratorTraits<FairStateEventEnabledSystemGraph<StatePropLabel,FL>> {};

template <typename FL>
struct ProductTransitionIteratorTraits<FairStateEventEnabledSystemGraph<EventPropLabel,FL>>:
	public EventOnlyProductTransitionIteratorTraits<FairStateEventEnabledSystemGraph<EventPropLabel,FL>> {};

template <typename FL>
struct ProductTransitionIteratorTraits<FairStateEventEnabledSystemGraph<StateEventPropLabel,FL>>:
	public StateEventProductTransitionIteratorTraits<FairStateEventEnabledSystemGraph<StateEventPropLabel,FL>> {};

} /* namespace modelChecker */
