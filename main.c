#include "stm32f4xx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

#define STR_MAX_LENGTH 100
#define TC_BIT 0x0040
#define TC_SHIFT_LEFT 6
#define RXNE_BIT 0x0020
#define RXNE_SHIFT_LEFT 5

void uart_my_init(void);
void uart_my_send_str_polling(const char * str);
void uart_my_receive_str_polling(char * str, uint8_t strLen);

void Main_menu();
void GPIO_my_Init(void);
void Led_on(void);
void Led_off(void);
void Basic_plus();

void send1char(int a)    // gui 1 so nguyen int
	{
		int dem = -1, i = 0;
		char A[100];
		while(a>0){
			A[i++] = a%10;
			a/=10;
			dem++;
		}
		for(i = dem; i>=0; i--){
			char b = A[i]+'0';
			uart_my_send_str_polling(&b);
		}
	}

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

void uart_my_send_str_polling(const char * str){
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

void uart_my_receive_str_polling(char * str, uint8_t strLen){
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

void Student_info(){
		char* string2 = " 1. Student info \r\n ID:xxx \r\n Full name: yyy \r\n 0. Return previous menu\r\n";
		uart_my_send_str_polling(string2);
}

void Basic_operation(){
	char* string3 = " 2. Basic operation \r\n a. Plus \r\n b. Subtract \r\n c. Multiple\r\n d.Divide \r\n e. Moudule \r\n";
	char* stringagaint = " ------->Choose againt\r\n";
	char charInput;
	char charInputStr[STR_MAX_LENGTH];
	uart_my_send_str_polling(string3);
	for(;;)
	{
			uart_my_receive_str_polling(charInputStr,1);
			charInput = charInputStr[0];
			switch(charInput){
				case 97:
				{
					Basic_plus();
					break;
				}
				case 98:
				{
					Basic_plus();
					break;
				}
				case 99:
				{
					Basic_plus();
					break;
				}
				case 100:
				{
					Basic_plus();
					break;
				}
				case 101:
				{
					Basic_plus();
					break;
				}
				case 48:
					{
						Main_menu();  //sai, tro ve basic operation moi dung
						break;
					}
				default:
					{
						uart_my_send_str_polling(stringagaint);
					}
			}
	}
}

void Simple_led(){
	char* string4 = " 3. Simple led \r\n a. Led on \r\n b. Led off \r\n 0. Return previous menu\r\n";
	uart_my_send_str_polling(string4);
}

void Advance_led(){
	char* string5 = " 4. Advance led \r\n a. Set led \r\n b. Set direction \r\n c. Start \r\n ESC: return previous menu\r\n";
	uart_my_send_str_polling(string5);
}

void Audio(){
	char* string6 = " 5. Audio\r\n Playing... \r\n ESC: return previous menu\r\n";
	uart_my_send_str_polling(string6);
}
void Basic_plus(){
	char * string7 = " a. Plus, 0. Return previous menu\r\n";
	char * string8 = " Operand 1: ";
	char * string9 = " Operand 2: ";
	char * string10 = " Result: ";
	char * newline = " \r\n"; 
	char charInput;
	char charInputStr[STR_MAX_LENGTH];
	memset(charInputStr, 0, STR_MAX_LENGTH);
	int a,b, sum =0;
	uart_my_send_str_polling(string7);
	uart_my_send_str_polling(string8);
	uart_my_receive_str_polling(charInputStr,1);
	if(charInput != 48)
	{
		uart_my_send_str_polling(charInputStr);   //output operand1 to terminal
		charInput = charInputStr[0];
		uart_my_send_str_polling(newline);
		a = atoi(charInputStr);			//convert  charInputStr -> int

		uart_my_send_str_polling(string9);
		uart_my_receive_str_polling(charInputStr,1);
		uart_my_send_str_polling(charInputStr);   //output operand2 to terminal
		charInput = charInputStr[0];			
		uart_my_send_str_polling(newline);

		b = atoi(charInputStr);    //convert  charInputStr -> int
		sum  = a+b;

		uart_my_send_str_polling(string10);
		send1char(sum);  // hien thi ket qua len Terminal
		uart_my_send_str_polling(newline);		
		uart_my_receive_str_polling(charInputStr,1);
	}
	else
		Basic_operation();
}

void Main_menu(){
	char* string1 = " Choose your option(1,2,..) \r\n 1. Student info \r\n 2. Basic operation \r\n 3. Simple led \r\n 4. Advance led \r\n 5. Audio\r\n    *****Y_Nguyen***** \r\n";
	char* stringagaint = " ------->Choose againt\r\n";
	char charInput;
	char charInputStr[STR_MAX_LENGTH];
	uart_my_send_str_polling(string1);
	for(;;){
		uart_my_receive_str_polling(charInputStr,1);
		charInput = charInputStr[0];
		switch(charInput){
			case 49:
			{
				Student_info();
				for(;;)
				{
					uart_my_receive_str_polling(charInputStr,1);
					charInput = charInputStr[0];
					if(charInput == 48)
						Main_menu();
					else
						uart_my_send_str_polling(stringagaint);			
				}
				break;
			}
			case 50:
			{
				Basic_operation();
				break;
			}
			case 51:
			{
				Simple_led();
				for(;;){
					uart_my_receive_str_polling(charInputStr,1);
					charInput = charInputStr[0];
					switch(charInput){
						case 97:
							{
								Led_on();
								break;
							}
						case 98:
							{
								Led_off();
								break;
							}
						case 48:
							{
								Main_menu();
								break;
							}
							default:
							{
								uart_my_send_str_polling(stringagaint);
							}
					}
				}
				break;
			}
			case 52:
			{
				Advance_led();
				uart_my_receive_str_polling(charInputStr,1);
				break;
			}
			case 53:
			{
				Audio();
				uart_my_receive_str_polling(charInputStr,1);
				break;
			}
			case 48:   // nhan 0 tro ve main menu
			{
				Main_menu();
				break;
			}
			default:
				uart_my_send_str_polling(stringagaint);
	}
}
}

int main(){
	GPIO_my_Init();
	uart_my_init();
	Main_menu();
}
