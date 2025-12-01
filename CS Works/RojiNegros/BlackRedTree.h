#ifndef BLACKREDTREE_H
#define BLACKREDTREE_H

#include <iostream>

#define RED true
#define BLACK false

using namespace std;

struct Node {
    int key;            
    string info;       
    Node *left, *right, *parent;
    bool color;         

    Node(int k = 0, string i = "") {
        key = k;
        info = i;
        color = RED;
        left = right = parent = nullptr;
    }
};

struct RedBlackTree {
    Node* root;
    Node* nil;

    RedBlackTree() {
        nil = new Node();
        nil->color = BLACK;
        nil->left = nil->right = nil->parent = nil;
        root = nil;
    }

    void LEFT_ROTATE(Node* x);
    void RIGHT_ROTATE(Node* x);

    void RB_INSERT(int clave, string info);
    void RB_INSERT_FIXUP(Node* z);

    void RB_TRANSPLANT(Node* u, Node* v);
    Node* TREE_MINIMUM(Node* x);

    void RB_DELETE(int clave);
    void RB_DELETE_INTERNAL(Node* z);
    void RB_DELETE_FIXUP(Node* x);

    Node* SEARCH(Node* x, int key);
    bool MODIFY(int key, const string& newInfo);
    void INORDER(Node* x);     
	void PREORDER(Node* x);    
	void POSTORDER(Node* x);   

};

#endif
