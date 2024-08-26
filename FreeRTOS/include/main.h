#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"
#include "board.h"

/*
 * Exception handlers.
 */
void xPortPendSVHandler( void );
void xPortSysTickHandler( void );
void vPortSVCHandler( void );


#endif

