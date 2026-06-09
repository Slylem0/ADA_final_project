#include <cstdio>
#include <cstring>
#include <climits>
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

FILE* archivo_entrada;
FILE* archivo_salida;

// =====================
// BÚSQUEDA BINARIA 1
// Encuentra qué rango contiene la posición 'pos'
// Retorna -1 si no existe
// Divide y vencer: O(log R)
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
// Divide y vencer: O(log R)
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
// INSERTAR en posición idx — O(R)
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
// ELIMINAR en posición idx — O(R)
// =====================
void eliminar(int idx) {
    for (int i = idx; i < total - 1; i++) {
        rangos[i] = rangos[i + 1];
    }
    total--;
}

// =====================
// MERGE
// Fusiona rangos adyacentes con el mismo valor — O(R)
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
    fprintf(archivo_salida, "MERGE = OK\n");
}

// =====================
// UPDATE — O(R)
// Asigna 'val' a todas las posiciones en [l, r]
// =====================
void opUpdate(long long l, long long r, long long val) {

    // Paso 1: partir rango que contiene l (si su inicio < l)
    int idxL = buscarPosicion(l);
    if (idxL != -1 && rangos[idxL].inicio < l) {
        long long finOriginal = rangos[idxL].fin;
        long long valOriginal = rangos[idxL].valor;
        rangos[idxL].fin = l - 1;

        if (finOriginal > r) {
            // El rango original cubre todo [l..r]: insertar fragmento derecho
            insertar(idxL + 1, valOriginal, r + 1, finOriginal);
            // Ya no hay rangos que eliminar dentro de [l,r], solo insertar
            int pos = buscarPrimero(l);
            insertar(pos, val, l, r);
            goto fusionar;
        }
        // Si finOriginal <= r, el rango ya quedó truncado; continuar normalmente
    }

    // Paso 2: partir rango que contiene r (si su fin > r y no fue tratado en Paso 1)
    {
        int idxR = buscarPosicion(r);
        if (idxR != -1 && rangos[idxR].fin > r) {
            long long valOriginal = rangos[idxR].valor;
            long long finOriginal = rangos[idxR].fin;
            rangos[idxR].inicio = r + 1;
            // No necesitamos guardar el fragmento izquierdo porque será reemplazado
        }
    }

    // Paso 3: eliminar todos los rangos completamente dentro de [l, r]
    {
        int i = buscarPrimero(l);
        while (i < total && rangos[i].inicio <= r) {
            if (rangos[i].fin <= r) {
                eliminar(i);
                // no incrementar i: el siguiente quedó en la misma posición
            }
            else {
                i++;
            }
        }
    }

    // Paso 4: insertar nuevo rango [l, r] con val
    {
        int pos = buscarPrimero(l);
        insertar(pos, val, l, r);
    }

    fusionar:
    // Paso 5: fusionar con vecino derecho
    {
        int idx = buscarPosicion(l);
        if (idx == -1) {
            fprintf(archivo_salida, "UPDATE %lld %lld %lld = OK\n", l, r, val);
            return;
        }

        if (idx + 1 < total &&
            rangos[idx + 1].valor == val &&
            rangos[idx].fin + 1 == rangos[idx + 1].inicio) {
            rangos[idx].fin = rangos[idx + 1].fin;
            eliminar(idx + 1);
        }

        // Paso 6: fusionar con vecino izquierdo
        if (idx - 1 >= 0 &&
            rangos[idx - 1].valor == val &&
            rangos[idx - 1].fin + 1 == rangos[idx].inicio) {
            rangos[idx - 1].fin = rangos[idx].fin;
            eliminar(idx);
        }
    }

    fprintf(archivo_salida, "UPDATE %lld %lld %lld = OK\n", l, r, val);
}

// =====================
// VALUE — O(log R)
// =====================
void opValue(long long pos) {
    int idx = buscarPosicion(pos);
    if (idx == -1) {
        fprintf(archivo_salida, "VALUE %lld = UNDEFINED\n", pos);
    } else {
        fprintf(archivo_salida, "VALUE %lld = %lld\n", pos, rangos[idx].valor);
    }
}

