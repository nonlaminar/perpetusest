/*****************************************************************************
* | File        :   EPD_DISPLAY_TEST.c
* | Author      :   nonlaminar
* | Function    :   7.5inch e-paper test demo
* | Info        :
*----------------
* | This version:   v1.0
* | Date        :   2026-04-11
* | Info        :
******************************************************************************/
#include <time.h>
#include <stdlib.h>
#include "host_api.h"
#include "epd_display.h"

int DISPLAY_TEST(void)
{
    printf("Display Test Demo \r\n");

    if (host_module_init() != 0) {
        printf("HOST MODULE INIT ISSUE\n");
        return -1;
    }
    
    printf("e-Paper init and clear... \r\n");
    epd_init();

    struct timespec start={0, 0}, finish={0,0};
    clock_gettime(CLOCK_REALTIME, &start);
    epd_clear();
    clock_gettime(CLOCK_REALTIME, &finish);
    printf("%ld S\r\n", finish.tv_sec - start.tv_sec);

    host_delay_ms(500);

    printf("Clear... \r\n");
    epd_init();
    epd_clear();
    
    printf("Goto Sleep... \r\n");
    epd_sleep();
    
    host_delay_ms(2000);

    printf("Close 5V, module enters 0power consumption ...\r\n");
    host_module_exit();

    return 0;
}

int main(void)
{
    DISPLAY_TEST();    
    return 0;
}
