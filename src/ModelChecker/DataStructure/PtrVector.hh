/*
 * PtrVector.hh
 *
 *  Created on: Dec 10, 2010
 *      Author: kquine
 */

#ifndef PTRVECTOR_HH_
#define PTRVECTOR_HH_
#include <memory>
#include "macros.hh"
#include "vector.hh"

namespace modelChecker {

template <typename T>
class PtrVector
{
	NO_COPYING(PtrVector);	// copying is not allowed.

public:
	typedef typename Vector<T*>::iterator			iterator;
	typedef typename Vector<T*>::const_iterator		const_iterator;
	typedef typename Vector<T*>::size_type			size_type;

	PtrVector()								{ }
	virtual ~PtrVector()					{ clear(); }

	iterator begin()						{ return vec.begin(); }
	iterator end()							{ return vec.end(); }
	const_iterator begin() const			{ return vec.begin(); }
	const_iterator end() const				{ return vec.end(); }

	const T* operator[](size_type i) const	{ return vec[i]; }
	T* operator[](size_type i)				{ return vec[i]; }
	void replace(size_type i, T* x);
	T* move(size_type i);
	void expandTo(size_type newLength, bool alloc = true);
	void contractTo(size_type newLength);
	void append(T* item)					{ vec.append(item); }
	size_type size() const					{ return vec.size(); }
	bool empty() const						{ return vec.empty(); }
	void clear();

private:
	Vector<T*> vec;
};

template <typename T> void
PtrVector<T>::replace(size_type i, T* x)
{
	delete vec[i];
	vec[i] = x;
}

template <typename T> T*
PtrVector<T>::move(size_type i)
{
	T* t = vec[i];
	vec[i] = NULL;
	return t;
}

template <typename T> void
PtrVector<T>::expandTo(size_type newLength, bool alloc)
{
	int old = vec.size();
	vec.expandTo(newLength);
	for (int i = old; i < newLength; ++i)
		vec[i] = alloc ? new T : NULL;
}

template <typename T> void
PtrVector<T>::contractTo(size_type newLength)
{
	for (int i = newLength; i < vec.size(); ++i )
		delete vec[i];
	vec.contractTo(newLength);
}

template <typename T> void
PtrVector<T>::clear()
{
	FOR_EACH_CONST(i, typename Vector<T*>, vec)
		delete (*i);
}



}

#endif /* PTRVECTOR_HH_ */
