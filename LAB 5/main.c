#include "stm32f4xx.h"

// Variable global incrementada por el SysTick cada 1 ms
volatile uint32_t tick_counter = 0;
// Bandera para seleccionar la velocidad de la secuencia (0 = Normal, 1 = 2x Rápida)
volatile uint8_t fast_mode = 0;

// Arreglo con mascara de bits para la Secuencia 7
const uint8_t secuencia_7[5] = {0x11, 0x0A, 0x04, 0x0A, 0x11};
uint8_t current_state = 0;

void SysTick_Handler(void) {
    tick_counter++;
}

int main(void) {
    // 1. Habilitar reloj para GPIOA, GPIOB y GPIOC
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;

    // 2. Configurar pines D2 a D6 como salidas (Output mode = 01)
    // D2 = PA10
    GPIOA->MODER &= ~(3U << (10 * 2)); // Limpiar
    GPIOA->MODER |=  (1U << (10 * 2)); // Setear a 01

    // D3 = PB3, D4 = PB5, D5 = PB4, D6 = PB10
    GPIOB->MODER &= ~( (3U << (3*2)) | (3U << (4*2)) | (3U << (5*2)) | (3U << (10*2)) );
    GPIOB->MODER |=  ( (1U << (3*2)) | (1U << (4*2)) | (1U << (5*2)) | (1U << (10*2)) );

    // 3. Configurar PC13 (Blue Button) como entrada
    GPIOC->MODER &= ~(3U << (13 * 2));

    // 4. Configurar temporización de SysTick a 1 milisegundo
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);

    uint8_t button_prev = 1;

    while (1) {
        
        uint32_t current_period = (fast_mode == 1) ? 600 : 1200;
        
        if (tick_counter >= current_period) {
            tick_counter = 0;
            
            uint8_t pat = secuencia_7[current_state];
            
            // Logica para mapear bits del patron a los pines D2-D6
            // LED 1 (Izquierda, Bit 4) -> D2 (PA10)
            if (pat & (1 << 4)) GPIOA->ODR |= (1 << 10); else GPIOA->ODR &= ~(1 << 10);
            
            // LED 2 (Bit 3) -> D3 (PB3)
            if (pat & (1 << 3)) GPIOB->ODR |= (1 << 3);  else GPIOB->ODR &= ~(1 << 3);
            
            // LED 3 (Centro, Bit 2) -> D4 (PB5)
            if (pat & (1 << 2)) GPIOB->ODR |= (1 << 5);  else GPIOB->ODR &= ~(1 << 5);
            
            // LED 4 (Bit 1) -> D5 (PB4)
            if (pat & (1 << 1)) GPIOB->ODR |= (1 << 4);  else GPIOB->ODR &= ~(1 << 4);
            
            // LED 5 (Derecha, Bit 0) -> D6 (PB10)
            if (pat & (1 << 0)) GPIOB->ODR |= (1 << 10); else GPIOB->ODR &= ~(1 << 10);
            
            current_state++;
            if (current_state > 4) {
                current_state = 0;
            }
        }

        // Leer botón PC13, tiene Pull-Up interno por lo que al oprimir es '0'
        uint8_t button_curr = (GPIOC->IDR & (1 << 13)) ? 1 : 0;
        
        if (button_prev == 1 && button_curr == 0) {
            for (volatile int i = 0; i < 50000; i++); 
            if ((GPIOC->IDR & (1 << 13)) == 0) {
                fast_mode = !fast_mode;
                while ((GPIOC->IDR & (1 << 13)) == 0);
            }
        }
        button_prev = button_curr;
    }
}
