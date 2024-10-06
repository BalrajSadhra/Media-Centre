#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "GLCD.h"
#include "KBD.h"
#include "LPC17xx.h"                        /* LPC17xx definitions */
#include <stdlib.h>
#include "LED.h"

extern int Init_Thread (void);

int main (void) {
	GLCD_Init();
	LED_Init();
	LED_Off(0);
	GLCD_Clear(White);
	KBD_Init();
  osKernelInitialize ();                    // initialize CMSIS-RTOS
 	Init_Thread ();
  osKernelStart ();                         // start thread execution 
	osDelay(osWaitForever);
}
