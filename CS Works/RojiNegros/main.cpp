#include <iostream>
#include "BlackRedTree.h"

using namespace std;

int main() {
    RedBlackTree T;

    cout << "\n=== INSERTANDO NODOS ===\n";
    for (int k : {10, 20, 30, 15, 25, 5}) {
        Node* n = new Node(k);
        T.RB_INSERT(n);
    }

    cout << "Inorder tras inserciones: ";
    T.INORDER(T.root);
    cout << "\n";

    cout << "\n=== ELIMINANDO 20 ===\n";
    Node* d = T.SEARCH(T.root, 20);
    if (d != T.nil)
        T.RB_DELETE(d);

    cout << "Inorder tras eliminar 20: ";
    T.INORDER(T.root);
    cout << "\n";

    cout << "\n=== INSERTANDO 17 ===\n";
    Node* n = new Node(17);
    T.RB_INSERT(n);

    cout << "Inorder final: ";
    T.INORDER(T.root);
    cout << "\n";

    return 0;
}
