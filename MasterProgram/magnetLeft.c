#include "stm32f091xc.h"
#include "magnetLeft.h" //is nodig voor de functie declaratie
#include "usart2.h" //is nodig voor de initialisatie
#include "stdbool.h" //is nodig voor booleans


bool HallSensorLeftDetected(){
	//InitUsart2(9600);
	
	//Lees de A4 pin uit.
		if((GPIOC->IDR & GPIO_IDR_1) != GPIO_IDR_1)
		{
			//StringToUsart2("1\r\n");
			return true;
		}
		else
		{
			//StringToUsart2("0\r\n");
			return false;
		}
};