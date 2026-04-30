/* ==============================================================================
   PROYECTO 01 - CÓDIGO FUENTE (SECUENCIA 5 Y VELOCIDADES)
   ==============================================================================
   Instrucciones:
   NO copies y pegues esto en un solo golpe y borres todo tu main.c de CubeIDE.
   Sigue las referencias de los comentarios "USER CODE BEGIN" para saber 
   exactamente dónde pegar cada pedacito de código en el archivo que 
   te genere STM32CubeIDE.
============================================================================== */

/* --- PEGA ESTO JUSTO EN: USER CODE BEGIN Includes --- */
#include <stdint.h>
/* --- FIN DE PEGA --- */



/* --- PEGA ESTO JUSTO EN: USER CODE BEGIN PV (Private Variables) --- */
// Arreglo de 10 estados para la secuencia "rebote" (Secuencia 5):
const uint8_t secuencia[10] = {0xE0, 0x70, 0x38, 0x1C, 0x0E, 0x07, 0x0E, 0x1C, 0x38, 0x70};

// Arreglo con los 3 niveles de velocidad (en milisegundos):
const uint32_t speed_delays[3] = {1500, 800, 300}; // Nivel 1, Nivel 2, Nivel 3

volatile uint8_t current_speed_level = 0; // Inicia en velocidad más baja (0)
volatile uint32_t current_delay = 1500;   // Retardo actual en milisegundos

uint8_t state_index = 0; // Para recorrer nuestro arreglo de la secuencia
uint32_t last_tick = 0;  // Para nuestro cronómetro no bloqueante
/* --- FIN DE PEGA --- */



/* --- PEGA ESTO JUSTO EN: USER CODE BEGIN 0 --- */
// Función para actualizar los LEDs indicadores de velocidad (PA8, PA9, PA10)
void update_speed_leds(void) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, (current_speed_level == 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, (current_speed_level == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, (current_speed_level == 2) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

// Rutina de interrupción para TODOS los botones (EXTI)
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    static uint32_t last_interrupt_time = 0;
    uint32_t interrupt_time = HAL_GetTick();

    // Comprobación de "Debounce" (Filtro anti-rebote de 200 milisegundos)
    if (interrupt_time - last_interrupt_time > 200) {
        
        // Botón AZUL placa (PB13 ó PC13) - Aumentar Velocidad
        if (GPIO_Pin == GPIO_PIN_13) { 
            if (current_speed_level < 2) {
                current_speed_level++;
            }
        }
        // Botón EXTRA protoboard (PB8) - Reducir Velocidad
        else if (GPIO_Pin == GPIO_PIN_8) { 
            if (current_speed_level > 0) {
                current_speed_level--;
            }
        }
        
        // Actualizamos los retrasos y el indicador visual general de la velocidad
        current_delay = speed_delays[current_speed_level];
        update_speed_leds(); 
    }
    last_interrupt_time = interrupt_time;
}
/* --- FIN DE PEGA --- */



/* --- PEGA ESTO JUSTO EN EL main() ADENTRO DE: USER CODE BEGIN 2 --- */
  // Estado inicial como dicta la rúbrica: 
  // Comenzar en el primer estado y en la velocidad más baja.
  current_speed_level = 0;
  current_delay = speed_delays[0];
  state_index = 0;
  update_speed_leds();
  
  // Imprimir el primer estado instantáneamente antes de empezar a rotar
  GPIOC->ODR = (GPIOC->ODR & 0xFF00) | secuencia[state_index];
  last_tick = HAL_GetTick();
/* --- FIN DE PEGA --- */



/* --- PEGA ESTO ADENTRO DEL BUCLE while (1) ESTANDO EN: USER CODE BEGIN WHILE --- */
    // Cronómetro no bloqueante usando GetTick
    if (HAL_GetTick() - last_tick >= current_delay) {
        last_tick = HAL_GetTick(); 
        
        // 1. Aumentamos la posición de la escena
        state_index++;
        if (state_index >= 10) {
            state_index = 0; // Si llegamos al tope, empezamos del principio
        }
        
        // 2. Extraemos el patrón hexadecimal (P.ej. 0xE0) y limpiamos todo a cero de PC0 a PC7.
        uint8_t out_val = secuencia[state_index];
        
        // 3. Empujamos el valor de 1s y 0s de un solo golpe al PuertoC
        // Magia con bits: Conservamos PC8-15 puros (con & 0xFF00) y le pegamos ('|') nuestros 8 LEDS de estado.
        GPIOC->ODR = (GPIOC->ODR & 0xFF00) | out_val;
    }
/* --- FIN DE PEGA --- */