// =====================
// SUM — O(R)
// =====================
void opSum(long long l, long long r) {
    long long suma = 0;
    for (int i = 0; i < total; i++) {
        if (rangos[i].fin >= l && rangos[i].inicio <= r) {
            long long desde = rangos[i].inicio > l ? rangos[i].inicio : l;
            long long hasta = rangos[i].fin < r   ? rangos[i].fin   : r;
            long long cantidad = hasta - desde + 1;
            suma += rangos[i].valor * cantidad;
        }
    }
    fprintf(archivo_salida, "SUM %lld %lld = %lld\n", l, r, suma);
}

// =====================
// FREQUENCY — O(R)
// =====================
void opFrequency(long long val) {
    long long count = 0;
    for (int i = 0; i < total; i++) {
        if (rangos[i].valor == val) {
            count += rangos[i].fin - rangos[i].inicio + 1;
        }
    }
    fprintf(archivo_salida, "FREQUENCY %lld = %lld\n", val, count);
}

// =====================
// MAX_RANGE — O(R)
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
    fprintf(archivo_salida, "MAX_RANGE %lld %lld = %lld\n", l, r, maximo);
}

// =====================
// MIN_RANGE — O(R)
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
    fprintf(archivo_salida, "MIN_RANGE %lld %lld = %lld\n", l, r, minimo);
}

// =====================
// DECOMPRESS — O(R + k)
// =====================
void opDecompress(long long l, long long r) {
    fprintf(archivo_salida, "DECOMPRESS %lld %lld = ", l, r);
    bool primero = true;
    for (int i = 0; i < total; i++) {
        if (rangos[i].fin >= l && rangos[i].inicio <= r) {
            long long desde = rangos[i].inicio > l ? rangos[i].inicio : l;
            long long hasta = rangos[i].fin   < r  ? rangos[i].fin   : r;
            for (long long j = desde; j <= hasta; j++) {
                if (!primero) fprintf(archivo_salida, " ");
                fprintf(archivo_salida, "%lld", rangos[i].valor);
                primero = false;
            }
        }
    }
    fprintf(archivo_salida, "\n");
}

// =====================
// COUNT_RANGES — O(1)
// =====================
void opCountRanges() {
    fprintf(archivo_salida, "COUNT_RANGES = %d\n", total);
}

// =====================
// MAIN
// =====================
int main() {
    archivo_entrada = fopen("entrada.txt", "r");
    archivo_salida  = fopen("salida.txt",  "w");

    // Leer rangos iniciales
    int N;
    fscanf(archivo_entrada, "%d", &N);

    for (int i = 0; i < N; i++) {
        fscanf(archivo_entrada, "%lld %lld %lld",
               &rangos[i].valor, &rangos[i].inicio, &rangos[i].fin);
    }
    total = N;

    // Leer y ejecutar operaciones
    int Q;
    fscanf(archivo_entrada, "%d", &Q);

    char operacion[20];
    for (int i = 0; i < Q; i++) {
        fscanf(archivo_entrada, "%s", operacion);

        if (strcmp(operacion, "VALUE") == 0) {
            long long pos;
            fscanf(archivo_entrada, "%lld", &pos);
            opValue(pos);
        }
        else if (strcmp(operacion, "SUM") == 0) {
            long long l, r;
            fscanf(archivo_entrada, "%lld %lld", &l, &r);
            opSum(l, r);
        }
        else if (strcmp(operacion, "UPDATE") == 0) {
            long long l, r, val;
            fscanf(archivo_entrada, "%lld %lld %lld", &l, &r, &val);
            opUpdate(l, r, val);
        }
        else if (strcmp(operacion, "FREQUENCY") == 0) {
            long long val;
            fscanf(archivo_entrada, "%lld", &val);
            opFrequency(val);
        }
        else if (strcmp(operacion, "MAX_RANGE") == 0) {
            long long l, r;
            fscanf(archivo_entrada, "%lld %lld", &l, &r);
            opMaxRange(l, r);
        }
        else if (strcmp(operacion, "MIN_RANGE") == 0) {
            long long l, r;
            fscanf(archivo_entrada, "%lld %lld", &l, &r);
            opMinRange(l, r);
        }
        else if (strcmp(operacion, "DECOMPRESS") == 0) {
            long long l, r;
            fscanf(archivo_entrada, "%lld %lld", &l, &r);
            opDecompress(l, r);
        }
        else if (strcmp(operacion, "COUNT_RANGES") == 0) {
            opCountRanges();
        }
        else if (strcmp(operacion, "MERGE") == 0) {
            opMerge();
        }
    }

    fclose(archivo_entrada);
    fclose(archivo_salida);
    return 0;
}
