# Proyecto Final — Motor de Consultas sobre Datos Comprimidos por Rangos

**Universidad del Valle **

| | |
|---|---|
| **Integrantes** | Pablo Nicolás Marín - Miguel Ángel Sanclemente |
| **Docente** | Mateo Echeverry Correa |
| **Curso** | Análisis y Diseño de Algoritmos I |
| **Semestre** | 5 — 2026-I |

---

## 1. Descripción General del Problema

El problema consiste en construir un motor de consultas eficiente sobre una secuencia de datos representada mediante rangos comprimidos. En lugar de almacenar cada elemento individualmente, la secuencia se representa como una lista de rangos de la forma `(valor, inicio, fin)`, donde todas las posiciones entre `inicio` y `fin` tienen el mismo valor.

Por ejemplo, la secuencia:

```
1 1 1 1 1 3 3 3 2 2 2 2
```

se representa comprimida como:

- Valor `1` desde la posición 1 hasta la 5
- Valor `3` desde la posición 6 hasta la 8
- Valor `2` desde la posición 9 hasta la 12

El motor debe responder consultas y aplicar actualizaciones directamente sobre esta representación comprimida, sin expandir la secuencia completa. Esto es crucial porque las posiciones pueden llegar hasta 10⁹, haciendo inviable cualquier representación elemento a elemento.

---

## 2. Formato de Entrada y Salida

### 2.1 Entrada (`entrada.txt`)

```
N
valor inicio fin
valor inicio fin
...
Q
OPERACION parametros
OPERACION parametros
...
```

- `N`: cantidad de rangos iniciales.
- Cada rango tiene un valor, una posición inicial y una posición final.
- `Q`: cantidad de operaciones a ejecutar.

### 2.2 Salida (`salida.txt`)

Cada operación que produce resultado escribe una línea con el formato:

| Operación | Formato de salida |
|---|---|
| `VALUE pos` | `VALUE pos = valor` |
| `SUM l r` | `SUM l r = resultado` |
| `UPDATE l r val` | `UPDATE l r val = OK` |
| `FREQUENCY val` | `FREQUENCY val = cantidad` |
| `MAX_RANGE l r` | `MAX_RANGE l r = maximo` |
| `MIN_RANGE l r` | `MIN_RANGE l r = minimo` |
| `DECOMPRESS l r` | `DECOMPRESS l r = v1 v2 ... vn` |
| `COUNT_RANGES` | `COUNT_RANGES = total` |
| `MERGE` | `MERGE = OK` |

---

## 3. Diseño General de la Solución

La solución mantiene un arreglo ordenado de rangos, donde cada rango representa un segmento contiguo de posiciones con el mismo valor. Los rangos se mantienen ordenados por su posición de inicio, lo que permite aplicar búsqueda binaria para localizar rangos eficientemente.

### Invariante del arreglo

- Los rangos están ordenados por `inicio` de menor a mayor.
- No existen solapamientos entre rangos.
- Toda posición del dominio pertenece a exactamente un rango.

Esta invariante se mantiene activamente durante cada operación `UPDATE`.

---

## 4. Estructuras de Datos Implementadas

### 4.1 Arreglo de Rangos (`Rango rangos[MAX]`)

La estructura principal es un arreglo estático de structs `Rango`, cada uno con tres campos `long long`: `valor`, `inicio` y `fin`. El arreglo se mantiene ordenado por `inicio`.

```cpp
struct Rango {
    long long valor;
    long long inicio;
    long long fin;
};
```

**Justificación:** Permite acceso por índice en O(1) y búsqueda binaria en O(log R). Se usa `long long` para soportar posiciones hasta 10⁹ y sumas que pueden superar 2³¹.

### 4.2 Búsqueda Binaria 1 — `buscarPosicion(pos)`

Encuentra qué rango contiene la posición `pos`. Retorna `-1` si no existe.

**Complejidad:** O(log R)

### 4.3 Búsqueda Binaria 2 — `buscarPrimero(pos)`

Encuentra el primer rango cuyo `inicio >= pos` (equivalente a un lower_bound implementado manualmente).

**Complejidad:** O(log R)

### 4.4 Tabla resumen de estructuras

| Estructura | Problema donde se usa | Operaciones principales | Justificación | Complejidad |
|---|---|---|---|---|
| Arreglo de Rangos | Todas | Todas | Almacena la secuencia comprimida sin expandirla | O(R) espacio |
| `buscarPosicion` | Todas las consultas | VALUE, UPDATE, fusión | Localiza el rango que contiene una posición exacta | O(log R) |
| `buscarPrimero` | UPDATE, inserción | Insertar en orden | Localiza el punto de inserción correcto | O(log R) |

