/**
 ******************************************************************************
* @file           : main.c
 * @course         : CC3086 - Programacion de Microprocesadores
 * @institution    : Universidad del Valle de Guatemala
 * @brief          : Parpadeo de LED mediante acceso directo a registros GPIO
 *
 * @description
 * Programa bare-metal para STM32F4 que configura el puerto GPIOA y el pin PA5
 * como salida digital. El LED es encendido y apagado de forma periodica
 * utilizando el registro BSRR y retardos por software (busy-wait).
 *
 * @author         : Embedded Systems – Bare-Metal Programming Ground Up (STM32)
 * @date           : 2026-02-10
 *
 * @references
 * - RM0090: Reference Manual STM32F4 Series
 * - Datasheet STM32F446RE
 * - STM32CubeIDE Documentation
 *
 ******************************************************************************
 */

#include "stm32f4xx.h"

#define GPIOAEN   (1U << 0)
#define PIN5      (1U << 5)   // PA5
#define PIN6      (1U << 6)   // PA6

int main(void)
{
    RCC->AHB1ENR |= GPIOAEN;

    GPIOA->MODER &= ~(3U << 10);
    GPIOA->MODER |=  (1U << 10);

    // PA6 → bits 12 y 13
    GPIOA->MODER &= ~(3U << 12);
    GPIOA->MODER |=  (1U << 12);

    while (1)
    {
        // 3️⃣ Encender ambos pines al mismo tiempo (UNA sola instrucción)
        GPIOA->BSRR = PIN5 | PIN6;

        for (volatile int i = 0; i < 100000; i++);

        GPIOA->BSRR = (PIN5 << 16) | (PIN6 << 16);

        for (volatile int i = 0; i < 100000; i++);
    }
}
