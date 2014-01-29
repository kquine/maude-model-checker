/*
 * BaseLabelGraph.hh
 *
 *  Created on: Sep 29, 2013
 *      Author: kquine
 */

#ifndef BASELABELGRAPH_HH_
#define BASELABELGRAPH_HH_
#include "PropTable/PropositionTable.hh"

namespace modelChecker {

template <typename T> struct SystemLabelTraits;

template <class T>
class BaseLabelGraph
{
	using PropLabel = typename SystemLabelTraits<T>::PropLabel;
public:
	BaseLabelGraph(unique_ptr<PropLabel>&& pl, const PropositionTable& propTable);

	bool satisfiesStateFormula(Bdd formula, unsigned int stateNr) const;
	bool satisfiesStateEventFormula(Bdd formula, unsigned int stateNr, unsigned int transitionNr) const;
	pair<bool,Bdd> satisfiesPartialStateFormula(Bdd formula, unsigned int stateNr) const;

private:
	const unique_ptr<PropLabel> propLabel;
	const PropositionTable& propTable;
};

} /* namespace modelChecker */

#include "BaseLabelGraph.cc"

#endif /* BASELABELGRAPH_HH_ */
