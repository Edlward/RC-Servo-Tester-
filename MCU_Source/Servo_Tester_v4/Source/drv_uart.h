#pragma once


#define BUFFER_SIZE     MAX_QUEUE_SIZE
/* Exported variables ---------------------------------------------------------*/
extern QueueType Uart1RxQueue, Uart1TxQueue;

void USART1_Init(void);

/* Exported functions ------------------------------------------------------- */
void UART_Init(uint8_t uart,uint32_t baud);
ErrorStatus UART1_PutChar(const char ascii);
ErrorStatus UART1_GetChar(char *data);
void UART1_PutStr(char *string);
void UART1_printf(const char *fmt,...);

