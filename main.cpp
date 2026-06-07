#include <bits/stdc++.h>
using namespace std;
const int MAX = 1400005;

struct Rango {
    long long valor;
    long long inicio;
    long long fin;
};

Rango rangos[MAX];
int total = 0;

ifstream fin("entrada.txt");
ofstream fout("salida.txt");

int buscarPosicion(long long pos) {
    int izq = 0, der = total - 1;

    while (izq <= der) {
        int mid = izq + (der - izq) / 2;

        if (rangos[mid].inicio <= pos && pos <= rangos[mid].fin) {
            return mid;
        }
        else if (rangos[mid].fin < pos) {
            izq = mid + 1;
        }
        else {
            der = mid - 1;
        }
    }

    return -1;
}

int buscarPrimero(long long pos) {
    int izq = 0, der = total - 1, resultado = total;

    while (izq <= der) {
        int mid = izq + (der - izq) / 2;

        if (rangos[mid].inicio >= pos) {
            resultado = mid;
            der = mid - 1;
        }
        else {
            izq = mid + 1;
        }
    }

    return resultado;
}

int main() {

    int N;
    fin >> N;

    for (int i = 0; i < N; i++) {
        fin >> rangos[i].valor >> rangos[i].inicio >> rangos[i].fin;
    }
    total = N;
    
    int Q;
    fin >> Q;

    char operacion[20];
    for (int i = 0; i < Q; i++) {

        fin >> operacion;

        if (strcmp(operacion, "VALUE") == 0) {

        }
        else if (strcmp(operacion, "SUM") == 0) {

        }
        else if (strcmp(operacion, "UPDATE") == 0) {
        }
        else if (strcmp(operacion, "FREQUENCY") == 0) {
        }
        else if (strcmp(operacion, "MAX_RANGE") == 0) {
        }
        else if (strcmp(operacion, "MIN_RANGE") == 0) {

        }
        else if (strcmp(operacion, "DECOMPRESS") == 0) {

        }
        else if (strcmp(operacion, "COUNT_RANGES") == 0) {
        }
        else if (strcmp(operacion, "MERGE") == 0) {
        }
    }

    fin.close();
    fout.close();
    return 0;
}