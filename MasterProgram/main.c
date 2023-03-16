// Basiscode voor het starten van eender welk project op een Nucleo-F091RC met Nucleo Extension Shield V2.
//
// OPM:
//	- via 'Project -> Manage -> Select software packs' kies je bij Keil::STM32F0xx_DFP voor versie 2.0.0.
//	- via 'Options for Target ...' zet je de compiler op C99 (en eventueel op AC5-like warnings).
// 
// Versie: 20220906

// Includes.
#include "stm32f091xc.h"
#include "stdio.h"
#include "stdbool.h"
#include "leds.h"
#include "buttons.h"
#include "usart2.h"
#include "ad.h"
#include <time.h>
#include <stdlib.h>
#include "laserRight.h"
#include "laserLeft.h"
#include "magnetLeft.h"
#include "magnetRight.h"
#include "motor.h"


// Functie prototypes.
void SystemClock_Config(void);
void InitIo(void);
void WaitForMs(uint32_t timespan);
void CountToTen();

// Variabelen aanmaken. 
// OPM: het keyword 'static', zorgt ervoor dat de variabele enkel binnen dit bestand gebruikt kan worden.
static uint8_t count = 0;
static volatile uint32_t ticks = 0;
static bool sw2Previous = true;
int rand(void);
void srand( unsigned int seed );
int counter = 0;
int ledCounter = 0;
bool switch1Pressed = false;
int led[8] = {1,2,4,8,16,32,64,128};

// Entry point.
int main(void)
{
	// Initialisaties.
	SystemClock_Config();
	InitIo();
	InitButtons();
	InitLeds();
	InitUsart2(9600);
	InitAd();
	// Clock voor GPIOA inschakelen.
	//RCC->AHBENR = RCC->AHBENR | RCC_AHBENR_GPIOAEN;
	
		// Alle pinnen met drukknoppen verbonden, op input zetten.
//	GPIOA->MODER = GPIOA->MODER & ~GPIO_MODER_MODER1;
//	GPIOA->MODER = GPIOA->MODER & ~GPIO_MODER_MODER4;	
//	GPIOB->MODER = GPIOB->MODER & ~GPIO_MODER_MODER0;
//	GPIOC->MODER = GPIOC->MODER & ~GPIO_MODER_MODER1;
//	GPIOC->MODER = GPIOC->MODER & ~GPIO_MODER_MODER13;

	//note to laserreceiver, receiver mid in A4, Receiver left in gnd, receiver right in 3v3.
	//note to lasersender, laser left in 3v3, laser right in gnd, laser mid nowhere.
		
	// Laten weten dat we opgestart zijn, via de USART2 (USB).
	StringToUsart2("Reboot\r\n");
	// Oneindige lus starten.
	while(1)
	{
		//a1 pin - rechter laser
		if(LaserRightDetected() == true){
			StringToUsart2("LASER RIGHT - Sending data to wifi mod.\r\n");
		}
		//a2 pin - linker laser
		if(LaserLeftDetected() == true){
			StringToUsart2("LASER LEFT - Sending data to wifi mod.\r\n");
		}
		
		//a3 pin - rechter magneet
		if(HallSensorRightDetected() == true){
			//Why does MotorSpins(); not work if there is StringToUsart2("text")?
			MotorSpins();
			StringToUsart2("MAGNET RIGHT\r\n");
			//StringToUsart2("MAGNET RIGHT\r\n");
			//StringToUsart2("Motor should've run\r\n");
		}
		else{
			MotorStops();
		}
		//a4 pin - linker magneet
		if(HallSensorLeftDetected() == true){
			StringToUsart2("MAGNET LEFT\r\n");
			MotorSpins();
		}else{
			MotorStops();
		}
	}
}
void TIM7_IRQHandler(void)
{
	if((TIM7->SR & TIM_SR_UIF) == TIM_SR_UIF)
	{
		TIM7->SR &= ~TIM_SR_UIF;

		ToggleLed(1);
//		ToggleLed(8);
	}
}
// Functie om extra IO's te initialiseren.
void InitIo(void)
{

}

// Handler die iedere 1ms afloopt. Ingesteld met SystemCoreClockUpdate() en SysTick_Config().
void SysTick_Handler(void)
{
	ticks++;
}

// Wachtfunctie via de SysTick.
void WaitForMs(uint32_t timespan)
{
	uint32_t startTime = ticks;
	
	while(ticks < startTime + timespan);
}

// Klokken instellen. Deze functie niet wijzigen, tenzij je goed weet wat je doet.
void SystemClock_Config(void)
{
	RCC->CR |= RCC_CR_HSITRIM_4;														// HSITRIM op 16 zetten, dit is standaard (ook na reset).
	RCC->CR  |= RCC_CR_HSION;																// Internal high speed oscillator enable (8MHz)
	while((RCC->CR & RCC_CR_HSIRDY) == 0);									// Wacht tot HSI zeker ingeschakeld is
	
	RCC->CFGR &= ~RCC_CFGR_SW;															// System clock op HSI zetten (SWS is status geupdatet door hardware)	
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);	// Wachten to effectief HSI in actie is getreden
	
	RCC->CR &= ~RCC_CR_PLLON;																// Eerst PLL uitschakelen
	while((RCC->CR & RCC_CR_PLLRDY) != 0);									// Wacht tot PLL zeker uitgeschakeld is
	
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_PREDIV;								// 01: HSI/PREDIV selected as PLL input clock
	RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV2;										// prediv = /2		=> 4MHz
	RCC->CFGR |= RCC_CFGR_PLLMUL12;													// PLL multiplied by 12 => 48MHz
	
	FLASH->ACR |= FLASH_ACR_LATENCY;												//  meer dan 24 MHz, dus latency op 1 (p 67)
	
	RCC->CR |= RCC_CR_PLLON;																// PLL inschakelen
	while((RCC->CR & RCC_CR_PLLRDY) == 0);									// Wacht tot PLL zeker ingeschakeld is

	RCC->CFGR |= RCC_CFGR_SW_PLL; 													// PLLCLK selecteren als SYSCLK (48MHz)
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);	// Wait until the PLL is switched on
		
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;												// SYSCLK niet meer delen, dus HCLK = 48MHz
	RCC->CFGR |= RCC_CFGR_PPRE_DIV1;												// HCLK niet meer delen, dus PCLK = 48MHz	
	
	SystemCoreClockUpdate();																// Nieuwe waarde van de core frequentie opslaan in SystemCoreClock variabele
	SysTick_Config(48000);																	// Interrupt genereren. Zie core_cm0.h, om na ieder 1ms een interrupt 
																													// te hebben op SysTick_Handler()
}
