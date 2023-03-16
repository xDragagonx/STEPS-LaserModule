#include "stm32f091xc.h"
#include "motor.h" //is nodig voor de functie declaratie
#include "usart2.h" //is nodig voor de initialisatie
#include "stdbool.h" //is nodig voor booleans
#include "buttons.h" //is nodig voor de userbutton



void MotorSpins(){
	//InitUsart2(9600);
	GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODER4) | GPIO_MODER_MODER4_0;		//Poort B4(D5)		Set als output
	GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODER10) | GPIO_MODER_MODER10_0;	//Poort B10(D6)		idem
	
	
			GPIOB->ODR = GPIOB->ODR | GPIO_ODR_10;		//draai tegenwijzerszin
			GPIOB->ODR = GPIOB->ODR & ~GPIO_ODR_4; 
};

void MotorStops(){
	InitUsart2(9600);
	GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODER4) | GPIO_MODER_MODER4_0;		//Poort B4(D5)		Set als output
	GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODER10) | GPIO_MODER_MODER10_0;	//Poort B10(D6)		idem
	
	GPIOB->ODR = GPIOB->ODR & ~GPIO_ODR_10;		//Motor uit
	GPIOB->ODR = GPIOB->ODR & ~GPIO_ODR_4;
};