/*
 * SystemGraphTraits.hh
 *
 *  Created on: Sep 27, 2013
 *      Author: kquine
 */

#ifndef SYSTEMGRAPHTRAITS_HH_
#define SYSTEMGRAPHTRAITS_HH_
#include "Interface/ProofTermGenerator.hh"
#include "PropChecker/PropSet.hh"

namespace modelChecker {

template <typename T> struct SystemGraphTraits;

struct RuleDagGenerator
{
	RuleDagGenerator(const Rule* rule): rule(rule) {}
	DagNode* makeDag(DagNode* stateDag, RewritingContext& context, const ProofTermGenerator& ptg) const;
private:
	const Rule* rule;
};

struct ProofDagGenerator
{
	ProofDagGenerator(unsigned int index): transitionIndex(index) {}
	DagNode* makeDag(DagNode* stateDag, RewritingContext& context, const ProofTermGenerator& ptg) const;
private:
	const unsigned int transitionIndex;		// to construct a proofterm
};

template <typename T>
struct ptr_compare
{
	bool operator() (const T* a, const T* b) const	{ return (*a) < (*b); }
};

template <typename T>
using ptr_set = set<T*,ptr_compare<T>>;


} /* namespace modelChecker */
#endif /* SYSTEMGRAPHTRAITS_HH_ */