---

## 5. Operaciones Soportadas

| Operación | Parámetros | Descripción |
|---|---|---|
| `VALUE` | `pos` | Retorna el valor en la posición `pos`. |
| `SUM` | `l r` | Suma de todos los valores en el rango `[l, r]`. |
| `UPDATE` | `l r val` | Asigna `val` a todas las posiciones en `[l, r]`. Puede dividir, reemplazar y fusionar rangos. |
| `FREQUENCY` | `val` | Cuenta cuántas posiciones tienen exactamente el valor `val`. |
| `MAX_RANGE` | `l r` | Retorna el mayor valor presente en `[l, r]`. |
| `MIN_RANGE` | `l r` | Retorna el menor valor presente en `[l, r]`. |
| `DECOMPRESS` | `l r` | Expande y lista cada elemento individualmente en `[l, r]`. |
| `COUNT_RANGES` | — | Retorna el número total de rangos actuales. |
| `MERGE` | — | Fusiona rangos adyacentes que tengan el mismo valor. |

---

## 6. Análisis de Complejidad Temporal

Variables usadas:
- **R**: cantidad actual de rangos.
- **k**: cantidad de elementos expandidos (solo en DECOMPRESS).

| Operación | Complejidad | Justificación |
|---|---|---|
| `VALUE` | O(log R) | Una búsqueda binaria sobre los R rangos. |
| `SUM` | O(R) | Recorre todos los rangos para acumular en el intervalo. |
| `UPDATE` | O(R) | Las operaciones insertar y eliminar desplazan elementos del arreglo. |
| `FREQUENCY` | O(R) | Recorre todos los rangos buscando el valor solicitado. |
| `MAX_RANGE` | O(R) | Recorre los rangos que intersectan con `[l, r]`. |
| `MIN_RANGE` | O(R) | Igual que MAX_RANGE. |
| `DECOMPRESS` | O(R + k) | Localiza rangos en O(R) y expande k elementos. |
| `COUNT_RANGES` | O(1) | Retorna directamente la variable `total`. |
| `MERGE` | O(R) | Un solo recorrido lineal sobre el arreglo. |
| `buscarPosicion` | O(log R) | Búsqueda binaria — divide y vencer. |
| `buscarPrimero` | O(log R) | Búsqueda binaria — divide y vencer. |
| `insertar` (auxiliar) | O(R) | Desplaza elementos para abrir espacio. |
| `eliminar` (auxiliar) | O(R) | Desplaza elementos para cerrar el hueco. |

### Comparación con solución ingenua

| Aspecto | Solución ingenua | Solución implementada |
|---|---|---|
| Memoria | O(10⁹) — inviable | O(R) donde R ≤ 1,000,000 |
| `VALUE` | O(1) | O(log R) |
| `UPDATE` | O(r − l + 1) | O(R) |
| `SUM` | O(r − l + 1) | O(R) |
| `FREQUENCY` | O(10⁹) | O(R) |
| Viabilidad | Inviable para dominio 10⁹ | Viable con R ≤ 10⁶ |

---

## 7. Estrategia de Dividir y Vencer

Se implementaron dos funciones de búsqueda binaria que aplican la estrategia de dividir y vencer: `buscarPosicion` y `buscarPrimero`.

### 7.1 `buscarPosicion(pos)`

Encuentra qué rango contiene la posición `pos`.

| Elemento | Descripción |
|---|---|
| **Problema** | Determinar cuál de los R rangos contiene una posición dada. |
| **Caso base** | `izq > der` — la posición no existe en ningún rango, retorna -1. |
| **División** | Se calcula `mid = izq + (der - izq) / 2` y se compara `pos` con el rango `mid`. |
| **Combinación** | Si `pos` está en `[rangos[mid].inicio, rangos[mid].fin]`, retorna `mid`. Si `pos > rangos[mid].fin`, busca en la mitad derecha. Si no, en la izquierda. |
| **Complejidad** | O(log R) — cada paso descarta la mitad del espacio de búsqueda. |
| **vs. solución ingenua** | Búsqueda lineal sería O(R); la binaria logra O(log R). |

### 7.2 `buscarPrimero(pos)`

Encuentra el primer rango cuyo `inicio` es mayor o igual a `pos`.

| Elemento | Descripción |
|---|---|
| **Problema** | Localizar el punto de inserción para un nuevo rango con `inicio = pos`. |
| **Caso base** | `izq > der` — retorna `resultado`, el índice de inserción correcto. |
| **División** | Se calcula `mid` y se compara `rangos[mid].inicio` con `pos`. |
| **Combinación** | Si `rangos[mid].inicio >= pos`, se guarda `mid` como candidato y se busca en la mitad izquierda para encontrar uno más temprano. |
| **Complejidad** | O(log R). |
| **Uso** | Se usa en UPDATE para insertar el nuevo rango en la posición correcta y mantener el orden. |

