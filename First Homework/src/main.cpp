#include <iostream>
#include <cstdlib>
using namespace std;

/*

INTEGRANTES GRUPO - TAREA 1

SERGIO MENDIVELSO > 20231020227
SEBASTIAN AVENDANO > 20232020101

*/


struct nodo {
    int x,y;
    nodo *sig;
};

void codigo1(int N){    
    nodo *cab = NULL, *Aux;
    cout << sizeof(cab) << endl;
    cout << sizeof(Aux) << endl;

    for(int i = 0; i < N; ++i)
        {Aux = new nodo;
            Aux -> x = i;
            Aux -> y = N-i;
            Aux -> sig = cab;
            cab = Aux;
            
            cout << sizeof(cab) << endl;
            cout << sizeof(Aux) << endl;
        }
    Aux = cab;
    while (Aux != NULL)
    { //cout << Aux->x <<Aux->y<<endl;
        cout << sizeof(cab->x) << sizeof(cab->y) << endl;
        Aux = Aux-> sig; 
        cout << sizeof(Aux) << endl;
    }
    for (Aux=cab->sig; Aux!=NULL; Aux=Aux->sig)
    {
        delete cab;
        cab = Aux;
    }
    delete cab;
}

void codigo2(int M, int tam) {
    nodo **arr, *Aux;

    cout << "sizeof(nodo): " << sizeof(nodo) << endl;
    cout << "sizeof(nodo*): " << sizeof(nodo*) << endl;
    cout << "sizeof(nodo**): " << sizeof(nodo**) << endl;

    arr = new nodo *[M];
    cout << "sizeof(arr): " << sizeof(arr) << endl;

    for (int i = 0; i < M; i++)
        arr[i] = NULL;

    for (int i = 0; i < M; i++) {
        for (int j = 1; j <= tam; j++) {
            Aux = new nodo;
            Aux->x = j;
            Aux->y = M + j * i;
            Aux->sig = arr[i];
            arr[i] = Aux;

            cout << "sizeof(*Aux): " << sizeof(*Aux) << endl;
        }
    }

    for (int i = 0; i < M; i++) {
        Aux = arr[i];
        while (Aux != NULL) {
            cout << "sizeof(Aux->x): " << sizeof(Aux->x)
                 << " sizeof(Aux->y): " << sizeof(Aux->y) << endl;
            Aux = Aux->sig;
        }
        for (Aux = arr[i]->sig; Aux != NULL; Aux = Aux->sig) {
            delete arr[i];
            arr[i] = Aux;
        }
        delete arr[i];
    }
    delete[] arr;
}

void codigo3(int N) {
    nodo ***a, *Aux;

    a = new nodo **[N];
    cout << "sizeof(a): " << sizeof(a) << endl;

    for (int i = 0; i < N; i++) {
        a[i] = new nodo *[N];
        for (int j = 0; j < N; j++)
            a[i][j] = NULL;
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++) {
                Aux = new nodo;
                Aux->x = j * k;
                Aux->y = i + k;
                Aux->sig = a[i][j];
                a[i][j] = Aux;

                cout << "sizeof(*Aux): " << sizeof(*Aux) << endl;
            }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Aux = a[i][j];
            while (Aux != NULL) {
                cout << "sizeof(Aux->x): " << sizeof(Aux->x)
                     << " sizeof(Aux->y): " << sizeof(Aux->y) << endl;
                Aux = Aux->sig;
            }
            for (Aux = a[i][j]->sig; Aux != NULL; Aux = Aux->sig) {
                delete a[i][j];
                a[i][j] = Aux;
            }
            delete a[i][j];
        }
        delete a[i];
    }
    delete[] a;
}

struct nodo2 {
    int a;
    float b;
};

void codigo3(int N) {
    nodo2 **a;

    a = new nodo2 *[N];
    cout << "sizeof(a): " << sizeof(a) << endl;

    for (int i = 0; i < N; i++) {
        a[i] = new nodo2[N];
        for (int j = 0; j < N; j++) {
            a[i][j].a = j * i;
            a[i][j].b = j + i;

            cout << "sizeof(a[i][j].a): " << sizeof(a[i][j].a)
                 << " sizeof(a[i][j].b): " << sizeof(a[i][j].b) << endl;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            cout << a[i][j].a << "," << a[i][j].b << " ";
        delete[] a[i];
    }
    delete[] a;
}


int main() {

    // LLenamos los valores los cuales seran usados, en total 50, paso de 10
    // const int size = 50;
    // int array[size];

    // for (int i = 0; i < size; ++i) {
    //     array[i] = i * 10;
    // }

    //TODO: Setear este valor a el valor de N dentro del array de 50 espacios
    cout << sizeof(5) << endl;
    codigo1(5);

    return 0;
}
