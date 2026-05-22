/*****************************************************************************
* | File        :   host_api.h
* | Author      :   em
* | Function    :   Wrappers for host hardware functions
* | Info        :
*                Used to shield the underlying layers of each master
*                and enhance portability
*----------------
* | This version:   V1.0
* | Date        :   2026-11-4
* | Info        :
******************************************************************************/
#ifndef _HOST_API_H_
#define _HOST_API_H_

#include <stdint.h>

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

// TODO: these are Raspberry pi specific
enum host_epd_pins_e
{
    EPD_RST_PIN     = 17,
    EPD_DC_PIN      = 25,
    EPD_CS_PIN      = 8,
    EPD_BUSY_PIN    = 18,
    EPD_PWR_PIN     = 24,
    EPD_MOSI_PIN    = 10,
    EPD_SCLK_PIN    = 11,
};

/**
 * @brief Host hardware read/write wrappers
**/
inline void host_digital_write(UWORD pin, UBYTE val);

inline void host_delay_ms(UDOUBLE xms);

inline void host_spi_write(uint8_t val);

inline void host_spi_writen(uint8_t *pData, uint32_t len);

/**
 * @brief the module init and exit funcs
 *        sets up (and shuts down) the host hardware features
**/
UBYTE host_module_init(void);
void host_module_exit(void);


#endif // ifndef _HOST_API_H_
