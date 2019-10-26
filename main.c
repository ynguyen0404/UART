#include "stm32f4xx.h"
#include "stdio.h"
#include "string.h"


#define STR_MAX_LENGTH 100
#define TC_BIT 0x0040
#define TC_SHIFT_LEFT 6
#define RXNE_BIT 0x0020
#define RXNE_SHIFT_LEFT 5



void uart_my_init(void);
void uart_my_send_str_polling(const char * str);
void uart_my_receive_str_polling(char * str, uint8_t strLen);

void GPIO_my_Init(void);
void Led_on(void);
void Led_off(void);

void uart_my_init(){
	GPIO_InitTypeDef gpio_init;
	USART_InitTypeDef usart_init;
	
	GPIO_StructInit(&gpio_init);
	USART_StructInit(&usart_init);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	
	gpio_init.GPIO_Mode = GPIO_Mode_AF;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &gpio_init);
	
	usart_init.USART_BaudRate = 9600;
	usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart_init.USART_Parity = USART_Parity_No;
	usart_init.USART_StopBits = USART_StopBits_1;
	usart_init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &usart_init);
	
	USART_Cmd(USART3, ENABLE);
}

void uart_my_send_str_polling(const char * str)
{
	if(str != NULL)
	{
		uint8_t strIndex = 0;
		uint8_t strLen = strlen(str);
		for(strIndex = 0; strIndex < strLen; strIndex++)
		{
			//Send
			USART_SendData(USART3, (uint16_t)str[strIndex]);
			//Waits
			while(((USART3->SR & TC_BIT) >> TC_SHIFT_LEFT) == 0)
			{
				// Do thing
			}
		}
	}
}

void uart_my_receive_str_polling(char * str, uint8_t strLen)
{
	if(str != NULL)
	{
		uint8_t strIndex = 0;
		for (strIndex = 0; strIndex < strLen; strIndex++)
		{
			while(((USART3->SR & RXNE_BIT)>>RXNE_SHIFT_LEFT)==0)
			{
				//Do thing
			}
			str[strIndex] = (char)USART_ReceiveData(USART3);
		}
	}
}
void GPIO_my_Init(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOG, &GPIO_InitStruct);
}
void Led_on(){
	GPIO_SetBits(GPIOG,GPIO_Pin_13);
}
void Led_off(){
	GPIO_WriteBit(GPIOG, GPIO_Pin_13,0);
}
int main()
{
	char* string1 = " Choose your option(1,2,..) \r\n 1. Student info \r\n 2. Basic operation \r\n 3. Simple led \r\n 4. Advance led \r\n 5. Audio\r\n    *****Y_Nguyen***** \r\n";
	char* string2 = " 1. Student info \r\n ID:xxx \r\n Full name: yyy \r\n ESC: return previous menu\r\n";
	char* string3 = " 2. Basic operation \r\n a. Plus \r\n b. Subtract \r\n c. Multiple\r\n d.Divide \r\n e. Moudule \r\n";
	char* string4 = " 3. Simple led \r\n a. Led on \r\n b. Led off \r\n";
	char* string5 = " 4. Advance led \r\n a. Set led \r\n b. Set direction \r\n c. Start \r\n ESC: return previous menu\r\n";
	char* string6 = " 5. Audio\r\n Playing... \r\n ESC: return previous menu\r\n";
	char* stringagaint = " ------->Choose againt\r\n";
	//char* inputCharMsg = "Input your character: ";
	//char* dontSupportStr = "Don't support";
	//char* visibleCharStr = "Visible char";
	//char* invisibleCharStr = "Invisible char";
	//char* newLineStr = "\r\n";
	
	char charInput;
	char charInputStr[STR_MAX_LENGTH];
	
	GPIO_my_Init();
	uart_my_init();
	uart_my_send_str_polling(string1);
	for(;;)
	{
			uart_my_receive_str_polling(charInputStr,1);
			charInput = charInputStr[0];	
			if(charInput == 49) 			//Student info
			{
					uart_my_send_str_polling(string2);
					uart_my_receive_str_polling(charInputStr,1);
			}
			else if (charInput == 50) //Basic operation
			{
					uart_my_send_str_polling(string3);
					uart_my_receive_str_polling(charInputStr,1);
			}
			else if (charInput == 51) //Simple led
			{
					uart_my_send_str_polling(string4);
					uart_my_receive_str_polling(charInputStr,1);
					charInput = charInputStr[0];
					/*if(charInput == 97)
						{
							Led_on();					
							uart_my_receive_str_polling(charInputStr,1);
							charInput = charInputStr[0];
						}
					else if(charInput == 98)
						{
							Led_off();
							uart_my_receive_str_polling(charInputStr,1);
						}
					else
						{
						uart_my_send_str_polling(stringagaint);
						}*/
						while(charInput != 27)
						{
										if(charInput == 97)
									{
										Led_on();					
										uart_my_receive_str_polling(charInputStr,1);
										charInput = charInputStr[0];
									}
								else if(charInput == 98)
									{
										Led_off();
										uart_my_receive_str_polling(charInputStr,1);
									}
								else
									{
									uart_my_send_str_polling(stringagaint);
									}
						}
					
			}
			else if(charInput == 52) //Advance led
			{
					uart_my_send_str_polling(string5);
					uart_my_receive_str_polling(charInputStr,1);
			}
			else if(charInput == 53) //5. Audio
			{
					uart_my_send_str_polling(string6);
					uart_my_receive_str_polling(charInputStr,1);
			}
			else
				{
					uart_my_send_str_polling(stringagaint);
				}
				
		
		
		
		
		/*charInput = charInputStr[0];
		if((charInput>=32) && (charInput<126))
		{
			uart_my_send_str_polling(charInputStr);
			uart_my_send_str_polling(newLineStr);
		}
		else
		{
			uart_my_send_str_polling(dontSupportStr);
			uart_my_send_str_polling(newLineStr);
		}*/
	}
	return 0;
}