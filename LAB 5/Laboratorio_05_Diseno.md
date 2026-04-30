# PARTE 01 - Diseño de la Solución: Laboratorio 05

**Secuencia Asignada:** Secuencia 7 (Período base: 1.2 segundos).
**Placa Utilizada:** STM32F446RE (Nucleo-64).

## a) Definición de los pines GPIO
Para facilitar el cableado de la protoboard usando solo la información visible (serigrafiada en la tarjeta Nucleo), usaremos los pines digitales "tipo Arduino" desde el `D2` hasta el `D6`, y el botón de usuario preintegrado (`PC13`).

| Etiqueta Placa | Puerto Real del MCU | Uso en Circuito | Modo GPIO |
| :--- | :--- | :--- | :--- |
| **D2** | PA10 | LED 1 (Extremo izquierdo) | Salida (Push-Pull) |
| **D3** | PB3 | LED 2 | Salida (Push-Pull) |
| **D4** | PB5 | LED 3 (Centro)  | Salida (Push-Pull) |
| **D5** | PB4 | LED 4| Salida (Push-Pull) |
| **D6** | PB10 | LED 5 (Extremo derecho) | Salida (Push-Pull) |
| **USER Button** | PC13 | Control de velocidad 2x | Entrada (internamente Pull-Up) |

## b) Registros para configuración y control
Para configurar y operar el microcontrolador a nivel de registros se utilizarán los siguientes:
*   **Habilitación de Relojes (RCC):**
    *   `RCC->AHB1ENR`: Habilitamos reloj de puertos GPIOA (para D2), GPIOB (para D3, D4, D5, D6) y GPIOC (para el botón).
*   **Configuración de GPIO:**
    *   `GPIOA->MODER` y `GPIOB->MODER`: Para configurar nuestros 5 pines como salidas de propósito general (`01`).
    *   `GPIOC->MODER`: Para asegurar que `PC13` esté configurado como entrada digital (`00`).
*   **Control de Estado de GPIO:**
    *   `GPIOA->ODR` y `GPIOB->ODR`: Para escribir los estados alto/bajo (`1` y `0`) en los LEDs y visualizar la secuencia dictada.
    *   `GPIOC->IDR`: Para interceptar o leer el estado actual del pin `PC13` (Blue button).
*   **Control Systick:**
    *   `SysTick->LOAD` y `SysTick->CTRL`: A través del llamado a `SysTick_Config` definimos el valor de recarga para obtener la interrupción con un período base de **1 milisegundo**.

## c) Diagrama del Circuito
*(Dibuja o añade aquí un diagrama en tu reporte PDF ilustrando: 5 LEDs conectados con sus respectivas resistencias delimitadoras de corriente de 330Ω hacia GND, conectados a los pines D2 a D6 [PA10, PB3, PB5, PB4, PB10], y la placa STM32 provista de energía. El botón va dentro de la misma placa Nucleo en PC13).*

## d) Diagrama de Flujo
1.  **Inicio**
2.  Configurar Clocks (RCC).
3.  Configurar GPIOA y GPIOB como salidas, GPIOC como entrada.
4.  Configurar SysTick (Interrupción cada 1 ms).
5.  **Bucle Principal:**
    *   Leer estado de `PC13`. Si ocurre evento *flanco de bajada* válido (usuario presiona con debouncing software):
        *   Alternar bandera lógica `fast_mode` (0 a 1, o de 1 a 0).
    *   Verificar el contador temporal actual incrementado por la interrupción. Si superó el límite esperado (1200 ms regular; 600 ms rápido):
        *   Resetear contador.
        *   Aplicar la máscara de bits a cada salida respectiva (D2 a D6) según el array estado.
        *   Avanzar estado del array Secuencia_7.
6.  **Interrupción SysTick (SysTick_Handler):**
    *   Incrementar el contador de ciclos 1 vez por milisegundo.
    *   Cerrar interrupción y retornar control.

## e) Lógica de control del Blue Button (Normal y 2x)
La lógica de control del botón emplea lectura pasiva dentro del *loop infinito* a manera de *polling*. En cada iteración se evalúa el nivel lógico de PC13. Dado que está condicionado a una resistencia *Pull-up* en la Nucleo, un toque prolonga a `0`.
Se captura ese flanco descendente (de `1` a `0`), se inserta un retardo vacío transitorio que neutraliza los picos eléctricos (`debouncing`) y por último una trampa bloqueante pasiva `while((GPIOC->IDR & (1 << 13)) == 0)` nos garantiza que la variable global selectiva `fast_mode` alterne de valor precisamente sólo cuando exista la liberación mecánica del botón, no activándolo dos veces continuas por accidente.

## f) Párrafo Explicativo
El sistema inicia configurando los relojes (RCC) y los pines de entrada-salida para que concuerden con las conexiones físicas de la tarjeta hija. El corazón que coordina el ritmo es el evento de interrupción generado por el temporizador base `SysTick`, el cual estalla transparentemente cada 1 milisegundo e incrementa por desborde una variable de seguimiento. Paralelamente, la lógica principal verifica ininterrumpidamente que esta variable contadora cruce un umbral numérico dictado por la variable de velocidad actual (1200 en velocidad estandar o 600 al detectar doble). Cuando el umbral temporal es cruzado, nuestro programa aplica individualmente los bits correspondientes al array de la secuencia #7 (`0x11, 0x0A, 0x04...`) directamente encima de sus puertos asíncronos configurados (D2-D6), para avanzar al siguiente estado por consiguiente. Todo este mecanismo se conjuga asincrónicamente con la lectura filtrada (`debouncing`) del Botón Azul permitiendo interactuar con los retardos temporales sin detener de golpe la animación principal.
