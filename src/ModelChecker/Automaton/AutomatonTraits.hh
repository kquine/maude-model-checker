/*
 * AutomatonTraits.hh
 *
 *  Created on: Sep 13, 2013
 *      Author: kquine
 */

#ifndef AUTOMATONTRAITS_HH_
#define AUTOMATONTRAITS_HH_
#include <memory>
#include "buchiAutomaton2.hh"
#include "genBuchiAutomaton.hh"

namespace modelChecker {

template <typename PropertyAutomaton> struct PropertyTransitionTraits;

template <typename PA>
struct AutomatonTraits: public PropertyTransitionTraits<PA>
{
	using State			= pair<int,int>;
	using PropertyIndex	= typename PropertyTransitionTraits<PA>::PropertyTransitionSet::const_iterator;

	struct Transition
	{
		State source, target;
		int systemIndex;
		PropertyIndex propertyIndex;
	};
};

inline ostream& operator<<(ostream &o, const pair<int,int>& state)
{
	o << "(" << state.first << "," <<  state.second << ")";
	return o;
}

//
// property transition specializations
//
template <>
struct PropertyTransitionTraits<BuchiAutomaton2>
{
	using PropertyTransition =		BuchiAutomaton2::Transition;
	using PropertyTransitionSet =	BuchiAutomaton2::TransitionMap;
	static int getNextState(const PropertyTransition& t)		{ return t.first; }
	static const Bdd& getFormula(const PropertyTransition&t)	{ return t.second; }
};

template <>
struct PropertyTransitionTraits<GenBuchiAutomaton>
{
	using PropertyTransition =		GenBuchiAutomaton::FairTransition;
	using PropertyTransitionSet =	GenBuchiAutomaton::FairTransitionSet;
	static int getNextState(const PropertyTransition& t)		{ return t.first.first; }
	static const Bdd& getFormula(const PropertyTransition&t)	{ return t.second; }
};

} /* namespace modelChecker */
#endif /* AUTOMATONTRAITS_HH_ */
