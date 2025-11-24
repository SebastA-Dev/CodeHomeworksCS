#ifndef BLACKREDTREE_H
#define BLACKREDTREE_H

#include <iostream>
using namespace std;

// COLORES SIN ENUM
#define RED 1
#define BLACK 2

struct Node {
    int key;
    int color; // AHORA ES UN INT (1 o 2)
    Node *left, *right, *parent;

    Node(int k = 0) {
        key = k;
        color = RED;  // 1
        left = right = parent = nullptr;
    }
};

struct RedBlackTree {
    Node* root;
    Node* nil;

    RedBlackTree() {
        nil = new Node();
        nil->color = BLACK;  // 2
        nil->left = nil->right = nil->parent = nil;
        root = nil;
    }

    void LEFT_ROTATE(Node* x);
    void RIGHT_ROTATE(Node* x);
    void RB_INSERT(Node* z);
    void RB_INSERT_FIXUP(Node* z);

    void RB_TRANSPLANT(Node* u, Node* v);

    Node* TREE_MINIMUM(Node* x);

    void RB_DELETE(Node* z);
    void RB_DELETE_FIXUP(Node* x);

    Node* SEARCH(Node* x, int key);

    void INORDER(Node* x);
};

#endif
