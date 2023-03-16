#include "stm32f091xc.h"
#include "stdbool.h" //is nodig voor booleans

#if !defined(MAGNETLEFT_DEFINED)
	#define MAGNETLEFT_DEFINED
	
	bool HallSensorLeftDetected();
#endif