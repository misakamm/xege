/* vim: set tabstop=4 shiftwidth=4: */
#pragma once

#define NULL_PTR 0

namespace ege {

typedef long sbt_int_t;

template<typename VAL>
class SBT {
protected:
	struct SBT_Node {
		SBT_Node*   l;
		SBT_Node*   r;
		sbt_int_t   size;
		VAL         val;
		SBT_Node(): l(NULL_PTR), r(NULL_PTR), size(1) {
		}
	};
	typedef SBT_Node  NodeType;
public:
	typedef SBT_Node* TreeType;
	SBT() : _root(NULL_PTR) {
	}
	SBT(const SBT& sbt) : _root(NULL_PTR) {
		_copy(_root, sbt.root());
	}
	~SBT() {
		_clear(root());
	}

	TreeType&
	root() {
		return _root;
	}
	const TreeType&
	root() const {
		return _root;
	}
	void
	clear() {
		_clear(root());
	}
	sbt_int_t
	size() const {
		return _size(root());
	}
	SBT&
	operator = (const SBT& sbt) {
		_copy(root(), sbt.root());
	}

protected:
	void
	_clear(TreeType& t) {
		if (t == NULL_PTR)
			return ;
		_clear(t->l);
		_clear(t->r);
		delete t;
		t = NULL_PTR;
	}
	void
	_copy(TreeType &t1, const TreeType &t2) {
		if (t1) {
			_clear(t1);
		}
		if (t2) {
			t1 = new NodeType;
			t1->size = t2->size;
			t1->val = t2->val;
			_copy(t1->l, t2->l);
			_copy(t1->r, t2->r);
		}
	}
	sbt_int_t
	_size(const TreeType& t) const {
		if (t) return t->size;
		return 0;
	}
	// face to: //
	//    t     //
	//   / \    //
	//  a   k   //
	//     / \  //
	//    b   c //
	// trans to //
	//    k     //
	//   / \    //
	//  t   c   //
	// / \      //
	//a   b     //
	inline void
	_left_rotate(TreeType &t) {
		TreeType k = t->r;
		t->r = k->l;
		k->l = t;
		k->size = t->size;
		t->size = _size(t->l)
				+ _size(t->r)
				+ 1;
		t = k;
	}
	// face to: //
	//    t     //
	//   / \    //
	//  k   c   //
	// / \      //
	//a   b     //
	// trans to //
	//    k     //
	//   / \    //
	//  a   t   //
	//     / \  //
	//    b   c //
	inline void
	_right_rotate(TreeType &t) {
		TreeType k = t->l;
		t->l = k->r;
		k->r = t;
		k->size = t->size;
		t->size = _size(t->l)
				+ _size(t->r)
				+ 1;
		t = k;
	}
	void
	_maintain(TreeType& t, bool flag) {
		if (t == NULL_PTR)
			return;
		if (!flag) {
			if ( t->l && t->l->l
					&&(!t->r || t->l->l->size > t->r->size) )
				_right_rotate(t);
			else if ( t->l && t->l->r
					&&(!t->r || t->l->r->size > t->r->size) ) {
				_left_rotate(t->l);
				_right_rotate(t);
			} else
				return;
		} else {
			if ( t->r && t->r->r
					&&(!t->l || t->r->r->size > t->l->size) ) {
				_left_rotate(t);
			} else if ( t->r && t->r->l
					&&(!t->l || t->r->l->size > t->l->size) ) {
				_right_rotate(t->r);
				_left_rotate(t);
			} else
				return;
		}
		_maintain(t->l, false);
		_maintain(t->r, true);
		_maintain(t, false);
		_maintain(t, true);
	}
	const TreeType
	_search(const TreeType t, const VAL & val) const {
		if (t == NULL_PTR)
			return NULL_PTR;
		if (val == t->val)
			return t;
		if (val < t->val)
			return _search(t->l, val);
		else
			return _search(t->r, val);
	}
	sbt_int_t
	_lower_bound(const TreeType t, const VAL & val) const {
		if (t == NULL_PTR)
			return 0;
		// val <= t->val
		if ( !( t->val < val) ) {
			return _lower_bound(t->l, val);
		} else {
			sbt_int_t s = _lower_bound(t->r, val);
			return s + (_size(t->l) + 1);
		}
	}
	sbt_int_t
	_upper_bound(const TreeType t, const VAL & val) const {
		if (t == NULL_PTR)
			return 0;
		// val < t->val
		if ( val < t->val ) {
			return _lower_bound(t->l, val);
		} else {
			sbt_int_t s = _lower_bound(t->r, val);
			return s + (_size(t->l) + 1);
		}
	}
	const TreeType
	_pred(const TreeType t, const VAL & val) const {
		if (t == NULL_PTR)
			return NULL_PTR;
		// val <= t->val
		if ( !(t->val < val) )
			return _pred(t->l, val);
		else {
			const TreeType s = _pred(t->r, val);
			return (s ? s : t);
		}
	}
	const TreeType
	_succ(const TreeType t, const VAL & val) const {
		if (t == NULL_PTR)
			return NULL_PTR;
		// val >= t->val
		if ( !(val < t->val) )
			return _succ(t->r, val);
		else {
			const TreeType s = _succ(t->l, val);
			return (s ? s : t);
		}
	}
	TreeType
	_insert(TreeType& t, const VAL & val) {
		if (t == NULL_PTR) {
			t = new NodeType;
			t->val = val;
			t->size = 1;
			return t;
		} else {
			TreeType ret;
			if (val < t->val)
				ret = _insert(t->l, val);
			else
				ret = _insert(t->r, val);
			//if (ret)
			{
				t->size++;
				_maintain(t, !(val < t->val));
			}
			return ret;
		}
	}
	TreeType
	_remove_select(TreeType& t, sbt_int_t rank) {
		if (t == NULL_PTR || (t->size <= rank) )
			return NULL_PTR;
		sbt_int_t lsize = _size(t->l);
		t->size -= 1;
		if (lsize == rank) {
			TreeType del = t;
			if ( !t->l || !t->r ) {
				t = ( t->l ? t->l : t->r );
			} else {
				del = _remove_select(t->l, lsize - 1);
				t->val = del->val;
			}
			return del;
		} else {
			TreeType ret;
			if ( lsize > rank ) {
				ret = _remove_select(t->l, rank);
			} else {
				ret = _remove_select(t->r, rank - lsize - 1);
			}
			_maintain(t, false);
			_maintain(t, true);
			return ret;
		}
	}
	TreeType
	_remove(TreeType& t, const VAL & val) {
		if (t == NULL_PTR)
			return NULL_PTR;
		t->size -= 1;
		if (val == t->val) {
			TreeType del = t;
			if ( !t->l || !t->r ) {
				t = ( t->l ? t->l : t->r );
			} else {
				del = _remove_select(t->l, t->l->size - 1);
				t->val = del->val;
			}
			return del;
		} else {
			TreeType ret;
			if (val < t->val) {
				ret = _remove(t->l, val);
			} else {
				ret = _remove(t->r, val);
			}
			_maintain(t, false);
			_maintain(t, true);
			return ret;
		}
	}
	const TreeType
	_select(const TreeType t, sbt_int_t rank) const {
		if (t == NULL_PTR || (t->size <= rank) )
			return NULL_PTR;
		sbt_int_t lsize = _size(t->l);
		if (lsize == rank)
			return t;
		else if ( lsize > rank )
			return _select(t->l, rank);
		else
			return _select(t->r, rank - lsize - 1);
	}
	sbt_int_t
	_rank(const TreeType t, const VAL & val) const {
		if (t == NULL_PTR)
			return -1;
		if (t->val == val)
			return _size(t->l);
		else if ( val < t->val )
			return _rank(t->l, val);
		else {
			sbt_int_t r = _rank(t->r, val);
			if (r >= 0)
				return r + (_size(t->l) + 1);
			return -1;
		}
	}
public:
	TreeType
	search(const VAL & val) {
		return _search(root(), val);
	}
	TreeType
	pred(const VAL & val) {
		return const_cast<TreeType>(_pred(root(), val));
	}
	TreeType
	succ(const VAL & val) {
		return const_cast<TreeType>(_succ(root(), val));
	}
	TreeType
	insert(const VAL & val) {
		return _insert(root(), val);
	}
	bool
	remove_select(sbt_int_t rank) {
		TreeType del = _remove_select(root(), rank);
		if (del) {
			delete del;
			return true;
		}
		return false;
	}
	bool
	remove(const VAL & val) {
		TreeType del = _remove(root(), val);
		if (val) {
			delete del;
			return true;
		}
		return false;
	}
	const TreeType
	select(sbt_int_t rank) const {
		return _select(root(), rank);
	}
	TreeType
	select(sbt_int_t rank) {
		return const_cast<TreeType>(_select(root(), rank));
	}
	sbt_int_t
	rank(const VAL & val) const {
		return _rank(root(), val);
	}
protected:
	TreeType _root;
};

}
