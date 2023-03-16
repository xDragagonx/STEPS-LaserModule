#include "stm32f091xc.h"
#include "magnetRight.h" //is nodig voor de functie declaratie
#include "usart2.h" //is nodig voor de initialisatie
#include "stdbool.h" //is nodig voor booleans


bool HallSensorRightDetected(){
	//InitUsart2(9600);
	
	//Lees de A3 pin uit.
		if((GPIOB->IDR & GPIO_IDR_0) != GPIO_IDR_0)
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