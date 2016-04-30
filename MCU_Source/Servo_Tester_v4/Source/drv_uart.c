#include "board.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
QueueType Uart1RxQueue, Uart1TxQueue;
//QueueType Uart2RxQueue, Uart2TxQueue;
//QueueType Uart3RxQueue, Uart3TxQueue;

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/*****************************************************************************
* Descriptions  : Retargets the C library printf function to the USART.
* Parameters    : None
* Return Value  : None
*****************************************************************************/
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
//   USART_SendData(USART1, (uint8_t) ch);
    UART1_PutChar(ch);
    /* Loop until the end of transmission */
 //  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
 //   while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
   // {}    
       
    return ch;
}


/*****************************************************************************
* Descriptions  : UART1으로 1문자 출력
* Parameters    : 1byte 문자(ascii code)
* Return Value  : 에러 상태 반환 (SUCCESS, ERROR)
*****************************************************************************/

ErrorStatus UART1_PutChar(char ascii)
{    
    if(enqueue(&Uart1TxQueue, ascii) != QUEUE_SUCCESS)
    {            
        return ERROR;        
    }               
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    
    return SUCCESS;            
}



/*****************************************************************************
* Descriptions  : UART1으로 수신버퍼에서 1문자 가져오기
* Parameters    : 반환할 문자
* Return Value  : 에러 상태 반환(SUCCESS, ERROR)
*****************************************************************************/
ErrorStatus UART1_GetChar(char *ascii)
{          
    if(dequeue(&Uart1RxQueue, (unsigned char *)ascii) == QUEUE_SUCCESS) 
        return SUCCESS;
    else return ERROR;
}

/*****************************************************************************
* Descriptions  : UART1으로 문자열 출력
* Parameters    : 출력할 문자열
* Return Value  : None
*****************************************************************************/
void UART1_PutStr(char *string)
{
	while(*string != '\0') UART1_PutChar(*(string++));
}


/*******************************************************************************
* Function Name  : SerialPutChar
* Description    : Print a character on the HyperTerminal
* Input          : - c: The character to be printed
* Output         : None
* Return         : None
*******************************************************************************/
void SerialPutChar(char c)
{
    USART_SendData(USART1, c);
    while ((USART1->ISR & USART_ISR_TXE ) != USART_ISR_TXE );
}


	
void USART1_Init(void){
	USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	//  USART_InitTypeDef USART_InitStructur;
	USART_ClockInitTypeDef USART_ClockInitStructur;

	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOA Clocks enable */
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);


	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);			//TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);			//RX


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9  ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10  ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	RCC_USARTCLKConfig(RCC_USART1CLK_PCLK);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_ClockInitStructur.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructur.USART_CPOL = USART_CPOL_High;
	USART_ClockInitStructur.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructur.USART_LastBit = USART_LastBit_Enable;
	USART_ClockInit(USART1, &USART_ClockInitStructur);

	USART_InitStructure.USART_BaudRate = UART1_BAURD;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);



/* Enable the USARTx */
  USART_Cmd(USART1, ENABLE);
/* Enable USART2 Receive interrupt */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);           

}



