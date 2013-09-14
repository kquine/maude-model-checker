/*
 * StateMap.hh
 *
 *  Created on: Nov 30, 2010
 *      Author: kquine
 */

#ifndef STATEMAP_HH_
#define STATEMAP_HH_
#include <memory>
#include <unordered_map>

namespace modelChecker {

//
//	map of (systemNr, propertyNr) |-> T, where systemNr's are consecutive..
//
template <class T>
class StateMap
{
	typedef unordered_map<int,T> 	IntMap;
	typedef pair<int,int>			State;
public:
	bool contains(const State& s) const;
	void set(const State& s, const T& n);
	void remove(const State& s);
	const T& get(const State& s) const;
private:
	// NOTE: we use vector, assuming that stateNr is successively increased
	vector<unique_ptr<IntMap> > map;		// systemNr -> propertyNr -> dfs_number
};

}

#include "StateMap.cc"

#endif /* STATEMAP_HH_ */
