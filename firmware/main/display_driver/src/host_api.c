/*****************************************************************************
* | File        :   host_api.c
* | Author      :   em
* | Function    :   Wrappers for hardware specific functions
* | Info        :
*----------------
* | This version:   v1.0
* | Date        :   2026-11-4
* | Info        :   
******************************************************************************/
#include "debug.h"

/* TODO: Host device specific (raspberry pi) */
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "host_api.h"

inline void host_digital_write(UWORD pin, UBYTE val)
{
    digitalWrite(pin, value);
}

inline UBYTE host_digital_read(UWORD pin)
{
    return digitalRead(pin);
}

inline void host_spi_write(uint8_t val)
{
    wiringPiSPIDataRW(0, &val, 1);    
}

inline void host_spi_writen(uint8_t *pData, uint32_t len)
{
    wiringPiSPIDataRW(0, pData, len);
}

void host_gpio_mode(UWORD pin, UWORD mode)
{
    if (mode == 0 || mode == INPUT)
    {
        pinMode(pin, INPUT);
        pullUpDnControl(pin, PUD_UP);
    } else {
        pinMode(pin, OUTPUT);
    }
    
}

inline void host_delay_ms(UDOUBLE xms)
{
    delay(xms);
}

/**
 * @breif detects operating system from `/etc/issue` file
 *        and will ensure that the firmware is built for the right host
**/
static int host_equiptment_testing(void)
{
    FILE *fp;
    char issue_str[64];
    
    // Create and/or test file
    fp = fopen("/etc/issue", "r");

    if (fp == NULL)
    {
        DPRINTF("Unable to open /etc/issue");
        return -1;
    }
    
    if (fread(issue_str, 1, sizeof(issue_str), fp) <= 0)
    {
        DPRINTF("Unale to read from /etc/issue");
        return -1;
    }    
    
    issue_str[sizeof(issue_str)-1] = '\0';
    fclose(fp);
    
    printf("Current environment: ");

//#ifdef RPI (TODO: Host platform Specific)
    char systems[][9] = {"Raspbian", "Debian", "NixOS"};
    int detected = 0;
    
    for (int i=0; i < 3; ++i)
    {
        if (strstr(issue_str, systems[i] != NULL) 
        {
            printf("%s\n", systems[i]);     
            detected = 1;
        }
    }
    
    if (!detected) {
        printf("not recognized\n");
        printf("Built for Raspberry Pi, but unable to detect environment.\n");
        printf("Perhaps you meant to build for a different platform?\n");
        return -1;
    }
    
    return 0; 
}

static void host_GPIO_init(void)
{
    // OUTPUT = 1, INPUT = 0
    host_gpio_mode(EPD_BUSY_PIN, 0);
    host_gpio_mode(EPD_RST_PIN, 1);
    host_gpio_mode(EPD_DC_PIN, 1);
    host_gpio_mode(EPD_CS_PIN, 1);
    host_gpio_mode(EPD_PWR_PIN, 1); 

    host_digital_write(EPD_CS_PIN, 1);
    host_digital_write(EPD_PWR_PIN, 1);
}

UBYTE host_module_init(void)
{
    printf("/***********************************/ \r\n");

    if (DEV_Equipment_Testing() < 0) {
        return 1;
    }
    
// TODO: Raspberry pi specific
    if (wiringPiSetupGpio() < 0) 
    {
        printf("wiringPiSetupGPIO failed !!! \r\n");
        return 1;
    } else {
        DPRINTF("wiringPiSetupGPIO: done\n");
    }

    host_GPIO_ini();
    WiringPiSPISetup(0, 10000000);

    printf("/***********************************/ \r\n");
    return 0;
}

void host_module_exit(void)
{
    host_digital_write(EPD_CS_PIN, 0);
    host_digital_write(EPD_PWR_PIN, 0);
    host_digital_write(EPD_DC_PIN, 0);
    host_digital_write(EPD_RST_PIN, 0);
}
