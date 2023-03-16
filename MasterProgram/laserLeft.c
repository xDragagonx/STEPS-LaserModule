#include "stm32f091xc.h"
#include "laserLeft.h" //is nodig voor de functie declaratie
#include "usart2.h" //is nodig voor de initialisatie
#include "stdbool.h" //is nodig voor booleans


bool LaserLeftDetected(){
	InitUsart2(9600);
	
	//Lees de A2 pin uit.
		if((GPIOA->IDR & GPIO_IDR_4) == GPIO_IDR_4)
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