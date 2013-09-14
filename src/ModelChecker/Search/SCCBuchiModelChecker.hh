/*
 * SCCBuchiModelChecker.h
 *
 *  Created on: Nov 30, 2010
 *      Author: kquine
 */

#ifndef SCCBUCHIMODELCHECKER_HH_
#define SCCBUCHIMODELCHECKER_HH_
#include "SCCModelChecker.hh"

namespace modelChecker {

template <typename Automaton>
class SCCBuchiModelChecker : public SCCModelChecker<Automaton>
{
public:
	SCCBuchiModelChecker(unique_ptr<Automaton> graph);

private:
	using Super 		= SCCModelChecker<Automaton>;
	using State 		= typename Super::State;
	using Transition	= typename Super::Transition;
	using SCC 			= typename Super::SCC;
	using SCCStack		= typename Super::SCCStack;

	unique_ptr<SCC> findAcceptedSCC(const vector<State>& initials) override;
};

}

#include "SCCBuchiModelChecker.cc"

#endif /* SCCBUCHIMODELCHECKER_H_ */
