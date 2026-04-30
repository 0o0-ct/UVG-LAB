#ifndef UART_LAB04_H
#define UART_LAB04_H

#include <stdint.h>

/*
 * Driver UART/USART2 (STM32F446) — CMSIS / Registros
 * Placa: NUCLEO-F446RE
 * USART2: PA2 = TX, PA3 = RX, AF7
 */

void uart2_init(uint32_t baud, uint8_t enable_rx);
void uart2_write_char(char c);
void uart2_write_str(const char *s);

/* RX (polling) */
char uart2_read_char_blocking(void);   /* espera RXNE */

/* Utilidad: enviar número decimal por UART (opcional) */
void uart2_write_u32(uint32_t v);

#endif
