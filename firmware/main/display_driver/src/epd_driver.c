/*****************************************************************************
* | File        :   epd_driver.c
* | Author      :   nonlaminar
* | Function    :   Electronic paper driver
* | Info        :  
*----------------
* | This version:   v1.0
* | Date        :   2026-04-11
* | Info        :   Written specifically for the 7.5 inch V2 Waveshare Epaper
*                   module, TODO: update to be board agnostic!
******************************************************************************/
#include "debug.h"
#include "host_api.h"
#include "epd_driver.h"

static void epd_reset(void)
{
    host_digital_write(EPD_RST_PIN, 1);
    host_delay_ms(20);
    host_digital_write(EPD_RST_PIN, 0);
    host_delay_ms(2);
    host_digital_write(EPD_RST_PIN, 1);
    host_delay_ms(20);
}

static void epd_send_command(UBYTE reg)
{
    host_digital_write(EPD_DC_PIN, 0);
    host_digital_write(EPD_CS_PIN, 0);
    host_spi_write(reg);
    host_digital_write(EPD_CS_PIN, 1);
}

static void epd_send_data(UBYTE reg)
{
    host_digital_write(EPD_DC_PIN, 1);
    host_digital_write(EPD_CS_PIN, 0);
    host_spi_write(reg);
    host_digital_write(EPD_CS_PIN, 1);
}

static void epd_send_datan(UBYTE *pData, UDOUBLE len)
{
    host_digital_write(EPD_DC_PIN, 1);
    host_digital_write(EPD_CS_PIN, 0);
    host_spi_writen(pData, len);
    host_digital_write(EPD_CS_PIN, 1);
}

static void epd_wait_for_idle(void)
{
    DPRINTF("Display Busy\r\n");
    do {
        host_delay_ms(5);
    }while(!(host_digital_read(EPD_BUSY_PIN)));
    host_delay_ms(5);
    DPRINTF("Display busy release\r\n");
}

static void epd_display_refresh(void)
{
    epd_send_command(0x12);
    host_delay_ms(100);
    epd_wait_for_idle();
}

UBYTE epd_init(void)
{
    epd_reset()

    // Power setting
    // VGH=20v, VGL=-20V, VHD = 15V, VDL=-15
    uint8_t data[4] = {0x07, 0x07, 0x3f, 0x3f};
    epd_send_command(0x1);
    epd_send_datan(data, 4);

    // Booster Soft Start
    // PHA[7:6] Soft start period of phase A: 00 (10ms)
    // PHA[5:3] Driving strength of phase A: 010 (3)
    // PHA[2:0] Minimum off time settin of gdrA: 111 (6.58uS)
    // PHB[7:6] SSP of phase B: 00 (10ms)
    // PHB[5:3] Drive strength of PB: 010 (3)
    // PHB[2:0] Min off time set of gdrB: 111 (6.58uS)
    // PHC1[5:3] drive strength PC1: 101 (6)
    // PHC1[2:0] min off time set of gdrc1: 000 (0.27uS)
    // PHC2EN: 0 disable booser phase c2 (always applied)
    // PHC2[5:3] drive strength PC2: 010 (3)
    // PHC2[2:0] min off time set of gdrc1: 111 (6.58uS)
    data = {0x17, 0x17, 0x28, 0x17};
    epd_send_command(0x06);
    epd_send_datan(data, 4);
    
    // Power On
    epd_send_command(0x04);
    host_delay_ms(100);
    epd_wait_until_idle();
    
    // Panel Setting (KW Mode - not KWR Mode)
    epd_send_command(0x00);
    epd_send_data(0x1F);

    // TRES (Resolution setting)
    data = {0x03, 0x20 0x01, 0xE0};
    epd_send_command(0x61);
    epd_send_datan(data, 4);
    
    // Dual Spi Mode 
    // (disable mm input pin definition, and disable dualspi mode)
    epd_send_command(0x15);
    epd_send_data(0x00);

    // VCOM and data interval setting
    // Border LUT = LUTKW (1 -> 0) (DD = 0, BDV=01) (KW Mode = 1)
    // CDI VCOM and data interval is 10
    data = {0x10, 0x07, 0x00, 0x00};
    epd_send_command(0x50);
    epd_send_datan(data, 2);

    // TCON setting - gate/source non-overlap time
    // 0x22 sets S2G and G2S no overlap period to 12
    epd_send_command(0x60);
    epd_send_data(0x22);

    return 0;
}

