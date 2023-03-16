#include "stm32f091xc.h"
#include "laserRight.h" //is nodig voor de functie declaratie
#include "usart2.h" //is nodig voor de initialisatie
#include "stdbool.h" //is nodig voor booleans


bool LaserRightDetected(){
	InitUsart2(9600);
	
	//Lees de A1 pin uit.
		if((GPIOA->IDR & GPIO_IDR_1) != GPIO_IDR_1)
		{
			//StringToUsart2("1\r\n");
		return true;
		}
		else
		{
			//tringToUsart2("0\r\n");
			return false;
		}
};