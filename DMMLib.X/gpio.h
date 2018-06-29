/* ************************************************************************** */
/** Descriptive File Name

  @Company
 Digilent

  @File Name
    gpio.h

  @Description
        This file contains the definitions needed for GPIO module.

  @Author
    Cristian Fatu 
    cristian.fatu@digilent.ro

  @Versioning:
 	 Cristian Fatu - 2018/06/29 - Initial release, DMMShield Library

 */

#ifndef CONFIG_H
#define	CONFIG_H

#define PB_FRQ  10000000

#define macro_enable_interrupts() \
{  unsigned int val = 0;\
    asm volatile("mfc0 %0,$13":"=r"(val));  \
    val |= 0x00800000;  \
    asm volatile("mtc0 %0,$13" : "+r"(val)); \
    INTCONbits.MVEC = 1; \
__builtin_enable_interrupts(); }

//#define macro_enable_interrupts INTEnableSystemMultiVectoredInt()

#define macro_disable_interrupts __builtin_disable_interrupts()
//#define macro_disable_interrupts INTDisableInterrupts()

/*
#ifdef	__cplusplus
extern "C" {
#endif
*/
#define tris_LD5    TRISFbits.TRISF0
#define  lat_LD5    LATFbits.LATF0
//#define tris_LD4    TRISGbits.TRISG6
//#define  lat_LD4    LATGbits.LATG6

//define CTRL signals for relays
#define tris_CTRL_RLU    TRISDbits.TRISD0   // IO3
//#define  lat_CTRL_RLU    LATDbits.LATD0

#define tris_CTRL_RLD    TRISFbits.TRISF1   // IO4
//#define  lat_CTRL_RLD    LATFbits.LATF1

#define tris_CTRL_RLI    TRISDbits.TRISD8   // IO2
//#define  lat_CTRL_RLI    LATDbits.LATD8

// DMM SPI slave select - corresponds to schematic signal CS_DMM
#define tris_SPI_SS    TRISDbits.TRISD4
//#define  lat_SPI_SS    LATDbits.LATD4

// EPROM SPI slave select - corresponds to schematic signal CS_EPROM
#define tris_ESPI_SS    TRISDbits.TRISD3
//#define  lat_ESPI_SS    LATDbits.LATD3

// corresponds to schematic signal CLK
#define tris_SPI_CLK   TRISGbits.TRISG6
//#define  lat_SPI_CLK   LATGbits.LATG6

// corresponds to schematic signal DI
#define tris_SPI_MOSI   TRISGbits.TRISG7
//#define  lat_SPI_MOSI   LATGbits.LATG7

// corresponds to schematic signal DO
#define tris_SPI_MISO   TRISGbits.TRISG8
//#define  prt_SPI_MISO   PORTGbits.RG8


// UART
#define tris_UART_TX   TRISFbits.TRISF3
#define tris_UART_RX   TRISFbits.TRISF2


#define GPIO_SetValue_CS_EPROM(val) \
		LATDbits.LATD3 = val

#define GPIO_SetValue_CS_DMM(val) \
		LATDbits.LATD4 = val

#define GPIO_SetValue_CLK(val) \
		LATGbits.LATG6 = val

#define GPIO_SetValue_MOSI(val) \
		LATGbits.LATG7 = val

#define GPIO_SetValue_RLD(val) \
		LATFbits.LATF1 = val

#define GPIO_SetValue_RLU(val) \
		LATDbits.LATD0 = val

#define GPIO_SetValue_RLI(val) \
		LATDbits.LATD8 = val

#define GPIO_Get_MISO() \
        PORTGbits.RG8


/*
#ifdef	__cplusplus
}
#endif
*/
#endif	/* CONFIG_H */

