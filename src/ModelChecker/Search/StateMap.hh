/*
 * StateMap.hh
 *
 *  Created on: Nov 30, 2010
 *      Author: kquine
 */

#ifndef STATEMAP_HH_
#define STATEMAP_HH_
#include <tr1/unordered_map>
#include "DataStruct/PtrVector.hh"
#include "Automaton/ProductAutomaton.hh"

namespace modelChecker {

//
//	map of (systemNr, propertyNr) |-> T, where systemNr's are consecutive..
//
template <class T>
class StateMap
{
	typedef tr1::unordered_map<int,T> IntMap;
public:
	bool invalid(const product::State& s) const;
	bool expand(const product::State& s);			// should be called first
	bool contains(const product::State& s) const;
	void set(const product::State& s, const T& n);
	void remove(const product::State& s);
	const T& get(const product::State& s) const;
private:
	// NOTE: we assume that stateNr is successively increased, so we use Vector.
	PtrVector<IntMap> map;		// systemNr -> propertyNr -> dfs_number
};

}

#include "StateMap.cc"

#endif /* STATEMAP_HH_ */