---

## 8. Análisis de Uso de Memoria

La memoria usada es proporcional al número de rangos R, no al tamaño del dominio. Cada `struct Rango` ocupa 24 bytes (3 campos `long long` de 8 bytes cada uno).

```
1,400,005 × 24 bytes ≈ 33.6 MB
```

Esto es viable y está por debajo de los límites típicos (256–512 MB). La solución nunca construye estructuras de tamaño proporcional al dominio (10⁹).

| Estructura | Tamaño | Memoria aprox. |
|---|---|---|
| `rangos[MAX]` | 1,400,005 structs × 24 bytes | ≈ 33.6 MB |
| Variables auxiliares | int, long long | < 1 KB |
| **Total** | | **≈ 33.6 MB** |

---

## 9. Casos de Prueba Utilizados

El archivo `entrada.txt` contiene 58 operaciones diseñadas para cubrir todos los escenarios relevantes:

| Caso | Operaciones | Resultado esperado |
|---|---|---|
| Consultas básicas | `VALUE 10`, `SUM 1 10`, `MAX_RANGE 1 20` | `VALUE 10 = 2`, `SUM 1 10 = 18`, `MAX_RANGE 1 20 = 8` |
| FREQUENCY valor inexistente | `FREQUENCY 99` | `FREQUENCY 99 = 0` |
| UPDATE reemplaza rango completo | `UPDATE 6 8 5` | El rango [6,8] cambia de valor 3 a 5 |
| UPDATE divide un rango | `UPDATE 3 10 7` | El rango existente se divide en fragmentos |
| UPDATE sobre todo el dominio | `UPDATE 1 1000000000 9` | `COUNT_RANGES = 1` |
| MERGE después de updates | `UPDATE 50 150 7`, `UPDATE 140 250 7`, `MERGE` | `COUNT_RANGES = 3` |
| DECOMPRESS parcial | `DECOMPRESS 48 55` | `9 9 7 7 7 7 7 7` |
| SUM con valores grandes | `SUM 1 1000000000` | `9000000000` |

---

## 10. Casos Límite Considerados

| Caso límite | Cómo se maneja |
|---|---|
| Rangos consecutivos con el mismo valor | `MERGE` los fusiona en un solo rango. |
| UPDATE que divide un rango en tres | Se guarda el fragmento izquierdo, se inserta el derecho, luego se inserta el nuevo rango. |
| UPDATE que fusiona rangos vecinos | Después de insertar, se verifica si el vecino izquierdo o derecho tiene el mismo valor y se fusionan. |
| Consulta sobre una sola posición (`l == r`) | VALUE, SUM y demás operaciones funcionan correctamente. |
| Posiciones hasta 10⁹ | Se usan `long long` en todos los campos para evitar desbordamiento. |
| FREQUENCY de valor inexistente | Retorna 0 correctamente. |
| Secuencia con un único rango | Todas las operaciones funcionan con `total = 1`. |
| SUM con resultado mayor a 2³¹ | Se usa `long long` para la acumulación. |

---

## 11. Instrucciones de Compilación y Ejecución

### Requisitos

- Compilador `g++` con soporte para C++17.
- Archivos `main.cpp` y `entrada.txt` en la misma carpeta.

### Compilación

```bash
g++ -std=c++17 -O2 -o motor main.cpp
```

### Ejecución

```bash
./motor.exe   # Windows
./motor       # Linux / macOS
```

El programa lee automáticamente `entrada.txt` y genera `salida.txt` en la misma carpeta donde se ejecuta.

---

## 12. Conclusiones

- La representación por rangos comprimidos permite trabajar eficientemente con secuencias de hasta 10⁹ elementos, manteniendo en memoria únicamente los rangos activos.
- La búsqueda binaria, implementada manualmente como estrategia de dividir y vencer, reduce el tiempo de localización de rangos de O(R) a O(log R), lo cual es crítico para operaciones frecuentes como VALUE y UPDATE.
- La operación UPDATE es la más compleja del sistema, ya que requiere dividir, eliminar y fusionar rangos manteniendo la invariante de no solapamiento.
- El uso de `long long` en todos los campos numéricos evita desbordamientos en dominios grandes y en operaciones de suma acumulada.
- La solución es significativamente más eficiente en memoria que una solución ingenua: O(R) versus O(10⁹), haciendo viable el procesamiento de grandes volúmenes de datos comprimidos.
