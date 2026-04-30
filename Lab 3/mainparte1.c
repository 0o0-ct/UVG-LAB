/**
 ******************************************************************************
 * @file           : main.c
 * @course         : CC3086 - Programacion de Microprocesadores
 * @institution    : Universidad del Valle de Guatemala
 * @brief          : Control básico de GPIO (encendido y apagado de LED)
 *
 * @description
 * Programa de ejemplo que configura un pin GPIO del STM32F4 como salida
 * digital para encender y apagar un LED utilizando acceso directo a
 * registros del microcontrolador.
 *
 * @author         : Embedded Systems – Bare-Metal Programming Ground Up (STM32)
 * @date           : 2026-02-10
 *
 * @references
 * - STM32F4 Reference Manual (RM0090)
 * - STM32F4xx Datasheet
 * - STM32CubeIDE Documentation
 *
 ******************************************************************************
 */

#include "stm32f4xx.h"


#define GPIOAEN        (1U<<0)
#define PIN5           (1U<<5)
#define LED_PIN        PIN5

int main(void)
{
    RCC->AHB1ENR |= GPIOAEN;

    GPIOA->MODER |= (1U<<10);
    GPIOA->MODER &= ~(1U<<11);

    while(1)
    {
        GPIOA->ODR ^= LED_PIN;

        for(int i=0; i<100000; i++)
        {
        }
    }
}
