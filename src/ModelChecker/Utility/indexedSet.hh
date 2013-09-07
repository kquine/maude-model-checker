/*
 * indexedSet.hh
 *
 *  Created on: Aug 29, 2013
 *      Author: kquine
 */

#ifndef INDEXEDSET_HH_
#define INDEXEDSET_HH_
#include <map>
#include <vector>

namespace modelChecker {

/*
 * a newer version of the existing indexed set
 */
template<typename T>
class indexed_set
{
	typedef map<T,unsigned int> EltMap;

public:
	typedef typename EltMap::iterator			iterator;
	typedef typename EltMap::const_iterator		const_iterator;

	indexed_set();
	virtual ~indexed_set();

	indexed_set(indexed_set&&) noexcept;
	indexed_set& operator=(indexed_set&&) noexcept;	// move

	const T& operator[](unsigned int i) const;
	T& operator[](unsigned int i);

	iterator begin();
	const_iterator begin() const noexcept;
	iterator end();
	const_iterator end() const noexcept;

	const_iterator find(const T& element) const;
	iterator find(T& element);

	unsigned int insert(const T& element);
	void swap(indexed_set& other);

	unsigned int size() const;
	void clear();

private:
	EltMap eltMap;
	vector<typename EltMap::const_iterator> indexMap;
};

template<typename T> indexed_set<T>::indexed_set()	{ }
template<typename T> indexed_set<T>::~indexed_set()	{ }

template<typename T>
indexed_set<T>::indexed_set(indexed_set&& other) noexcept:
	eltMap(std::move(other.eltMap)), indexMap(std::move(other.indexMap)) {}

template<typename T> indexed_set<T>&
indexed_set<T>::operator=(indexed_set&& other) noexcept
{
	eltMap = std::move(other.eltMap);
	indexMap = std::move(other.indexMap);
	return *this;
}

template<typename T> const T&
indexed_set<T>::operator[](unsigned int i) const
{
	return indexMap[i]->first;
}

template<typename T> T&
indexed_set<T>::operator[](unsigned int i)
{
	return indexMap[i]->first;
}

template<typename T> typename indexed_set<T>::const_iterator
indexed_set<T>::begin() const noexcept
{
	return eltMap.begin();
}

template<typename T> typename indexed_set<T>::const_iterator
indexed_set<T>::end() const noexcept
{
	return eltMap.end();
}

template<typename T> typename indexed_set<T>::iterator
indexed_set<T>::begin()
{
	return eltMap.begin();
}

template<typename T> typename indexed_set<T>::iterator
indexed_set<T>::end()
{
	return eltMap.end();
}

template<typename T> typename indexed_set<T>::const_iterator
indexed_set<T>::find(const T& element) const
{
	return eltMap.find(element);
}

template<typename T> typename indexed_set<T>::iterator
indexed_set<T>::find(T& element)
{
	return eltMap.find(element);
}

template<typename T> unsigned int
indexed_set<T>::insert(const T& element)
{
	pair<typename EltMap::iterator,bool> p = eltMap.insert(typename EltMap::value_type(element,indexMap.size()));
	if (p.second)
		indexMap.push_back(p.first);
	return p.first->second;
}

template<typename T> void
indexed_set<T>::swap(indexed_set& other)
{
	indexMap.swap(other.indexMap);
	eltMap.swap(other.eltMap);
}

template<typename T> unsigned int
indexed_set<T>::size() const
{
	return indexMap.size();
}

template<typename T> void
indexed_set<T>::clear()
{
	indexMap.clear();
	eltMap.clear();
}


} /* namespace modelChecker */
#endif /* INDEXEDSET_HH_ */
