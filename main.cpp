#include <bits/stdc++.h>
using namespace std;

// =====================
// CONSTANTES
// =====================
const int MAX = 1400005;

// =====================
// STRUCT PRINCIPAL
// =====================
struct Rango {
    long long valor;
    long long inicio;
    long long fin;
};

// =====================
// VARIABLES GLOBALES
// =====================
Rango rangos[MAX];
int total = 0;

ifstream archivo_entrada("entrada.txt");
ofstream archivo_salida("salida.txt");

// =====================
// BÚSQUEDA BINARIA 1
// Encuentra qué rango contiene la posición 'pos'
// Retorna -1 si no existe
// =====================
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

// =====================
// BÚSQUEDA BINARIA 2
// Encuentra el primer rango cuyo inicio >= pos
// =====================
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

// =====================
// INSERTAR en posición idx
// =====================
void insertar(int idx, long long valor, long long inicio, long long fin) {
    for (int i = total; i > idx; i--) {
        rangos[i] = rangos[i - 1];
    }
    rangos[idx].valor = valor;
    rangos[idx].inicio = inicio;
    rangos[idx].fin = fin;
    total++;
}

// =====================
// ELIMINAR en posición idx
// =====================
void eliminar(int idx) {
    for (int i = idx; i < total - 1; i++) {
        rangos[i] = rangos[i + 1];
    }
    total--;
}

// =====================
// MERGE
// =====================
void opMerge() {
    int i = 0;
    while (i < total - 1) {
        if (rangos[i].valor == rangos[i + 1].valor &&
            rangos[i].fin + 1 == rangos[i + 1].inicio) {
            rangos[i].fin = rangos[i + 1].fin;
            eliminar(i + 1);
        }
        else {
            i++;
        }
    }
    archivo_salida << "MERGE = OK\n";
}

// =====================
// UPDATE
// =====================
void opUpdate(long long l, long long r, long long val) {

    // Paso 1: ajustar rango que contiene l
    int idxL = buscarPosicion(l);
    if (idxL != -1) {
        if (rangos[idxL].inicio < l) {
            long long finOriginal = rangos[idxL].fin;
            rangos[idxL].fin = l - 1;
            if (finOriginal > r) {
                insertar(idxL + 1, rangos[idxL].valor, r + 1, finOriginal);
            }
        }
    }

    // Paso 2: eliminar rangos completamente dentro de [l, r]
    int i = buscarPrimero(l);
    while (i < total && rangos[i].inicio <= r) {
        if (rangos[i].inicio >= l && rangos[i].fin <= r) {
            eliminar(i);
        }
        else {
            i++;
        }
    }

    // Paso 3: ajustar rango que contiene r
    int idxR = buscarPosicion(r);
    if (idxR != -1) {
        if (rangos[idxR].fin > r) {
            rangos[idxR].inicio = r + 1;
        }
    }

    // Paso 4: insertar nuevo rango [l, r] con val
    int pos = buscarPrimero(l);
    insertar(pos, val, l, r);

    // Paso 5: fusionar con vecinos
    int idx = buscarPosicion(l);

    if (idx + 1 < total &&
        rangos[idx + 1].valor == val &&
        rangos[idx].fin + 1 == rangos[idx + 1].inicio) {
        rangos[idx].fin = rangos[idx + 1].fin;
        eliminar(idx + 1);
    }

    if (idx - 1 >= 0 &&
        rangos[idx - 1].valor == val &&
        rangos[idx - 1].fin + 1 == rangos[idx].inicio) {
        rangos[idx - 1].fin = rangos[idx].fin;
        eliminar(idx);
    }

    archivo_salida << "UPDATE " << l << " " << r << " " << val << " = OK\n";
}

// =====================
// VALUE
// =====================
void opValue(long long pos) {
    int idx = buscarPosicion(pos);
    archivo_salida << "VALUE " << pos << " = " << rangos[idx].valor << "\n";
}

// =====================
// SUM
// =====================
void opSum(long long l, long long r) {
    long long suma = 0;
    for (int i = 0; i < total; i++) {
        if (rangos[i].fin >= l && rangos[i].inicio <= r) {
            long long desde = max(rangos[i].inicio, l);
            long long hasta = min(rangos[i].fin, r);
            long long cantidad = hasta - desde + 1;
            suma += rangos[i].valor * cantidad;
        }
    }
    archivo_salida << "SUM " << l << " " << r << " = " << suma << "\n";
}

