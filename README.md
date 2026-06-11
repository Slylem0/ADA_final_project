# Motor de Consultas sobre Datos Comprimidos por Rangos
**Análisis y Diseño de Algoritmos I — 2026-I**  
Universidad del Valle

## Integrantes
- Pablo Nicolás Marín
- Miguel Ángel Sanclemente

---

## Descripción

Motor algorítmico que procesa una secuencia extensa representada mediante rangos comprimidos, respondiendo consultas y actualizaciones sin expandir completamente la secuencia.

Por ejemplo, la secuencia `1 1 1 1 1 3 3 3 2 2 2 2` se representa internamente como:
```
valor=1  inicio=1  fin=5
valor=3  inicio=6  fin=8
valor=2  inicio=9  fin=12
```

---

## Estructura del proyecto

```
ADA_final_project/
├── main.cpp
├── entrada.txt
├── salida.txt
└── README.md
```

---

## Operaciones soportadas

| Operación | Descripción | Complejidad |
|---|---|---|
| `VALUE pos` | Valor en la posición `pos` | O(log R) |
| `SUM l r` | Suma de valores en el rango `[l, r]` | O(R) |
| `UPDATE l r val` | Reemplaza valores en `[l, r]` con `val` | O(R) |
| `FREQUENCY val` | Total de posiciones con valor `val` | O(R) |
| `MAX_RANGE l r` | Valor máximo en `[l, r]` | O(R) |
| `MIN_RANGE l r` | Valor mínimo en `[l, r]` | O(R) |
| `DECOMPRESS l r` | Expande y muestra los valores de `[l, r]` | O(R + k) |
| `COUNT_RANGES` | Cantidad de rangos actualmente almacenados | O(1) |
| `MERGE` | Fusiona rangos adyacentes con el mismo valor | O(R) |

> R = cantidad de rangos almacenados, k = cantidad de posiciones descomprimidas

---

## Formato de entrada

```
N
valor1 inicio1 fin1
valor2 inicio2 fin2
...
Q
OPERACION parametros
OPERACION parametros
...
```

### Ejemplo `entrada.txt`
```
5
1 1 5
3 6 8
2 9 14
8 15 16
4 17 20
7
VALUE 10
SUM 1 10
UPDATE 6 8 5
FREQUENCY 2
MAX_RANGE 1 20
DECOMPRESS 1 12
COUNT_RANGES
```

### Ejemplo `salida.txt`
```
VALUE 10 = 2
SUM 1 10 = 18
UPDATE 6 8 5 = OK
FREQUENCY 2 = 6
MAX_RANGE 1 20 = 8
DECOMPRESS 1 12 = 1 1 1 1 1 5 5 5 2 2 2 2
COUNT_RANGES = 5
```

---

## Compilación y ejecución

### Compilar
```bash
g++ -O2 -o proyecto main.cpp
```

### Ejecutar
```bash
./proyecto
```
En Windows:
```bash
proyecto.exe
```

El programa lee automáticamente desde `entrada.txt` y escribe en `salida.txt`. Ambos archivos deben estar en la misma carpeta que el ejecutable.

---

## Diseño algorítmico

### Estructura de datos principal

Se utiliza un **arreglo ordenado de rangos** donde cada elemento almacena `{valor, inicio, fin}`. El arreglo se mantiene ordenado por posición de inicio en todo momento.

```cpp
struct Rango {
    long long valor;
    long long inicio;
    long long fin;
};
```

Esta representación permite trabajar directamente sobre los rangos comprimidos sin expandir la secuencia, cumpliendo con las restricciones de memoria del problema (dominios hasta 10⁹).

### Dividir y vencer — Búsqueda binaria

El proyecto implementa dos búsquedas binarias propias que son la base de las operaciones principales:

**`buscarPosicion(pos)`** — Encuentra qué rango contiene una posición dada.
- Caso base: `izq > der` → retorna -1
- División: calcula `mid = izq + (der - izq) / 2`
- Si `rangos[mid].inicio <= pos <= rangos[mid].fin` → encontrado
- Si `rangos[mid].fin < pos` → buscar en mitad derecha
- Si `rangos[mid].inicio > pos` → buscar en mitad izquierda
- Complejidad: **O(log R)**

**`buscarPrimero(pos)`** — Encuentra el primer rango cuyo inicio es mayor o igual a `pos`.
- Misma estrategia de división, mantiene el mejor candidato encontrado
- Complejidad: **O(log R)**

Ambas son superiores a una búsqueda lineal O(R), especialmente relevante en operaciones como `UPDATE` con Q=200,000 consultas.

### Operación UPDATE

Es la operación más compleja. Sigue cinco pasos:
1. Partir el rango que contiene `l` si su inicio es anterior a `l`
2. Partir el rango que contiene `r` si su fin es posterior a `r`
3. Eliminar todos los rangos completamente contenidos en `[l, r]`
4. Insertar el nuevo rango `{val, l, r}`
5. Fusionar con vecinos si tienen el mismo valor

---

## Restricciones consideradas

- N hasta 1,000,000 rangos iniciales
- Q hasta 200,000 operaciones
- Coordenadas hasta 10⁹
- No se expande la secuencia completa en memoria
- No se usan estructuras de la STL como `map`, `set`, `priority_queue` ni `std::sort`
