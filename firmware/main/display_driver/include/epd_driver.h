/*****************************************************************************
* | File        :   epd_driver.h
* | Author      :   nonlaminar
* | Function    :   driver for the E paper display
* | Info        :
*----------------
* | This version:   V1.0
* | Date        :   2026-11-4
* | Info        :
******************************************************************************/
#ifndef _EPD_DRIVER_H_
#define _EPD_DRIVER_H_
#include "host_api.c"

UBYTE epd_init(void);
UBYTE epd_init_fast(void);
UBYTE epd_init_part(void);
void epd_clear(void);
void epd_display_part(UBYTE *blackimage, UDOUBLE x_start, UDOUBLE y_start, 
                      UDOUBLE x_end, UDOUBLE y_end);
void epd_sleep(void);
#endif // ifndef _EPD_DRIVER_H_
