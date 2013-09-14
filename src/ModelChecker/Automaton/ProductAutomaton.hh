/*
 * ProductAutomaton.hh
 *
 *  Created on: Nov 23, 2010
 *      Author: kquine
 */

#ifndef PRODUCTAUTOMATON_HH_
#define PRODUCTAUTOMATON_HH_
#include "AutomatonTraits.hh"

namespace modelChecker {

template <typename SystemAutomaton> struct ProductTransitionIteratorTraits;

template <typename SA, typename PA>
class ProductAutomaton
{
public:
	using State = 		typename AutomatonTraits<PA>::State;
	using Transition = 	typename AutomatonTraits<PA>::Transition;
	struct TransitionIterator;

	ProductAutomaton(unique_ptr<SA> system, unique_ptr<PA> property);
	virtual ~ProductAutomaton() {}

	const SA& getSystemAutomaton() const				{ return *systemAut; }
	const PA& getPropertyAutomaton() const				{ return *propertyAut; }
	const vector<State>& getInitialStates() const		{ return initialStates; }

	unique_ptr<TransitionIterator> makeTransitionIterator(const State& state);

private:
	friend class TransitionIterator;

	unique_ptr<SA> systemAut;
	unique_ptr<PA> propertyAut;
	vector<State> initialStates;
};

template <typename SA, typename PA>
struct ProductAutomaton<SA,PA>::TransitionIterator: private ProductTransitionIteratorTraits<SA>
{
public:
	TransitionIterator(SA& sys, const PA& prop, const State& state): sysGraph(sys), ts(prop.getTransitions(state.second)) { tr.source = state; }

	bool hasNext() const					{ return tr.systemIndex != NONE; }
	const Transition& pick() const			{ return tr; }
	const State& getSource() const			{ return tr.source; }

	void init()								{ computeNextTransition(true); }
	void next()								{ computeNextTransition(false); }

protected:
	void computeNextTransition(bool first)	{ ProductTransitionIteratorTraits<SA>::template computeNextTransition<PA>(first,tr,ts,sysGraph); }

	SA& sysGraph;
	const typename AutomatonTraits<PA>::PropertyTransitionSet& ts;
	typename AutomatonTraits<PA>::Transition tr;
};

template <typename SA>
struct ProductTransitionIteratorTraits
{
	template <typename PA>
	void computeNextTransition(bool first, typename AutomatonTraits<PA>::Transition& tr, const typename AutomatonTraits<PA>::PropertyTransitionSet& ts, SA& ks);
};

}

#include "ProductAutomaton.cc"

#endif /* PRODUCTAUTOMATON_HH_ */
