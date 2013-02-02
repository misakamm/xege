/*
* EGE (Easy Graphics Engine)
* filename  egecontrolbase.cpp

egeControlBase基类定义和相关操作
*/
#ifndef _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
#define _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
#endif
#ifndef _ALLOW_RUNTIME_LIBRARY_MISMATCH
#define _ALLOW_RUNTIME_LIBRARY_MISMATCH
#endif

#pragma warning(disable: 4786)

#include <algorithm>
//#include <set>
//#include <vector>

#include "ege_head.h"

namespace ege {

template<typename T>
class Array
{
public:
	typedef T *iterator;
	class reverse_iterator {
	public:
		reverse_iterator(iterator it) {
			_it = it;
		}
		reverse_iterator(const reverse_iterator &rit) {
			_it = rit._it;
		}
		reverse_iterator& operator ++ () {
			--_it;
			return *this;
		}
		reverse_iterator& operator -- () {
			++_it;
			return *this;
		}
		T& operator* () {
			return *_it;
		}
		bool operator == (const reverse_iterator &rit) {
			return _it == rit._it;
		}
		bool operator != (const reverse_iterator &rit) {
			return _it != rit._it;
		}
	private:
		iterator _it;
	};
public:
	Array() {
		m_capacity = 0;
		m_size = 0;
		m_arr = NULL;
	}
	Array(const Array& arr) {
		m_capacity = arr.m_capacity;
		m_size = arr.m_size;
		m_arr = new T[m_size];;
		for (size_t i = 0; i < m_size; ++i) {
			m_arr[i] = arr.m_arr[i];
		}
	}
	~Array() {
		if (m_arr) {
			delete m_arr;
			m_arr = NULL;
		}
	}
	void resize(size_t sz, T c = T()) {
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
	iterator begin() {
		return m_arr;
	}
	iterator end() {
		return m_arr + m_size;
	}
	reverse_iterator rbegin() {
		return reverse_iterator(m_arr + m_size - 1);
	}
	reverse_iterator rend() {
		return reverse_iterator(m_arr - 1);
	}
	size_t size() const {
		return m_size;
	}
	T& front() {
		return m_arr[0];
	}
	T& back() {
		return m_arr[m_size - 1];
	}
	Array& push_back(const T& obj) {
		if (m_arr == NULL) {
			resize(8);
		} else if (m_size == m_capacity) {
			resize(m_capacity * 2);
		}
		m_arr[m_size++] = obj;
		return *this;
	}
	void pop_back() {
		if (m_size > 0)
			--m_size;
	}
	iterator erase ( iterator position ) {
		if (position == end())
			return position;
		iterator it = position, it2 = position;
		++it2;
		for ( ; it2 != end(); ++it, ++it2) {
			*it = *it2;
		}
		--m_size;
		return position;
	}
	iterator insert ( iterator position, const T& val ) {
		size_t pos = position - m_arr;
		if (m_arr == NULL) {
			resize(8);
		} else if (m_size == m_capacity) {
			resize(m_capacity * 2);
		}
		iterator it = end(), it2 = end();
		position = m_arr + pos;
		if (it2 == position) {
			//
		} else {
			--it;
			for ( ; it2 != position; --it, --it2) {
				*it2 = *it;
			}
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
	iterator begin() {
		return m_set.begin();
	}
	iterator end() {
		return m_set.end();
	}
	reverse_iterator rbegin() {
		return m_set.rbegin();
	}
	reverse_iterator rend() {
		return m_set.rend();
	}
	iterator find(const T& obj) {
		return std::find(m_set.begin(), m_set.end(), obj);
	}
	void insert(const T& obj) {
		typename Array<T>::iterator it = std::lower_bound(m_set.begin(), m_set.end(), obj);
		m_set.insert(it, obj);
	}
	iterator erase(iterator it) {
		return m_set.erase(it);
	}
	iterator erase(const T& obj) {
		return m_set.erase(find(obj));
	}
protected:
	Array<T> m_set;
};


//typedef std::set<egeControlBase*> egectlmap;
typedef Set<egeControlBase*> egectlmap;
//typedef std::vector<egeControlBase*> egectlvec;
typedef Array<egeControlBase*> egectlvec;

int egeControlBase::s_maxchildid = 1024;

static egectlvec s_egeCtlParent;

egeControlBase::InitObject::InitObject(egeControlBase* pThis, int inherit_level) {
	m_this = pThis;
	m_inherit_level = inherit_level;
}
egeControlBase::InitObject::~InitObject() {
	if (m_this->m_inheritlevel == m_inherit_level) {
		m_this->initok();
	}
}
egeControlBase::egeControlBase() {
	init(NULL);
}

egeControlBase::egeControlBase(int inherit, egeControlBase* pParent) {
	if (s_egeCtlParent.size() > 0) {
		if (pParent == 0) {
			pParent = *s_egeCtlParent.rbegin();
		}
	}
	{
		m_inheritlevel = inherit;
		s_egeCtlParent.push_back(this);
	}
	init(pParent);
}

void egeControlBase::initok() {
	s_egeCtlParent.pop_back();
}

egeControlBase::~egeControlBase() {
	if (m_parent) {
		m_parent->delchild(this);
		egectlmap*& cmap = (egectlmap*&)m_childmap;
		if (cmap) {
			egectlmap::iterator it = cmap->begin(); // 以后要附加排序
			for ( ; it != cmap->end(); it++) {
				m_parent->addchild(*it);
			}
		}
	}
	delimage( m_mainbuf );
	delimage( m_mainFilter );
}

void
egeControlBase::init(egeControlBase* parent) {
	struct _graph_setting * pg = &graph_setting;
	egeControlBase* &root = pg->egectrl_root;
	m_parent = NULL;
	m_mainbuf   = newimage();
	m_mainFilter = newimage();
	if (root == NULL) {
		root = this;
		m_parent = NULL;

		m_bVisable  = 0;
		m_bEnable   = 0;
		m_bAutoDraw = 0;
		m_bDirectDraw = 1;

		m_zOrder    = 0;
		m_w = getwidth();
		m_h = getheight();
	} else {
		if (parent) {
			parent->addchild(this);
			m_parent = parent;
		} else {
			root->addchild(this);
			m_parent = root;
		}

		m_bVisable  = 1;
		m_bEnable   = 1;
		m_bAutoDraw = 1;
		m_bDirectDraw = 0;

		m_zOrder    = m_parent->allocZorder(); // 待处理
		m_w = m_h = 1;
	}
	m_zOrderLayer = 0;
	m_allocId = 0x10000;
	m_allocZorder = 1;
	m_bCapture  = 0;
	m_bCapMouse = 0;
	m_bInputFocus = 0;
	m_childmap = NULL;
	m_childzorder = NULL;

	m_x = m_y = 0;
	m_rop = SRCCOPY;
	m_AlphablendMode = 0;
}

int egeControlBase::allocZorder() {
	if (m_allocZorder > 0x800000) {
		fixzorder();
	}
	return m_allocZorder++;
}

int egeControlBase::allocId() {
	return ++m_allocId;
}

bool ctlcmp(const egeControlBase* pa, const egeControlBase* pb) {
	return *pa < *pb;
}

void egeControlBase::sortzorder() {
	egectlvec*& cvec = (egectlvec*&)m_childzorder;
	if (cvec) {
		std::sort(cvec->begin(), cvec->end(), ctlcmp);
	}
}

int egeControlBase::addchild(egeControlBase* pChild) {
	egectlmap*& cmap = (egectlmap*&)m_childmap;
	egectlvec*& cvec = (egectlvec*&)m_childzorder;
	if (cmap == NULL ) {
		cmap = new egectlmap;
		cvec = new egectlvec;
	}
	if (pChild->m_parent) {
		pChild->m_parent->delchild(pChild);
	}
	++s_maxchildid;
	cmap->insert(pChild);
	cvec->push_back(pChild);
	sortzorder();
	pChild->m_parent = this;
	pChild->m_zOrder = allocZorder();
	onAddChild(pChild);
	return 0;
}

int egeControlBase::delchild(egeControlBase* pChild) {
	egectlmap*& cmap = (egectlmap*&)m_childmap;
	egectlvec*& cvec = (egectlvec*&)m_childzorder;
	if (cmap == NULL ) {
		return 0;
	}
	egectlmap::iterator it = cmap->find(pChild);
	if (it != cmap->end()) {
		egectlvec::iterator itv = cvec->begin();
		for ( ; itv != cvec->end(); itv++) {
			if (*itv == *it) break;
		}
		onDelChild(*it);
		cmap->erase(it);
		pChild->m_parent = NULL;
		if (itv != cvec->end()) {
			cvec->erase(itv);
			sortzorder();
		}
		return 1;
	}
	return 0;
}

void egeControlBase::fixzorder() {
	egectlmap*& cmap = (egectlmap*&)m_childmap;
	egectlvec*& cvec = (egectlvec*&)m_childzorder;
	if (cmap) {
		int z = 1;
		for (egectlvec::iterator it = cvec->begin() ; it != cvec->end(); it++) {
			(*it)->m_zOrder = z;
			z++;
		}
		m_allocZorder = z;
	}
}

void egeControlBase::zorderup() {
	m_zOrder = m_parent->allocZorder();
	parent()->sortzorder();
}

void egeControlBase::zorderdown() {
	m_zOrder = -m_parent->allocZorder();
	parent()->sortzorder();
}

void egeControlBase::zorderset(int z) {
	m_zOrder = z;
	parent()->sortzorder();
}

void egeControlBase::mouse(int x, int y, int flag) {
	struct _graph_setting * pg = &graph_setting;
	int ret = 0;
	x -= m_x, y -= m_y;
	{
		PushTarget _target;
		settarget(buf());
		ret = onMouse(x, y, flag);
	}
	if (ret == 0) {
		egectlmap*& cmap = (egectlmap*&)m_childmap;
		egectlvec*& cvec = (egectlvec*&)m_childzorder;
		if (cmap) {
			egectlvec vec = *cvec;
			egectlvec::reverse_iterator it = vec.rbegin();
			for ( ; it != vec.rend(); ++it) {
				if ((*it)->iscapmouse()) {
					(*it)->mouse(x, y, flag);
					break;
				}
			}
			if (it == vec.rend()) it = vec.rbegin();
			for ( ; it != vec.rend(); ++it) {
				egeControlBase* pc = *it;
				if (!pc->isvisable() || !pc->isenable()) continue;
				if (x >= pc->getx() && y >= pc->gety() &&
					x < pc->getx() + pc->getw() && y < pc->gety() + pc->geth() )
				{
					if (pc->m_AlphablendMode == 0 || pc->isdirectdraw() || getpixel(x - pc->getx(), y - pc->gety(), pc->filter())) {
						if ((flag & mouse_msg_down)) {
							int ret = pc->onGetFocus();
							if (ret == 0) {
								pc->capture(true);
								pc->m_zOrder = allocZorder();
								sortzorder();
								if (pg->egectrl_focus && pg->egectrl_focus != pc && pg->egectrl_focus != pc->parent()) {
									for (egeControlBase* pcb = pg->egectrl_focus; pcb != pc && pcb->parent(); pcb = pcb->parent()) {
										pcb->onLostFocus();
										pcb->capture(false);
									}
								}
								pg->egectrl_focus = pc;
							}
						}
						pc->mouse(x, y, flag);
						break;
					} else {
						continue;
					}
				}
			}
		}
	}
}

void egeControlBase::keymsgdown(unsigned key, int flag) {
	int ret = 0;
	{
		PushTarget _target;
		settarget(buf());
		ret = onKeyDown((int)key, flag);
	}
	if (ret == 0) {
		egectlmap*& cmap = (egectlmap*&)m_childmap;
		if (cmap) {
			for (egectlmap::reverse_iterator it = cmap->rbegin() ; it != cmap->rend(); ++it) {
				if (!(*it)->isvisable() || !(*it)->isenable()) continue;
				if ((*it)->iscapture()) {
					(*it)->keymsgdown(key, flag);
				}
			}
		}
	}
}

void egeControlBase::keymsgup(unsigned key, int flag) {
	int ret = 0;
	{
		PushTarget _target;
		settarget(buf());
		onKeyUp((int)key, flag);
	}
	if (ret == 0) {
		egectlmap*& cmap = (egectlmap*&)m_childmap;
		if (cmap) {
			for (egectlmap::reverse_iterator it = cmap->rbegin() ; it != cmap->rend(); ++it) {
				if (!(*it)->isvisable() || !(*it)->isenable()) continue;
				if ((*it)->iscapture()) {
					(*it)->keymsgup(key, flag);
				}
			}
		}
	}
}

void egeControlBase::keymsgchar(unsigned key, int flag) {
	int ret = 0;
	{
		PushTarget _target;
		settarget(buf());
		onKeyChar((int)key, flag);
	}
	if (ret == 0) {
		egectlmap*& cmap = (egectlmap*&)m_childmap;
		if (cmap) {
			for (egectlmap::reverse_iterator it = cmap->rbegin() ; it != cmap->rend(); ++it) {
				if (!(*it)->isvisable() || !(*it)->isenable()) continue;
				if ((*it)->iscapture()) {
					(*it)->keymsgchar(key, flag);
				}
			}
		}
	}
}

void egeControlBase::update() {
	egectlmap*& cmap = (egectlmap*&)m_childmap;
	if (m_parent == NULL) {
		m_w = getwidth();
		m_h = getheight();
	}
	if (cmap) {
		for (egectlmap::iterator it = cmap->begin() ; it != cmap->end(); ++it) {
			(*it)->update();
		}
	}
	PushTarget _target;
	settarget(buf());
	onUpdate();
}

void egeControlBase::draw(PIMAGE pimg) {
	PIMAGE pmain = m_mainbuf;
	if (m_parent == NULL || m_bDirectDraw)
		pmain = pimg;
	{
		PushTarget _target;
		settarget(pmain);
		onDraw(pmain);
	}
	egectlmap*& cmap = (egectlmap*&)m_childmap;
	egectlvec*& cvec = (egectlvec*&)m_childzorder;
	if (cmap) {
		for (egectlvec::iterator it = cvec->begin() ; it != cvec->end(); it++) {
			(*it)->draw(pmain);
		}
	}
	if (!m_bDirectDraw && m_bVisable) {
		if (m_AlphablendMode == SOLIDCOPY) {
			//putimage(pimg, m_x, m_y, &m_mainbuf, m_rop);
			m_mainbuf->putimage(pimg, m_x, m_y, m_rop);
		} else if (m_AlphablendMode == ALPHABLEND) {
			m_mainbuf->putimage_alphafilter(pimg, m_x, m_y, m_mainFilter);
		}
	}
}

} // namespace ege
