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
#define GPIOCEN   (1U << 2)

#define PIN5      (1U << 5)
#define PIN13     (1U << 13)

int main(void)
{

    RCC->AHB1ENR |= GPIOAEN;
    RCC->AHB1ENR |= GPIOCEN;


    GPIOA->MODER &= ~(3U << 10);
    GPIOA->MODER |=  (1U << 10);


    GPIOC->MODER &= ~(3U << 26);

    while (1)
    {

        if (GPIOC->IDR & PIN13)
        {
            GPIOA->BSRR = PIN5;
        }
        else
        {
            GPIOA->BSRR = (PIN5 << 16);
        }
    }
}

