/*
 * ProductAutomaton.cc
 *
 *  Created on: Dec 1, 2010
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
#include "Graph/StateSystemGraph.hh"
#include "ProductAutomaton.hh"

namespace modelChecker {

template <typename SA, typename PA>
ProductAutomaton<SA,PA>::ProductAutomaton(unique_ptr<SA>&& system, unique_ptr<PA>&& property):
	systemAut(move(system)), propertyAut(move(property))
{
	for(auto i : propertyAut->getInitialStates())
		initialStates.emplace_back(0,i);
}

template <typename SA, typename PA>
inline unique_ptr<typename ProductAutomaton<SA,PA>::TransitionIterator>
ProductAutomaton<SA,PA>::makeTransitionIterator(const State& state)
{
	TransitionIterator* ti = new TransitionIterator(*systemAut, *propertyAut, state);
	ti->init();
	return unique_ptr<TransitionIterator>(ti);
}

//
// an iterator for state-only graphs (formula, not fairness)
//
template <typename SA>
struct StateOnlyProductTransitionIteratorTraits
{
	template <typename PA>
	void computeNextTransition(bool first, typename AutomatonTraits<PA>::Transition& tr, const typename AutomatonTraits<PA>::PropertyTransitionSet& ts, SA& ks)
	{
		switch(first)
		{
		case 1:
			for (tr.propertyIndex = ts.begin(); tr.propertyIndex != ts.end(); ++tr.propertyIndex)
			{
				if (ks.satisfiesStateFormula(AutomatonTraits<PA>::getFormula(*tr.propertyIndex), tr.source.first))
				{
					for (tr.systemIndex = 0; ; ++tr.systemIndex)
					{
						tr.target.first = ks.getNextState(tr.source.first, tr.systemIndex);
						if (tr.target.first == NONE)
							break;
						tr.target.second = AutomatonTraits<PA>::getNextState(*tr.propertyIndex);
						return;
		default:;
					}
				}
			}
			/* no break */
		}
		tr.systemIndex = NONE;
		tr.target = make_pair(NONE,NONE);
	}
};


//
// an iterator for state/event graphs (formula, not fairness)
//
template <typename SA>
struct StateEventProductTransitionIteratorTraits
{
	Bdd label;	// temporary label

	template <typename PA>
	void computeNextTransition(bool first, typename AutomatonTraits<PA>::Transition& tr, const typename AutomatonTraits<PA>::PropertyTransitionSet& ts, SA& ks)
	{
		switch(first)
		{
		case 1:
			for (tr.propertyIndex = ts.begin(); tr.propertyIndex != ts.end(); ++tr.propertyIndex)
			{
				pair<bool,Bdd> test = ks.satisfiesStateFormula(AutomatonTraits<PA>::getFormula(*tr.propertyIndex), tr.source.first);

				if (test.first)
				{
					label = test.second;
					for (tr.systemIndex = 0; ; ++tr.systemIndex)
					{
						tr.target.first = ks->getNextState(tr.source.systemAut, tr.systemIndex);
						if (tr.target.first == NONE)
							break;

						// check state/event props
						if (ks.satisfiesStateEventFormula(label, tr.source.systemAut, tr.systemIndex))
						{
							tr.target.property = AutomatonTraits<PA>::getNextState(*tr.propertyIndex);
							return;
			default:;
						}
					}
				}
			}
			/* no break */
		}
		tr.systemIndex = NONE;
		tr.target = make_pair(NONE,NONE);
	}
};


//
// transition iterator specializations
//
template <>
struct ProductTransitionIteratorTraits<StateSystemGraph>: StateOnlyProductTransitionIteratorTraits<StateSystemGraph> {};



}
