/* vim: set tabstop=4 shiftwidth=4: */
#pragma once

#define NULL_PTR 0

namespace ege
{

typedef long sbt_int_t;

template <typename VAL> class SBT
{
protected:
    struct SBT_Node
    {
        SBT_Node* l;
        SBT_Node* r;
        SBT_Node* p;
        sbt_int_t size;
        VAL       val;

        SBT_Node() : l(NULL_PTR), r(NULL_PTR), p(NULL_PTR), size(1) {}
    };

    typedef SBT_Node NodeType;

public:
    typedef SBT_Node* TreeType;

    SBT() : _root(NULL_PTR) {}

    SBT(const SBT& sbt) : _root(NULL_PTR) { _copy(NULL_PTR, _root, sbt.root()); }

    ~SBT() { _clear(root()); }

    TreeType& root() { return _root; }

    const TreeType& root() const { return _root; }

    void clear() { root() = _clear(root()); }

    sbt_int_t size() const { return safe_size(root()); }

    SBT& operator=(const SBT& sbt) { _copy(NULL_PTR, root(), sbt.root()); }

protected:
    TreeType _clear(TreeType t)
    {
        if (t == NULL_PTR) {
            return NULL_PTR;
        }
        _clear(left(t));
        _clear(right(t));
        delete t;
        return NULL_PTR;
    }

    void _copy(TreeType p, TreeType t1, const TreeType t2)
    {
        if (t1) {
            _clear(t1);
        }
        if (t2) {
            t1         = new NodeType;
            size(t1)   = size(t2);
            val(t1)    = val(t2);
            parent(t1) = p;
            _copy(t1, left(t1), left(t2));
            _copy(t1, right(t1), right(t2));
        }
    }

    inline sbt_int_t safe_size(const TreeType t) const
    {
        if (t) {
            return size(t);
        }
        return 0;
    }

    inline sbt_int_t& size(TreeType t) { return t->size; }

    inline const sbt_int_t& size(const TreeType t) const { return t->size; }

    inline TreeType& left(TreeType t) { return t->l; }

    inline const TreeType& left(const TreeType t) const { return t->l; }

    inline TreeType& right(TreeType t) { return t->r; }

    inline const TreeType& right(const TreeType t) const { return t->r; }

    inline TreeType& parent(TreeType t) { return t->p; }

    VAL& val(TreeType t) { return t->val; }

    const VAL& val(const TreeType t) const { return t->val; }

    void _set_left(TreeType t, TreeType ch)
    {
        left(t) = ch;
        if (ch) {
            parent(ch) = t;
        }
    }

    void _set_right(TreeType t, TreeType ch)
    {
        right(t) = ch;
        if (ch) {
            parent(ch) = t;
        }
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
    // a   b     //
    inline TreeType _left_rotate(TreeType t)
    {
        TreeType k = right(t);
        _set_right(t, left(k));
        left(k)   = t;
        parent(k) = parent(t);
        parent(t) = k;
        size(k)   = size(t);
        size(t)   = safe_size(left(t)) + safe_size(right(t)) + 1;
        return k;
    }

    // face to: //
    //    t     //
    //   / \    //
    //  k   c   //
    // / \      //
    // a   b     //
    // trans to //
    //    k     //
    //   / \    //
    //  a   t   //
    //     / \  //
    //    b   c //
    inline TreeType _right_rotate(TreeType t)
    {
        TreeType k = left(t);
        _set_left(t, right(k));
        right(k)  = t;
        parent(k) = parent(t);
        parent(t) = k;
        size(k)   = size(t);
        size(t)   = safe_size(left(t)) + safe_size(right(t)) + 1;
        return k;
    }

    TreeType _maintain(TreeType t, bool flag)
    {
        if (t == NULL_PTR) {
            return NULL_PTR;
        }
        if (!flag) {
            if (left(t) && left(left(t)) && (!right(t) || size(left(left(t))) > size(right(t)))) {
                t = _right_rotate(t);
            } else if (left(t) && right(left(t)) && (!right(t) || size(right(left(t))) > size(right(t)))) {
                left(t) = _left_rotate(left(t));
                t       = _right_rotate(t);
            } else {
                return t;
            }
        } else {
            if (right(t) && right(right(t)) && (!left(t) || size(right(right(t))) > size(left(t)))) {
                t = _left_rotate(t);
            } else if (right(t) && left(right(t)) && (!left(t) || size(left(right(t))) > size(left(t)))) {
                right(t) = _right_rotate(right(t));
                t        = _left_rotate(t);
            } else {
                return t;
            }
        }

        left(t)  = _maintain(left(t), false);
        right(t) = _maintain(right(t), true);

        t = _maintain(t, false);
        t = _maintain(t, true);

        return t;
    }

    const TreeType _search(const TreeType t, const VAL& val_) const
    {
        if (t == NULL_PTR) {
            return NULL_PTR;
        }
        if (val_ == val(t)) {
            return t;
        }
        if (val_ < val(t)) {
            return _search(left(t), val_);
        } else {
            return _search(right(t), val_);
        }
    }

    sbt_int_t _lower_bound(const TreeType t, const VAL& val_) const
    {
        if (t == NULL_PTR) {
            return 0;
        }
        // val_ <= val(t)
        if (!(val(t) < val_)) {
            return _lower_bound(left(t), val_);
        } else {
            sbt_int_t s = _lower_bound(right(t), val_);
            return s + (safe_size(left(t)) + 1);
        }
    }

    sbt_int_t _upper_bound(const TreeType t, const VAL& val_) const
    {
        if (t == NULL_PTR) {
            return 0;
        }
        // val < val(t)
        if (val_ < val(t)) {
            return _lower_bound(left(t), val_);
        } else {
            sbt_int_t s = _lower_bound(right(t), val_);
            return s + (safe_size(left(t)) + 1);
        }
    }

    const TreeType _pred(const TreeType t, const VAL& val_) const
    {
        if (t == NULL_PTR) {
            return NULL_PTR;
        }
        // val <= val(t)
        if (!(val(t) < val_)) {
            return _pred(left(t), val_);
        } else {
            const TreeType s = _pred(right(t), val_);
            return (s ? s : t);
        }
    }

    const TreeType _succ(const TreeType t, const VAL& val_) const
    {
        if (t == NULL_PTR) {
            return NULL_PTR;
        }
        // val >= val(t)
        if (!(val_ < val(t))) {
            return _succ(right(t), val_);
        } else {
            const TreeType s = _succ(left(t), val_);
            return (s ? s : t);
        }
    }

    TreeType _insert(TreeType p, TreeType& t, const VAL& val_)
    {
        if (t == NULL_PTR) {
            t         = new NodeType;
            val(t)    = val_;
            size(t)   = 1;
            parent(t) = p;
            return t;
        } else {
            TreeType ret;
            if (val_ < val(t)) {
                ret = _insert(t, left(t), val_);
            } else {
                ret = _insert(t, right(t), val_);
            }
            // if (ret)
            {
                size(t)++;
                t = _maintain(t, !(val_ < val(t)));
            }
            return ret;
        }
    }

    TreeType _remove_select(TreeType* pt, sbt_int_t rank)
    {
        for (;;) {
            TreeType& t = *pt;
            if (t == NULL_PTR || size(t) <= rank) {
                return NULL_PTR;
            }

            sbt_int_t lsize  = safe_size(left(t));
            size(t)         -= 1;
            if (lsize == rank) {
                TreeType del = t;
                if (left(t) == NULL_PTR || right(t) == NULL_PTR) {
                    TreeType p = parent(t);
                    t          = (left(t) ? left(t) : right(t));
                    if (t) {
                        parent(t) = p;
                    }
                } else {
                    del    = _remove_select(&left(t), lsize - 1);
                    val(t) = val(del);
                }
                return del;
            }
            if (lsize > rank) {
                pt = &left(t);
            } else {
                pt   = &right(t);
                rank = rank - lsize - 1;
            }
        }
    }

    TreeType _remove(TreeType* pt, const VAL& val_)
    {
        for (;;) {
            TreeType& t = *pt;
            if (t == NULL_PTR) {
                return NULL_PTR;
            }

            size(t) -= 1;
            if (val_ == val(t)) {
                TreeType del = t;
                if (left(t) == NULL_PTR || right(t) == NULL_PTR) {
                    TreeType p = parent(t);
                    t          = (left(t) ? left(t) : right(t));
                    if (t) {
                        parent(t) = p;
                    }
                } else {
                    del    = _remove_select(&left(t), size(left(t)) - 1);
                    val(t) = val(del);
                }
                return del;
            }
            if (val_ < val(t)) {
                pt = &left(t);
            } else {
                pt = &right(t);
            }
        }
    }

    const TreeType _select(const TreeType t, sbt_int_t rank) const
    {
        if (t == NULL_PTR || (size(t) <= rank)) {
            return NULL_PTR;
        }
        sbt_int_t lsize = safe_size(left(t));
        if (lsize == rank) {
            return t;
        } else if (lsize > rank) {
            return _select(left(t), rank);
        } else {
            return _select(right(t), rank - lsize - 1);
        }
    }

    sbt_int_t _rank(const TreeType t, const VAL& val_) const
    {
        if (t == NULL_PTR) {
            return -1;
        }
        if (val(t) == val_) {
            return safe_size(left(t));
        } else if (val_ < val(t)) {
            return _rank(left(t), val_);
        } else {
            sbt_int_t r = _rank(right(t), val_);
            if (r >= 0) {
                return r + (safe_size(left(t)) + 1);
            }
            return -1;
        }
    }

public:
    TreeType search(const VAL& val) { return _search(root(), val); }

    TreeType pred(const VAL& val) { return const_cast<TreeType>(_pred(root(), val)); }

    TreeType succ(const VAL& val) { return const_cast<TreeType>(_succ(root(), val)); }

    TreeType insert(const VAL& val) { return _insert(NULL_PTR, root(), val); }

    bool remove_select(sbt_int_t rank)
    {
        TreeType del = _remove_select(&root(), rank);
        if (del) {
            delete del;
            return true;
        }
        return false;
    }

    bool remove(const VAL& val)
    {
        if (search(val)) {
            TreeType del = _remove(&root(), val);
            // assert (val)
            delete del;
            return true;
        }
        return false;
    }

    const TreeType select(sbt_int_t rank) const { return _select(root(), rank); }

    TreeType select(sbt_int_t rank) { return const_cast<TreeType>(_select(root(), rank)); }

    sbt_int_t rank(const VAL& val) const { return _rank(root(), val); }

protected:
    TreeType _root;
};

} // namespace ege
