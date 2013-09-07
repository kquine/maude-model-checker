/*
 * SCCBuchiModelChecker.h
 *
 *  Created on: Nov 30, 2010
 *      Author: kquine
 */

#ifndef SCCBUCHIMODELCHECKER_HH_
#define SCCBUCHIMODELCHECKER_HH_
#include "SCCModelChecker.hh"
#include "Automaton/FairnessMap.hh"

namespace modelChecker {

class SCCBuchiModelChecker : public SCCModelChecker
{
public:
	SCCBuchiModelChecker(Automaton& prod, FairnessMap& fm);
	SCCBuchiModelChecker(const SCCBuchiModelChecker&) = delete;
	SCCBuchiModelChecker& operator=(const SCCBuchiModelChecker&) = delete;

private:
	unique_ptr<SCC> findAcceptedSCC(const Vector<State>& initials);
};

}

#endif /* SCCBUCHIMODELCHECKER_H_ */