UBYTE epd_init_fast(void)
{
    epd_reset();
    
    // Panel Setting (KW Mode - not KWR Mode)
    epd_send_command(0x00);
    epd_send_data(0x1F);

    // VCOM and data interval setting
    // Border LUT = LUTKW (1 -> 0) (DD = 0, BDV=01) (KW Mode = 1)
    // CDI VCOM and data interval is 10
    data = {0x10, 0x07, 0x00, 0x00};
    epd_send_command(0x50);
    epd_send_datan(data, 2);

    // Power On
    epd_send_command(0x04);
    host_delay_ms(100);
    epd_wait_until_idle();
 
    // Booster Soft Start
    // PHA[7:6] Soft start period of phase A: 00 (10ms)
    // PHA[5:3] Driving strength of phase A: 100 (5)
    // PHA[2:0] Minimum off time settin of gdrA: 111 (6.58uS)
    // PHB[7:6] SSP of phase B: 00 (10ms)
    // PHB[5:3] Drive strength of PB: 100 (5)
    // PHB[2:0] Min off time set of gdrB: 111 (6.58uS)
    // PHC1[5:3] drive strength PC1: 011 (4)
    // PHC1[2:0] min off time set of gdrc1: 000 (0.27uS)
    // PHC2EN: 0 disable booser phase c2 (always applied)
    // PHC2[5:3] drive strength PC2: 010 (3)
    // PHC2[2:0] min off time set of gdrc1: 111 (6.58uS)
    data = {0x27, 0x27, 0x18, 0x17};
    epd_send_command(0x06);
    epd_send_datan(data, 4);
      
    // Cascade (TFIX=1, CCEN=0)
    epd_send_command(0xE0);
    epd_send_data(0x02);

    // Force temp for cascade mode
    epd_send_command(0xE5);
    epd_send_data(0x5A);

    return 0;
}

UBYTE epd_init_part(void)
{
    epd_reset();

    // Panel Setting (KW Mode - not KWR Mode)
    epd_send_command(0x00);
    epd_send_data(0x1F);

    // Power On
    epd_send_command(0x04);
    host_delay_ms(100);
    epd_wait_until_idle();
 
    // Cascade (TFIX=1, CCEN=0)
    epd_send_command(0xE0);
    epd_send_data(0x02);

    // Force temp for cascade mode
    epd_send_command(0xE5);
    epd_send_data(0x6E);
}

void epd_clear(void)
{
    UWORD width, height;
    width = (EPD_WIDTH % 8 == 0) ? (EPD_WIDTH / 8) : (EPD_WIDTH / 8 + 1);
    height = EPD_HEIGHT;
    
    UBYTE image[EPD_WIDTH / 8] = {0x00};
    
    UWORD i;
    for(i = 0; i < width; ++i)
    {
        image[i] = 0xFF;
    }
    
    // Start data transmission
    epd_send_command(0x10);
    for(i=0; i < height; ++i)
    {
        epd_send_datan(image, width);
    }

    for(i = 0; i < width; ++i)
    {
        image[i] = 0x00;
    }
    
    epd_send_command(0x13);
    for(i=0; i < height; ++i)
    {
        epd_send_datan(image, width);
    }

    // Stop data trasnmission and issue data refresh?
    epd_send_command(0x11);
    epd_send_data(0x80);
}

void epd_display_part(UBYTE *blackimage, UDOUBLE x_start, UDOUBLE y_start, 
                      UDOUBLE x_end, UDOUBLE y_end)
{
    UDOUBLE width, height;
    width = ( (x_end - x_start) % 8 == 0 ) ? ( (x_end - x_start) / 8 ) : 
                                             ( (x_end - x_start) / 8 + 1);

    height = y_end - y_start;
    
    epd_send_command(0x50);
    epd_send_data(0xA9);
    epd_send_data(0x07);
    
    epd_send_command(0x91);
    epd_send_command(0x90);
    epd_send_data(x_start / 256);
    epd_send_data(x_start % 256); // x-start
    
    epd_send_data(x_end / 256);
    epd_send_data(x_end % 256 - 1); // x-end

    epd_send_data(y_start / 256);
    epd_send_data(y_start % 256); // y-start

    epd_send_data(y_end / 256);
    epd_send_data(y_end % 256 - 1); // y-end

    epd_send_data(0x01);
    
    epd_send_command(0x13);
    for (UDOUBLE j = 0; j < height; ++j)
    {
        epd_send_datan((UBYTE *)(blackimage + j * width), width);
    } 

    epd_display_refresh();
}

void epd_sleep(void)
{
    // 
    epd_send_command(0x50);
    epd_send_data(0xF7);
    
    // Power off
    epd_send_command(0x02);
    epd_wait_for_idle();

    // Deep sleep with correct check code
    epd_send_command(0x07);
    epd_send_data(0xA5);
}

