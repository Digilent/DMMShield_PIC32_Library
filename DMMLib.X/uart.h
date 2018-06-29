/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    uart.h

  @Description
        This file groups the declarations of the functions that implement
        the UART module (defined in uart.c).
        Include the file in the project when this module is needed.
        Use #include "uart.h" in the source files where the functions are needed.

  @Versioning:
 	 Cristian Fatu - 2018/06/29 - Initial release, DMMShield Library

 */
/* ************************************************************************** */

#ifndef _UART_H    /* Guard against multiple inclusion */
#define _UART_H

#define	cchRxMax 0x40	// maximum number of characters a CR+LF terminated string

void UART_Init(unsigned int baud);
void UART_PutString(char szData[]);
uint8_t UART_GetString( char* pchBuff, int cchBuff );




//#ifdef __cplusplus
//extern "C" {
//#endif



    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************



    /* Provide C++ Compatibility */
//#ifdef __cplusplus
//}
//#endif
#endif /* _UART_H */

/* *****************************************************************************
 End of File
 */
