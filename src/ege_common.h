#pragma once

//#include <algorithm>

//#define EGE_SBT_SIMPLE
#ifdef EGE_SBT_SIMPLE
#include "ege_sbt_simple.h"
#else
#include "ege_sbt.h"
#endif


namespace ege
{

template<typename T>
class Array
{
public:
	typedef T *iterator;
	class reverse_iterator
	{
	public:
		reverse_iterator(iterator it) {_it = it;}
		reverse_iterator(const reverse_iterator &rit) {_it = rit._it;}

		reverse_iterator& operator ++ ()
		{
			--_it;
			return *this;
		}
		reverse_iterator& operator -- ()
		{
			++_it;
			return *this;
		}

		T& operator* () {return *_it;}

		bool operator == (const reverse_iterator &rit) {return _it == rit._it;}
		bool operator != (const reverse_iterator &rit) {return _it != rit._it;}

	private:
		iterator _it;
	};

public:
	Array()
	{
		m_capacity = 0;
		m_size = 0;
		m_arr = NULL;
	}

	Array(const Array& arr)
	{
		m_capacity = arr.m_capacity;
		m_size = arr.m_size;
		m_arr = new T[m_size];;
		for (size_t i = 0; i < m_size; ++i) {
			m_arr[i] = arr.m_arr[i];
		}
	}

	~Array()
	{
		if (m_arr) {
			delete m_arr;
			m_arr = NULL;
		}
	}

	void resize(size_t sz, T c = T())
	{
		if (m_arr == NULL) {
			m_arr = new T[sz];
			for (size_t i = 0; i < sz; ++i) {
				m_arr[i] = c;
			}
			m_capacity = sz;
			m_size = 0;
		} else {
			T* arr = new T[sz];
			size_t i = 0;
			for (; i < m_size; ++i) {
				arr[i] = m_arr[i];
			}
			for (; i < sz; ++i) {
				arr[i] = c;
			}
			m_capacity = sz;
			delete[] m_arr;
			m_arr = arr;
			if (m_size > m_capacity)
				m_size = m_capacity;
		}
	}

	iterator begin() { return m_arr; }
	iterator end()   { return m_arr + m_size; }

	reverse_iterator rbegin() { return reverse_iterator(m_arr + m_size - 1); }
	reverse_iterator rend()   { return reverse_iterator(m_arr - 1);}

	size_t size() const { return m_size; }

	T& front() { return m_arr[0]; }
	T& back()  { return m_arr[m_size - 1];}

	Array& push_back(const T& obj)
	{
		if (m_arr == NULL) {
			resize(8);
		} else if (m_size == m_capacity) {
			resize(m_capacity * 2);
		}
		m_arr[m_size++] = obj;
		return *this;
	}

	void pop_back()
	{
		if (m_size > 0)
			--m_size;
	}

	iterator erase ( iterator position )
	{
		if (position == end())
			return position;
		iterator it = position, it2 = position;
		for ( ; ++it2 != end(); ++it) {
			*it = *it2;
		}
		--m_size;
		return position;
	}

	iterator insert ( iterator position, const T& val )
	{
		size_t pos = position - m_arr;
		if (m_arr == NULL) {
			resize(8);
		} else if (m_size == m_capacity) {
			resize(m_capacity * 2);
		}
		iterator it = end(), it2 = end();
		position = m_arr + pos;
		for ( ; it2 != position; --it2) {
			*it2 = *--it;
		}
		*it2 = val;
		++m_size;
		return position;
	}
protected:
	size_t m_capacity;
	size_t m_size;
	T* m_arr;
};

#if 0
template<typename T>
class Set
{
public:
	typedef typename Array<T>::iterator iterator;
	typedef typename Array<T>::reverse_iterator reverse_iterator;
public:
	Set() : m_set() {
	}
	~Set() {
	}
	iterator
	begin() {
		return m_set.begin();
	}
	iterator
	end() {
		return m_set.end();
	}
	reverse_iterator
	rbegin() {
		return m_set.rbegin();
	}
	reverse_iterator
	rend() {
		return m_set.rend();
	}
	size_t
	size() const {
		return m_set.size();
	}
	iterator
	find(const T& obj) {
		//return std::find(m_set.begin(), m_set.end(), obj);
		typename Array<T>::iterator it = std::lower_bound(m_set.begin(), m_set.end(), obj);
		if (it != end() && *it == obj)
			return it;
		return end();
	}
	void
	insert(const T& obj) {
		typename Array<T>::iterator it = std::lower_bound(m_set.begin(), m_set.end(), obj);
		if (it == end() || *it != obj)
			m_set.insert(it, obj);
	}
	void
	erase(iterator it) {
		m_set.erase(it);
	}
	void
	erase(const T& obj) {
		m_set.erase(find(obj));
	}
protected:
	Array<T> m_set;
};
#else

template<typename T>
class Set
{
public:
	class iterator
	{
	public:
		iterator(SBT<T>& t, sbt_int_t it)
		{
			_t = &t;
			_it = it;
		}

		iterator& operator ++ ()
		{
			++_it;
			return *this;
		}

		iterator& operator -- ()
		{
			--_it;
			return *this;
		}

		iterator operator + (sbt_int_t i) {return iterator(*_t, _it + i);}
		iterator operator - (sbt_int_t i) {return iterator(*_t, _it - i);}

		T& operator * () {return _t->select(_it)->val;}

		bool operator == (const iterator &it) {return _t == it._t && _it == it._it;}
		bool operator != (const iterator &it) {return _t != it._t || _it != it._it;}

		sbt_int_t index() const {return _it;}

		void erase()
		{
			if (0 <= index() && index() < _t->size()) {
				_t->remove_select(_it);
			}
		}

	protected:
		sbt_int_t _it;
		SBT<T>* _t;
	};

	class reverse_iterator : public iterator
	{
	public:
		reverse_iterator(SBT<T>& t, sbt_int_t it) : iterator(t, it) {}

		reverse_iterator& operator ++ ()
		{
			--iterator::_it;
			return *this;
		}

		reverse_iterator& operator -- ()
		{
			++iterator::_it;
			return *this;
		}

		iterator operator + (sbt_int_t i) {return iterator(*iterator::_t, iterator::_it - i);}
		iterator operator - (sbt_int_t i) {return iterator(*iterator::_t, iterator::_it + i);}

		bool operator == (const reverse_iterator &rit) {return iterator::_t == rit._t && iterator::_it == rit._it;}
		bool operator != (const reverse_iterator &rit) {return iterator::_t != rit._t || iterator::_it != rit._it;}
	};
public:
	Set() : m_set() {}
	~Set() {}

	iterator begin() {return iterator(m_set, 0);}
	iterator end() {return iterator(m_set, m_set.size());}

	reverse_iterator rbegin() {return reverse_iterator(m_set, m_set.size() - 1);}
	reverse_iterator rend() {return reverse_iterator(m_set, -1);}

	iterator nth(sbt_int_t n) {return iterator(m_set, n);}
	sbt_int_t size() const {return m_set.size();}

	iterator find(const T& obj)
	{
		sbt_int_t i = m_set.rank(obj);
		if (i == -1)
			return end();
		return nth(i);
	}

	void insert(const T& obj)
	{
		sbt_int_t i = m_set.rank(obj);
		if (i == -1)
			m_set.insert(obj);
	}

	void erase(iterator it) {it.erase();}
	void erase(reverse_iterator it) {it.erase();}

	void erase(const T& obj)
	{
		if (m_set.search(obj)) {
			m_set.remove(obj);
		}
	}
protected:
	SBT<T> m_set;
};
#endif

}
