# Respuestas Teóricas - Laboratorio 07

Aquí tienes las respuestas a las preguntas teóricas del PDF para que puedas copiarlas en tu documento de entrega.

---

## Ejercicio 1 (fork_71.cpp)

**a. ¿Cuántas veces se solicita que se ingrese el valor de la variable a? ¿Explica por qué se realiza esa cantidad de veces la solicitud de ingreso de a?**
Se solicita **solo 1 vez**. Esto ocurre porque la instrucción `cin >> a` se ejecuta en la línea 21, lo cual sucede *antes* de que se llame a la función `fork()` (en la línea 25). En ese momento, el programa todavía es un solo proceso (el padre).

**b. Después de llamar a la función fork(), ¿cuántas variables con el nombre a existen en el address space de ejecución del programa? ¿por qué?**
Existen **2 variables** con el nombre `a`. Una pertenece a la memoria (address space) del proceso padre y la otra a la memoria del proceso hijo. Esto sucede porque `fork()` crea una copia exacta e independiente de toda la memoria del proceso padre en el hijo. Cuando el padre le suma 5 a su variable `a`, y el hijo le suma 10 a su propia variable `a`, no se afectan entre sí porque son espacios de memoria separados.

---

## Ejercicio 2 (fork_72.cpp)

**a. ¿En ambos ejemplos se generan procesos hijos paralelos?**
Sí, en el programa `fork_72.cpp` los procesos hijos sí se ejecutan en paralelo. Esto se debe a que dentro del primer ciclo `for`, el proceso padre simplemente crea a un hijo (usando `fork()`) y continúa a la siguiente iteración del ciclo para crear al siguiente hijo sin detenerse. Como el hijo llega a la instrucción `exit(EXIT_SUCCESS)` después de imprimir su mensaje, termina ahí mismo. El padre espera a todos sus hijos usando `wait()` en un ciclo *completamente separado* al final del programa.

**b. Dibuja un diagrama que muestre cómo se generan los procesos hijos para este programa.**
*(El diagrama sería así)*
```text
Padre 
  |-- Hijo_1
  |-- Hijo_2
  |-- Hijo_3
  |-- Hijo_N
```
*(Todos los hijos son creados directamente por el Padre, y se ejecutan al mismo tiempo de forma horizontal).*

**c. ¿Qué sucede cuando un hijo llama a exit?**
Cuando el proceso hijo llama a `exit`, este termina su ejecución inmediatamente y libera la memoria que ocupaba. Sin embargo, su entrada en la tabla de procesos del sistema operativo se mantiene (se convierte en un proceso "zombie") temporalmente hasta que el proceso padre recoja su estado.

**d. ¿Qué sucede cuando el proceso padre llama a wait?**
El proceso padre pausa (bloquea) su propia ejecución hasta que alguno de sus procesos hijos termine. Una vez que el hijo termina, el `wait()` recoge la información de salida del hijo y lo elimina completamente del sistema (evitando que se quede como proceso zombie zau huerfano).

---

## Ejercicio 3 (Investigación de funciones)

| FUNCIÓN | DESCRIPCIÓN DETALLADA |
| :--- | :--- |
| **`execvp()`** | **1. Uso:** Reemplaza el proceso actual en ejecución por un nuevo programa.<br>**2. Parámetros:** `file` (el nombre del programa/comando), `argv` (un arreglo de argumentos).<br>**3. Retorno:** No retorna si tiene éxito. Si falla, retorna `-1`.<br>**4. Ejemplo:** `execvp("ls", args);` |
| **`sleep()`** | **1. Uso:** Pausa o suspende la ejecución del proceso durante un tiempo determinado.<br>**2. Parámetros:** `seconds` (cantidad de segundos enteros a dormir).<br>**3. Retorno:** Retorna `0` si durmió el tiempo completo, o los segundos restantes si fue interrumpido.<br>**4. Ejemplo:** `sleep(5);` |
| **`wait()`** | **1. Uso:** Hace que el proceso padre espere hasta que un proceso hijo cambie de estado (generalmente, hasta que termine).<br>**2. Parámetros:** `&status` (puntero entero para guardar cómo terminó el hijo) o `NULL`.<br>**3. Retorno:** Retorna el PID del proceso hijo que finalizó, o `-1` si hubo un error.<br>**4. Ejemplo:** `wait(NULL);` |
| **`kill()`** | **1. Uso:** Envía una señal específica a un proceso (se usa frecuentemente para forzar el cierre de procesos).<br>**2. Parámetros:** `pid` (el PID del proceso destino), `sig` (la señal, por ejemplo, SIGKILL).<br>**3. Retorno:** Retorna `0` si la señal se envió exitosamente, o `-1` en caso de error.<br>**4. Ejemplo:** `kill(pid_hijo, SIGKILL);` |
| **`exit()`** | **1. Uso:** Termina la ejecución normal de un proceso inmediatamente.<br>**2. Parámetros:** `status` (un entero, típicamente `0` indica éxito y números mayores a `0` indican errores).<br>**3. Retorno:** No retorna ningún valor a la función que lo llamó (ya que el proceso muere).<br>**4. Ejemplo:** `exit(0);` |

---

## Ejercicio 4 (fork_73.cpp)

**a. Dibuja el árbol de procesos generado por las dos llamadas a fork(). Indica qué procesos son creados en la primera llamada y cuáles en la segunda.**
```text
Padre
  |-- Primer fork(): Crea Hijo_1
  |-- Segundo fork():
      |-- El Padre crea a Hijo_2
      |-- El Hijo_1 crea a Nieto_1
```
*(Total: 4 procesos).*

**b. Al habilitar la tercera llamada a fork(), determina el número total de procesos generados y dibuja el nuevo árbol.**
El número total de procesos generados será **8 procesos** (1 padre, 7 descendientes).
```text
Padre
 |-- Hijo_1 (creado en fork 1)
 |    |-- Nieto_1 (creado en fork 2)
 |    |    |-- Bisnieto_1 (creado en fork 3)
 |    |-- Nieto_2 (creado en fork 3)
 |-- Hijo_2 (creado en fork 2)
 |    |-- Nieto_3 (creado en fork 3)
 |-- Hijo_3 (creado en fork 3)
```

**c. Explica cómo el número de llamadas a fork() influye en la cantidad total de procesos generados. Justifica tu respuesta describiendo el patrón de crecimiento observado.**
El número total de procesos generados crece de forma **exponencial** siguiendo la fórmula $2^N$, donde `N` es la cantidad de llamadas consecutivas a la función `fork()`. 
Esto ocurre porque cada vez que se ejecuta un `fork()`, *todos* los procesos que existen en ese momento (tanto el padre original como todos los hijos creados anteriormente) se clonan a sí mismos. 
- 1 fork: 2^1 = 2 procesos.
- 2 forks: 2^2 = 4 procesos.
- 3 forks: 2^3 = 8 procesos.
