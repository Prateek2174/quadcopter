/*
 * uart_rx_dma.h
 *
 *  Created on: Oct 31, 2026
 *      Author: prate
 */

#ifndef INC_UART_RX_DMA_H_
#define INC_UART_RX_DMA_H_

//TODO: start receive-to-idle DMA,
//		handle the event callback,
//		notify a task from the ISR.

//TODO: Where do the head and tail indexes live, and which side updates each?

//TODO: uart_rx_dma_start,
//		HAL_UARTEx_RxEventCallback (runs in interrupt context),
//		the RX task body, a function that reads new bytes out of the circular buffer

bool uart_rx_dma_start();


#endif /* INC_UART_RX_DMA_H_ */
