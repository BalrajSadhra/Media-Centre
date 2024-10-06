#ifndef TTTH
#define TTTH

#include <stdio.h>
#include "GLCD.h"
#include "KBD.h"

extern char square[10];
extern uint32_t joy;

int checkwin();
void board();
int play();

#endif
