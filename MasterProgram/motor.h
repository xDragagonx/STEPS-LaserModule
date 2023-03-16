#include "stm32f091xc.h"
#include "stdbool.h" //is nodig voor booleans

#if !defined(MOTOR_DEFINED)
	#define MOTOR_DEFINED
	
	void MotorSpins();
	void MotorStops();
#endif