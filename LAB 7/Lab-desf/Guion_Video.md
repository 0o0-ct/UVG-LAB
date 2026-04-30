# Guion para el Video del Laboratorio 07 (Compilador Paralelo)

Este es un guion paso a paso que puedes leer o usar de guía mientras grabas la pantalla de tu computadora. Te sugiero tener abierto el archivo `compilador.cpp` en tu editor de código y una terminal lista para ejecutarlo.

---

### 1. Presentación e Introducción
*(Muestra tu terminal vacía)*
"Hola a todos, mi nombre es [TU NOMBRE] y en este video voy a presentar la ejecución y explicación del desafío final del Laboratorio 7: El Compilador Paralelo. Este programa decodifica instrucciones binarias de 7 bits y las ejecuta simultáneamente utilizando múltiples procesos hijos mediante la función `fork()`."

### 2. Demostración: Caso de Prueba Válido
*(En la terminal, compila el programa si no lo has hecho: `g++ compilador.cpp -o compilador` y ejecútalo: `./compilador`)*
"Para empezar, voy a demostrar un caso de prueba válido. Ingresaré tres instrucciones válidas separadas por espacios:"
*(Ingresa: `1100110 0011100 1001101` y presiona Enter)*
"Como podemos observar, el programa ha dividido el trabajo. El proceso padre toma la instrucción número 1 (Calculando el Modulo de 1 % 2). Al mismo tiempo, creó dos procesos hijos que se encargaron de las instrucciones 2 y 3 (División y Potencia). Cada proceso imprimió su PID único y el tiempo exacto que le tomó realizar la decodificación."

### 3. Demostración: Manejo de Errores
*(Vuelve a ejecutar el programa: `./compilador`)*
"El programa también es muy robusto y está diseñado para manejar errores sin colapsar. Ahora voy a forzar un error intencional ingresando una división por cero y una instrucción con un formato incorrecto."
*(Ingresa: `0111000 1234567 1011001` y presiona Enter)*
"Aquí vemos cómo el programa identificó correctamente la división por cero en la primera instrucción imprimiendo el mensaje de error. En la segunda instrucción, detectó que los caracteres eran inválidos (no eran binarios) e imprimió el error de formato. Y finalmente, la tercera instrucción se calculó de forma normal."

### 4. Explicación del Código y Funciones Nuevas
*(Cambia la pantalla para mostrar el código de `compilador.cpp`)*
"Pasando al código, implementamos la librería `<chrono>` para medir de forma ultra precisa el tiempo de ejecución en microsegundos de cada proceso de manera independiente."
"La lógica principal ocurre en el `main`. Aquí utilizamos un ciclo `for` para iterar sobre la cantidad de instrucciones. Usamos la función `fork()` para crear una copia exacta del proceso actual."

### 5. ¿Por qué `exit()` en los hijos y `wait()` en el padre?
*(Señala la línea del `exit(0);` en el if del hijo)*
"Es crítico observar que, si el proceso creado es un hijo, llamamos a la función `decode_and_execute()` e inmediatamente después usamos `exit(0)`. Esto es fundamental para evitar una 'bomba fork'. Si el hijo no muriera aquí, continuaría ejecutando el ciclo iterativo del padre creando una avalancha de procesos infinitos."

*(Señala la línea del `wait(NULL);` en el ciclo del padre al final)*
"De igual manera, el proceso padre, una vez que termina su propia instrucción, entra en un ciclo utilizando `wait(NULL)`. El padre está obligado a esperar a que sus hijos terminen su ejecución para recoger su estado. Si el padre terminara antes sin esperarlos, los hijos se quedarían atrapados en la memoria RAM como procesos 'Zombis'."

### 6. La pregunta de oro: ¿Es realmente paralelo?
*(Señala ambos ciclos `for` en tu código: el de creación y el de espera)*
"Una pregunta crucial es: *Si utilizo una estructura cíclica para crear a los procesos con fork(), ¿realmente se crean procesos paralelos?*"
"La respuesta es **SÍ**, son realmente paralelos, pero por la forma específica en que estructuramos el código. Fíjense que tenemos un ciclo `for` dedicado **exclusivamente** a hacer `fork()`. Esto hace que todos los hijos 'nazcan' prácticamente al mismo instante y comiencen a trabajar concurrentemente."
"El ciclo `wait()`, que es el que frena al programa, está puesto en un bloque **separado hasta el final**. Si hubiéramos metido el `wait()` dentro del mismo ciclo donde está el `fork()`, entonces sí hubiera sido un programa secuencial falso, porque el padre habría creado un hijo y se habría quedado paralizado esperándolo antes de crear al siguiente. Pero en nuestro código, todos nacen a la vez, trabajan a la vez, y el padre los espera al final. Logrando un paralelismo real."

"Y con esto concluye mi demostración. Muchas gracias."
---
*(Terminas de grabar)*
