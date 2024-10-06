#include "cmsis_os.h"
#include <stdio.h>
#include <stdlib.h>
#include "GLCD.h"
#include "KBD.h"
#include "corgi2.c"
#include "bffr.c"
#include "glass.c"
#include "minesweeper3.h"
#include "usbdmain.h"

#define __FI 1 


uint32_t dir;																	
int option = 1;												
int photo = 1;													


osTimerId InputTimer;																		

void callback(void const *param){												
	dir = get_button();
}

osTimerDef(InputTimer_Handle, callback);								

void menu (void const *argument);
void mp3( void const *argument);
void photoGallery (void const *argument);
void minesweeper (void const *argument);

osThreadId menu_tid;
osThreadId photoGallery_tid;
osThreadId mp3_tid;
osThreadId minesweeper_tid;

osThreadDef (menu, osPriorityAboveNormal, 1, 0);
osThreadDef (photoGallery, osPriorityNormal, 1, 0);
osThreadDef (mp3, osPriorityNormal, 1, 0);
osThreadDef (minesweeper, osPriorityNormal, 1, 0);


int Init_Thread (void) {
  
	menu_tid = osThreadCreate (osThread(menu), NULL);
	photoGallery_tid = osThreadCreate (osThread(photoGallery), NULL);
	mp3_tid = osThreadCreate (osThread(mp3), NULL);
	minesweeper_tid = osThreadCreate (osThread(minesweeper), NULL);
	InputTimer = osTimerCreate(osTimer(InputTimer_Handle), osTimerPeriodic, (void *)0);
	osTimerStart(InputTimer, 1);
  
  return(0);
}

void menu (void const *argument) {
	  for(;;) {
			GLCD_SetBackColor(Black);
			GLCD_SetTextColor(White);
			GLCD_DisplayString(0, 0, __FI, " COE718 Media Center");
			GLCD_DisplayString(1, 0, __FI, "        Balraj      ");
			GLCD_DisplayString(2, 0, __FI, "      Main Menu     ");
			GLCD_SetBackColor(White);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString(5, 0, __FI, "  Photo Gallery");			
			GLCD_DisplayString(6, 0, __FI, "  MP3 Player");
			GLCD_DisplayString(7, 0, __FI, "  Tic Tac Toe");			

		if(dir == KBD_DOWN){
				option++;
		}
		if(dir == KBD_UP){
				option--;
		}
		if(option > 3){
				option=1;
		}
		if(option < 1){
				option=3;
		}

	
		if(dir == KBD_SELECT){
				GLCD_Clear(White);
				if(option == 1){
				option=1;
				osSignalSet(photoGallery_tid, 0x01);						
				osSignalWait (0x01,osWaitForever);
				}
				if(option == 2){
				option=1;
				osSignalSet(mp3_tid, 0x01);
				osSignalWait (0x01,osWaitForever);
				}
				if(option == 3){
				option=1;
				osSignalSet(minesweeper_tid, 0x01);
				osSignalWait (0x01,osWaitForever);
				}
		}			
		
		
		if(option == 1){
				GLCD_SetTextColor(Black);
				GLCD_DisplayString(5, 0, __FI,">");
				GLCD_DisplayString(6, 0, __FI," ");
				GLCD_DisplayString(7, 0, __FI," ");
		}
		if(option == 2){
				GLCD_SetTextColor(Black);
				GLCD_DisplayString(5, 0, __FI," ");
				GLCD_DisplayString(6, 0, __FI,">");
				GLCD_DisplayString(7, 0, __FI," ");
		}
		if(option == 3){
				GLCD_SetTextColor(Black);
				GLCD_DisplayString(5, 0, __FI," ");
				GLCD_DisplayString(6, 0, __FI," ");
				GLCD_DisplayString(7, 0, __FI,">");
		}
  }
}

void photoGallery (void const *argument) {
	if(osSignalWait(0x01, osWaitForever).status == osEventSignal){		
		for(;;) {
			
			GLCD_DisplayString(9, 0, __FI,"<");
			GLCD_DisplayString(9, 17, __FI,">");
			GLCD_DisplayString(9, 2, __FI,"Push to Exit!");
			
			if(dir == KBD_SELECT){
					GLCD_Clear(White);
					osSignalSet(menu_tid, 0x01);
					osSignalWait (0x01,osWaitForever);
			}

			if(dir == KBD_RIGHT){
					photo++;
			}else if(dir == KBD_LEFT){
					photo--;
			}

			if(photo > 4){
					photo=1;
			}else if(photo < 1){
					photo=4;
			}
			
			switch(photo){
				case 1:
					//GLCD_Clear(White);
					GLCD_Bitmap (  0,   0, 300,  206, CORGI_PIXEL_DATA);
					break;
			
				case 2:
					//GLCD_Clear(White);
					GLCD_Bitmap (  0,   0, 300,  206, BFFR_PIXEL_DATA);
					break;
				
				case 3:
					//GLCD_Clear(White);
					GLCD_Bitmap (0,  0, 300,  206, GLASS_PIXEL_DATA);
					break;
				
				default:
					break;
			}

		}  
	}	
}


void mp3 (void const *argument) {
	int x;
	if(osSignalWait(0x01, osWaitForever).status == osEventSignal){
		
			GLCD_SetBackColor(Black);
			GLCD_SetTextColor(White);
			GLCD_DisplayString(0, 0, __FI, " COE718 Media Center");
			GLCD_DisplayString(1, 0, __FI, "        Balraj      ");
			GLCD_DisplayString(2, 0, __FI, "      MP3 Player    ");
			GLCD_SetBackColor(White);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString(6, 0, __FI, "     Now Playing!   ");
			GLCD_DisplayString(9, 0, __FI, "    Push to Exit!   ");
			usbd_main();
	}	                                         
}

void minesweeper (void const *argument){
	int x;
	if(osSignalWait(0x01, osWaitForever).status == osEventSignal){
	 if(play() == 0){
			GLCD_Clear(White);
			osSignalSet(menu_tid, 0x01);
			osSignalWait (0x01,osWaitForever);
	 }
	}	     
}
