#pragma once

#include "sbt.h"

namespace ege
{

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

template <typename T> class Set
{
public:
    class iterator
    {
    public:
        iterator(SBT<T>& t, sbt_int_t it)
        {
            _t  = &t;
            _it = it;
        }

        iterator& operator++()
        {
            ++_it;
            return *this;
        }

        iterator& operator--()
        {
            --_it;
            return *this;
        }

        iterator operator+(sbt_int_t i) { return iterator(*_t, _it + i); }

        iterator operator-(sbt_int_t i) { return iterator(*_t, _it - i); }

        T& operator*() { return _t->select(_it)->val; }

        bool operator==(const iterator& it) { return _t == it._t && _it == it._it; }

        bool operator!=(const iterator& it) { return _t != it._t || _it != it._it; }

        sbt_int_t index() const { return _it; }

        void erase()
        {
            if (0 <= index() && index() < _t->size()) {
                _t->remove_select(_it);
            }
        }

    protected:
        sbt_int_t _it;
        SBT<T>*   _t;
    };

    class reverse_iterator : public iterator
    {
    public:
        reverse_iterator(SBT<T>& t, sbt_int_t it) : iterator(t, it) {}

        reverse_iterator& operator++()
        {
            --iterator::_it;
            return *this;
        }

        reverse_iterator& operator--()
        {
            ++iterator::_it;
            return *this;
        }

        iterator operator+(sbt_int_t i) { return iterator(*iterator::_t, iterator::_it - i); }

        iterator operator-(sbt_int_t i) { return iterator(*iterator::_t, iterator::_it + i); }

        bool operator==(const reverse_iterator& rit) { return iterator::_t == rit._t && iterator::_it == rit._it; }

        bool operator!=(const reverse_iterator& rit) { return iterator::_t != rit._t || iterator::_it != rit._it; }
    };

public:
    Set() : m_set() {}

    ~Set() {}

    iterator begin() { return iterator(m_set, 0); }
    iterator end()   { return iterator(m_set, m_set.size()); }

    reverse_iterator rbegin() { return reverse_iterator(m_set, m_set.size() - 1); }
    reverse_iterator rend()  { return reverse_iterator(m_set, -1); }

    iterator nth(sbt_int_t n) { return iterator(m_set, n); }

    sbt_int_t size() const { return m_set.size(); }

    iterator find(const T& obj)
    {
        sbt_int_t i = m_set.rank(obj);
        if (i == -1) {
            return end();
        }
        return nth(i);
    }

    void insert(const T& obj)
    {
        sbt_int_t i = m_set.rank(obj);
        if (i == -1) {
            m_set.insert(obj);
        }
    }

    void erase(iterator it) { it.erase(); }

    void erase(reverse_iterator it) { it.erase(); }

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

