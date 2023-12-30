#ifndef _UART_HANDLER_H
#define _UART_HANDLER_H
#include "usart.h"
#include "kfifo.h"
#include <string.h>
#include <stdlib.h>
// #include "stdio.h"
#include "utils.h"
#include "printf.h"
#define RX_BUFFER_SIZE 128
#define TX_BUFFER_SIZE 128
#define TX_USE_DMA 1
typedef void (*UART_RX_Callback)();
// use dma normal mode to prevent overrun
typedef struct UART_InstanceTypeDef
{
    USART_TypeDef *UARTx;
    DMA_TypeDef *RX_DMAx;
    uint32_t RX_DMA_Stream;
    DMA_TypeDef *TX_DMAx;
    uint32_t TX_DMA_Stream;
    uint16_t rx_stack_size; // size to reach a complete packet
    char rx_buffer[RX_BUFFER_SIZE+1];
    kfifo_t rx_fifo;
    volatile uint8_t rx_finished_flag;
    char tx_buffer[TX_BUFFER_SIZE+1];
    kfifo_t tx_fifo;
    UART_RX_Callback rx_cb;
}UART_InstanceTypeDef;

__STATIC_INLINE uint32_t UART_GetRxLen(UART_InstanceTypeDef *uart_instance)
{
    return kfifo_used(&uart_instance->rx_fifo);
}

__STATIC_INLINE uint32_t UART_GetTxLen(UART_InstanceTypeDef *uart_instance)
{
    return kfifo_used(&uart_instance->tx_fifo);
}

__STATIC_INLINE char UART_PeekRxFifo(UART_InstanceTypeDef *uart_instance)
{
    // return fifo_s_preread(&uart_instance->rx_fifo, ptr);
    char temp;
    return kfifo_peek(&uart_instance->rx_fifo, &temp, 1);
}

__STATIC_INLINE void UART_GetRxFifo(UART_InstanceTypeDef *uart_instance, char *p_dest, uint32_t len)
{
    kfifo_get(&uart_instance->rx_fifo, p_dest, len);
}

__STATIC_INLINE void UART_FlushRxFifo(UART_InstanceTypeDef *uart_instance)
{
    __attribute__((unused)) uint32_t u32wk0;
    // u32wk0 = uart_instance->UARTx->TDR; // Clear data register
    u32wk0 = uart_instance->UARTx->RDR; // Clear data register
    kfifo_flush(&uart_instance->rx_fifo);
    uart_instance->rx_finished_flag = 0;
}

__STATIC_INLINE void UART_PutTxFifo(UART_InstanceTypeDef *uart_instance, char *p_src, uint32_t len)
{
    kfifo_put(&uart_instance->tx_fifo, p_src, len);
}

__STATIC_INLINE void UART_FlushTxFifo(UART_InstanceTypeDef *uart_instance)
{
    kfifo_flush(&uart_instance->tx_fifo);
}

__STATIC_INLINE void UART_RegisterRxCallback(UART_InstanceTypeDef *uart_instance, UART_RX_Callback func)
{
    uart_instance->rx_cb = func;
}

extern UART_InstanceTypeDef UART1_Handler;

void UART_InstanceInit(UART_InstanceTypeDef *uart_instance, USART_TypeDef *UARTx, DMA_TypeDef *RX_DMAx, uint32_t RX_Stream, DMA_TypeDef *TX_DMAx, uint32_t TX_Stream, uint16_t stack_size);
void UART_printf(UART_InstanceTypeDef *uart_instance, uint8_t transmit, char *fmt, ...);
void UART_TransmitFromFifo(UART_InstanceTypeDef *uart_instance);
void UART_TransmitFromBuffer(UART_InstanceTypeDef *uart_instance, char *p_src, uint32_t len);
void UART_RX_IRQHandler(UART_InstanceTypeDef *uart_instance);
#endif