/*****************************************************************************
* | File        :   Debug.h
* | Author      :   em
* | Function    :   debug with printf
* | Info        :
*----------------
* | This version:   v1.0
* | Date        :   2026-4-11
* | Info        :   
*   1.USE_DEBUG -> DEBUG, If you need to see the debug information, 
*    clear the execution: make DEBUG=-DDEBUG
******************************************************************************/
#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdio.h>

#ifdef DEBUG
    #define DPRINTF(__info,...) printf("[DEBUG]: " __info, ##__VA_ARGS__)
#else
    #define DPRINTF(__info,...)
#endif // ifdef DEBUG

#endif // ifndef __DEBUG_H
