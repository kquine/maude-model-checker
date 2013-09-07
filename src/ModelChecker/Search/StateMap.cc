/*
 * StateMap.cc
 *
 *  Created on: Dec 1, 2010
 *      Author: kquine
 */

#include "StateMap.hh"

namespace modelChecker {

template <class T> inline bool
StateMap<T>::invalid(const product::State& s) const
{
	return (size_t)s.system >= map.size();
}

template <class T> inline bool
StateMap<T>::expand(const product::State& s)
{
	if ( (size_t)s.system >= map.size())
	{
		map.resize(s.system + 1);
		return true;
	}
	return false;
}

template <class T> inline bool
StateMap<T>::contains(const product::State& s) const
{
	return map[s.system]->find(s.property) != map[s.system]->end();
}

template <class T> inline void
StateMap<T>::set(const product::State& s, const T& n)
{
	(*map[s.system])[s.property] = n;
}

template <class T> inline void
StateMap<T>::remove(const product::State& s)
{
	map[s.system]->erase(s.property);
}

template <class T> inline const T&
StateMap<T>::get(const product::State& s) const
{
	return map[s.system]->find(s.property)->second;
}

}
