// include/lib/avl_tree.h
// Copyright (C) 2023  Alex Zebra

// AVL Tree Implementation in C++

#include "define/integer.h"

struct avl_node {
    u_int32 key;
    void *data;
    avl_node *left;
    avl_node *right;
    u_char8 height;

    // avl_node () { key = k; left = right = NULL; height = 1; };
    avl_node (u_int32 k, u_char8 *d) {
        key = k;
        left = right = NULL;
        height = 1;
        data = d;
    };
};

class avl_tree {
    avl_node *root = &avl_node(140, "data");
    u_int32 _tree_size = 1;
    avl_node *nodes_list = (size_t)0x8000;

    u_char8
    _height (avl_node *p) {
        return p ? p->height : 0;
    }

    s_char8
    _bfactor (avl_node *p) {
        return _height(p->right) - _height(p->left);
    }

    void
    _fixheight (avl_node *p) {
        u_char8 hl = _height(p->left);
        u_char8 hr = _height(p->right);
        p->height = (hl > hr ? hl : hr) + 1;
    }

    avl_node *                  // правый поворот вокруг p
    _rotateright (avl_node *p) {
        avl_node *q = p->left;
        p->left = q->right;
        q->right = p;
        _fixheight(p);
        _fixheight(q);
        return q;
    }

    avl_node *                 // левый поворот вокруг q
    _rotateleft (avl_node *q) {
        avl_node *p = q->right;
        q->right = p->left;
        p->left = q;
        _fixheight(q);
        _fixheight(p);
        return p;
    }

    avl_node *              // балансировка узла p
    _balance (avl_node *p) {
        _fixheight(p);
        if (_bfactor(p) == 2) {
            if (_bfactor(p->right) < 0)
                p->right = _rotateright(p->right);
            return _rotateleft(p);
        }
        if (_bfactor(p) == -2) {
            if (_bfactor(p->left) > 0)
                p->left = _rotateleft(p->left);
            return _rotateright(p);
        }
        return p; // балансировка не нужна
    }

    avl_node *              // поиск узла с минимальным ключом в дереве p 
    _findmin (avl_node *p) {
        return p->left ? _findmin(p->left) : p;
    }

    avl_node *                // удаление узла с минимальным ключом из дерева p
    _removemin (avl_node* p) {
        if (p->left==0)
            return p->right;
        p->left = _removemin(p->left);
        return _balance(p);
    }

    avl_node *
    _find (avl_node *p, u_int32 x) {
        while (p != NULL) {
            // if (p == NULL)
            if (x < p->key)
                p = p->left;
            else if (x > p->key)
                p = p->right;
            else
                return p;
        }
        return NULL;
    }

    avl_node *                    // вставка ключа k в дерево с корнем p
    _insert (avl_node *p, u_int32 k, u_char8 *data) {
        if (p == NULL) {
            // avl_node *new_node = &avl_node(k, data);
            nodes_list[0] = avl_node(k, data);
            return new_node;
        }
        if (k<p->key)
            p->left = _insert(p->left, k, data);
        else
            p->right = _insert(p->right, k, data);
        return _balance(p);
    }

    avl_node *                    // удаление ключа k из дерева p
    _remove (avl_node *p, u_int32 k) {
        if (p == NULL)
            return NULL;
        if (k < p->key)
            p->left = _remove(p->left, k);
        else if (k > p->key)
            p->right = _remove(p->right, k);
        else {  //  k == p->key 
            avl_node *q = p->left,
                     *r = p->right;
            delete p;
            if (!r)
                return q;
            avl_node *min = _findmin(r);
            min->right = _removemin(r);
            min->left = q;
            return _balance(min);
        }
        return _balance(p);
    }

public:
    // avl_tree () {
    //     root = NULL;
    // }

    void
    insert (u_int32 key, u_char8 *data = NULL) {
        root = _insert(root, key, data);
    }

    void
    remove (u_int32 key) {
        root = _remove(root, key);
    }

    avl_node *
    find_node (u_int32 key) {
        return _find(root, key);
    }

    u_char8 *
    find (u_int32 key) {
        return _find(root, key)->data;
    }

    u_int32
    get_size () {
        return _tree_size;
    }
};

// u_int32 main()
// {
//     BST t;
//     t.insert(20);
//     t.remove(38);
// }