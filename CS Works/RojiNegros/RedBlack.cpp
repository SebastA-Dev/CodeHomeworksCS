#include "BlackRedTree.h"
#include "pila1.h"
#include "estructura1.h"

// ---------------- ROTACIONES ----------------

void RedBlackTree::LEFT_ROTATE(Node* x) {
    Node* y = x->right;
    x->right = y->left;

    if (y->left != nil)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == nil)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void RedBlackTree::RIGHT_ROTATE(Node* x) {
    Node* y = x->left;
    x->left = y->right;

    if (y->right != nil)
        y->right->parent = x;

    y->parent = x->parent;

    if (x->parent == nil)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;

    y->right = x;
    x->parent = y;
}

// ---------------- INSERCIÓN ----------------

void RedBlackTree::RB_INSERT(int clave, string info) {
    Node* z = new Node(clave, info);

    Node* y = nil;
    Node* x = root;

    while (x != nil) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;

    if (y == nil)
        root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;

    z->left = nil;
    z->right = nil;
    z->color = RED;

    RB_INSERT_FIXUP(z);
}

void RedBlackTree::RB_INSERT_FIXUP(Node* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;

            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    LEFT_ROTATE(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                RIGHT_ROTATE(z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left;

            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    RIGHT_ROTATE(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                LEFT_ROTATE(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

// ---------------- ELIMINACIÓN ----------------

void RedBlackTree::RB_TRANSPLANT(Node* u, Node* v) {
    if (u->parent == nil)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    v->parent = u->parent;
}

Node* RedBlackTree::TREE_MINIMUM(Node* x) {
    while (x->left != nil)
        x = x->left;
    return x;
}

void RedBlackTree::RB_DELETE(int clave) {
    Node* z = SEARCH(root, clave);
    if (z == nil) return;

    // Marca lógica
    z->info = "[BORRADO]";

    RB_DELETE_INTERNAL(z);
}

void RedBlackTree::RB_DELETE_INTERNAL(Node* z) {
    Node* y = z;
    Node* x;
    bool y_original = y->color;

    if (z->left == nil) {
        x = z->right;
        RB_TRANSPLANT(z, z->right);
    } 
    else if (z->right == nil) {
        x = z->left;
        RB_TRANSPLANT(z, z->left);
    } 
    else {
        y = TREE_MINIMUM(z->right);
        y_original = y->color;
        x = y->right;

        if (y->parent == z)
            x->parent = y;
        else {
            RB_TRANSPLANT(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        RB_TRANSPLANT(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (y_original == BLACK)
        RB_DELETE_FIXUP(x);
}

void RedBlackTree::RB_DELETE_FIXUP(Node* x) {
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                LEFT_ROTATE(x->parent);
                w = x->parent->right;
            }

            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } 
            else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    RIGHT_ROTATE(w);
                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                LEFT_ROTATE(x->parent);
                x = root;
            }
        } 
        else {
            Node* w = x->parent->left;

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                RIGHT_ROTATE(x->parent);
                w = x->parent->left;
            }

            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } 
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    LEFT_ROTATE(w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                RIGHT_ROTATE(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

// ---------------- UTILIDADES ----------------

Node* RedBlackTree::SEARCH(Node* x, int key) {
    if (x == nil || key == x->key) return x;
    if (key < x->key) return SEARCH(x->left, key);
    return SEARCH(x->right, key);
}

bool RedBlackTree::MODIFY(int key, const string& newInfo) {
    Node* n = SEARCH(root, key);   // buscar nodo
    
    if (n == nil) {
        return false;              // clave no encontrada
    }

    n->info = newInfo;             // modificar informaci�n
    return true;
}

void RedBlackTree::INORDER(Node* root) {
    if (root == nil) return;

    pila<Node*> P;
    Node* current = root;

    while (current != nil || !P.PilaVacia()) {

        while (current != nil) {
            P.Push(current);
            current = current->left;
        }

        current = P.Pop();

        std::cout << "(" << current->key << ", " << current->info << ")\n" ;

        current = current->right;
    }
}

void RedBlackTree::PREORDER(Node* root) {
    if (root == nil) return;

    pila<Node*> P;
    P.Push(root);

    while (!P.PilaVacia()) {
        Node* x = P.Pop();

        std::cout << "(" << x->key << ", " << x->info << ")\n";

        if (x->right != nil) P.Push(x->right);
        if (x->left  != nil) P.Push(x->left);
    }
}

void RedBlackTree::POSTORDER(Node* root) {
    if (root == nil) return;

    pila<Node*> P1, P2;
    P1.Push(root);

    while (!P1.PilaVacia()) {
        Node* x = P1.Pop();
        P2.Push(x);

        if (x->left  != nil) P1.Push(x->left);
        if (x->right != nil) P1.Push(x->right);
    }

    while (!P2.PilaVacia()) {
        Node* t = P2.Pop();
        std::cout << "(" << t->key << ", " << t->info << ")\n";
    }
}


