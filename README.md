# CC3086 - Programación de Microprocesadores

Este repositorio contiene los laboratorios y proyectos del curso **CC3086 -
Programación de Microprocesadores** de la Universidad del Valle de
Guatemala (UVG).

A continuación se detalla el contenido de cada una de las prácticas y
proyectos alojados en este repositorio:

## 🔬 Laboratorios

### [Lab 3](./Lab%203/)

En este laboratorio se aborda el **Control Básico de GPIO**. Se desarrolló un
programa de ejemplo para configurar un pin GPIO del STM32F4 como salida
digital. Se demuestra cómo encender y apagar un LED mediante acceso directo
a los registros del microcontrolador.

### [Lab 4](./Lab%204/)

Este laboratorio consiste en la **Transmisión Experimental de Datos UART**.
Se desarrollaron programas modulares para enviar datos (tramas y paquetes)
desde el microcontrolador a diferentes tasas de baudios (115200, 38400,
19200, y 9600). Se evidencia el uso de protocolos y la gestión de errores
de sincronización.

### [LAB 5](./LAB%205/)

En esta práctica se trabajó con **SysTick** para la temporización del
microcontrolador STM32F4xx. El objetivo fue diseñar y programar la lógica
de control para generar una secuencia de activación cíclica de un arreglo de
LEDs utilizando los puertos GPIO y la temporización requerida por el SysTick.

### [LAB 7](./LAB%207/)

Este laboratorio trata sobre la gestión de **Procesos con `fork()`**. En
esta práctica se aplican conceptos de creación de procesos paralelos,
manejo de sincronización con la instrucción `wait()`, y finalización de
procesos hijos con `exit()` en sistemas de tipo Unix utilizando C/C++.

## 🚀 Proyectos

### [Proyecto 01](./Proyecto%2001/)

El Proyecto 1 consistió en desarrollar un **Sistema de Control de LEDs**
utilizando la tarjeta STM32 Nucleo. Se programó el microcontrolador en C
para ejecutar secuencias automáticas cíclicas en un arreglo de 8 LEDs con
un periodo base de 1.5 segundos. Además, el sistema incorporó dos botones
que permiten incrementar y disminuir la velocidad de la secuencia a través
de, al menos, tres niveles diferentes.

---

*Universidad del Valle de Guatemala - Facultad de Ingeniería - Departamento
de Ciencias de la Computación.*