// =====================
// FREQUENCY
// =====================
void opFrequency(long long val) {
    long long count = 0;
    for (int i = 0; i < total; i++) {
        if (rangos[i].valor == val) {
            count += rangos[i].fin - rangos[i].inicio + 1;
        }
    }
    archivo_salida << "FREQUENCY " << val << " = " << count << "\n";
}

// =====================
// MAX_RANGE
// =====================
void opMaxRange(long long l, long long r) {
    long long maximo = LLONG_MIN;
    for (int i = 0; i < total; i++) {
        if (rangos[i].fin >= l && rangos[i].inicio <= r) {
            if (rangos[i].valor > maximo) {
                maximo = rangos[i].valor;
            }
        }
    }
    archivo_salida << "MAX_RANGE " << l << " " << r << " = " << maximo << "\n";
}

// =====================
// MIN_RANGE
// =====================
void opMinRange(long long l, long long r) {
    long long minimo = LLONG_MAX;
    for (int i = 0; i < total; i++) {
        if (rangos[i].fin >= l && rangos[i].inicio <= r) {
            if (rangos[i].valor < minimo) {
                minimo = rangos[i].valor;
            }
        }
    }
    archivo_salida << "MIN_RANGE " << l << " " << r << " = " << minimo << "\n";
}

// =====================
// DECOMPRESS
// =====================
void opDecompress(long long l, long long r) {
    archivo_salida << "DECOMPRESS " << l << " " << r << " = ";
    bool primero = true;
    for (int i = 0; i < total; i++) {
        if (rangos[i].fin >= l && rangos[i].inicio <= r) {
            long long desde = max(rangos[i].inicio, l);
            long long hasta = min(rangos[i].fin, r);
            for (long long j = desde; j <= hasta; j++) {
                if (!primero) archivo_salida << " ";
                archivo_salida << rangos[i].valor;
                primero = false;
            }
        }
    }
    archivo_salida << "\n";
}

// =====================
// COUNT_RANGES
// =====================
void opCountRanges() {
    archivo_salida << "COUNT_RANGES = " << total << "\n";
}

// =====================
// MAIN
// =====================
int main() {

    // Leer rangos iniciales
    int N;
    archivo_entrada >> N;

    for (int i = 0; i < N; i++) {
        archivo_entrada >> rangos[i].valor >> rangos[i].inicio >> rangos[i].fin;
    }
    total = N;

    // Leer y ejecutar operaciones
    int Q;
    archivo_entrada >> Q;

    char operacion[20];
    for (int i = 0; i < Q; i++) {
        archivo_entrada >> operacion;

        if (strcmp(operacion, "VALUE") == 0) {
            long long pos;
            archivo_entrada >> pos;
            opValue(pos);
        }
        else if (strcmp(operacion, "SUM") == 0) {
            long long l, r;
            archivo_entrada >> l >> r;
            opSum(l, r);
        }
        else if (strcmp(operacion, "UPDATE") == 0) {
            long long l, r, val;
            archivo_entrada >> l >> r >> val;
            opUpdate(l, r, val);
        }
        else if (strcmp(operacion, "FREQUENCY") == 0) {
            long long val;
            archivo_entrada >> val;
            opFrequency(val);
        }
        else if (strcmp(operacion, "MAX_RANGE") == 0) {
            long long l, r;
            archivo_entrada >> l >> r;
            opMaxRange(l, r);
        }
        else if (strcmp(operacion, "MIN_RANGE") == 0) {
            long long l, r;
            archivo_entrada >> l >> r;
            opMinRange(l, r);
        }
        else if (strcmp(operacion, "DECOMPRESS") == 0) {
            long long l, r;
            archivo_entrada >> l >> r;
            opDecompress(l, r);
        }
        else if (strcmp(operacion, "COUNT_RANGES") == 0) {
            opCountRanges();
        }
        else if (strcmp(operacion, "MERGE") == 0) {
            opMerge();
        }
    }

    archivo_entrada.close();
    archivo_salida.close();
    return 0;
}