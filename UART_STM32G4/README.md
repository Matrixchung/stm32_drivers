# UART Library for STM32G4
**Reasons for non-cross platform: The STM32 LL Library using "DMA_STREAM_x" instead of "DMA_CHANNEL_x" in STM32F4**

#### Usage:

1. Set bi-direction DMA with "**Data Width**" set to **Byte**, and set the RX DMA with Circular Mode, the TX DMA with Normal Mode.
2. Turn off "Force DMA channels Interrupts" in NVIC settings (optimal, since we don't need DMA Channel Interrupts)
3. Enable the corresponding UART Global Interrupt
4. Call `UART_InstanceInit()` with correct arguments, you should first define your instance name in `uart_handler.c`
5. Put `UART_RX_IRQHandler(&UART_InstanceTypeDef)` in corresponding interrupt handler in `stm32xxxx_it.c`
6. Register your own RX callback using `UART_RegisterCallback()` (optional)
