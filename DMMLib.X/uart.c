/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    uart.c

  @Description
        This file groups the functions that implement the UART functionality.
        This module implements the UART1 functionality connected to the USB - UART 
        interface of the uc32 board. It provides basic functions to configure UART and  
        transmit / receive functions. The module initializes the UART to generate interrupt when a character is received.
        In the interrupt handler, the array of characters are stored as commands, using a circular buffer. 
        A command is a sequence of characters terminated by one of '\r', '\n', or both. 
        The "Interrupt service routines" section contains the Uart1 interrupt Handler function, 
        that implements the UART receive mode if the interrupt mode is chosen.
        The "Interface functions" section groups functions that can also be called by User.
        The "Local functions" section groups low level functions that are only called from within current module. 
    
       		

  @Author
    Cristian Fatu 
    cristian.fatu@digilent.ro

  @Versioning:
 	 Cristian Fatu - 2018/06/29 - Initial release, DMMShield Library

 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <xc.h>
#include <sys/attribs.h>
#include "gpio.h"
#include "uart.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions Prototypes                                        */
/* ************************************************************************** */
/* ************************************************************************** */

void UART_ConfigureUartRXInt(unsigned int baud);
void UART_PutChar(char ch);
void UART_InitCircBuffer();
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Global Variables                                                  */
/* ************************************************************************** */
/* ************************************************************************** */
// global variables, to communicate between interrupt handler and other 
volatile uint8_t fRxLock = 0;
volatile uint8_t ichRxCol;
#define RX_NOLINES 10
char rgchRxn[RX_NOLINES][cchRxMax];
//circular buffer definition
typedef struct circ_buf
{
    uint8_t ichRxLineWR;    
    uint8_t ichRxLineRD;    
    int cntRxLines;
} CIRC_BUF;

volatile CIRC_BUF cb;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interrupt service routines                                        */
/* ************************************************************************** */
/* ************************************************************************** */

/* ------------------------------------------------------------ */
/***	Uart1Handler
**
**	Description:
**		This is the interrupt handler for UART1 RX. It is used to receive strings using interrupt method. 
**      The interrupt recognizes a string having up to cchRxMax - 1/2 characters, followed by a CR or LF.
**      When available received bytes are found they are placed in a global string rgchRxn, for each line.
**      When a carriage return or a line feed ("\r", "\n", CR/LF) sequence is recognized, the interrupt handler 
**      flags the passing to a new line, takes the new string value and performs specific checks for circular buffer - eg. full.
**          
*/

void __ISR(_UART_1_VECTOR, ipl6) Uart1Handler (void)
{
	uint8_t bVal;
	uint8_t fNewLine;
	//Read the Uart1 RX buffer while data is available
	while(U1STAbits.URXDA)
	{
		bVal = (uint8_t)U1RXREG;
		
		if(!fRxLock)
	    {	
            fNewLine = 0;
	        // Do we have space to store another character?
	        if (cchRxMax <= ichRxCol)
	        {
	            // No.
                fNewLine = 1;
                // step back to accommodate incoming terminator
                ichRxCol--;
            }
            // Is this one of the line terminator chars ?
            if(('\n' == bVal ) || ('\r' == bVal) )
            {
	            // Yes.
                fNewLine = 1;
            }
            if(fNewLine)
            {
                if(ichRxCol > 0)
                {
                    // ignore new line if it comes after empty content (second of the '\n' '\r' pair)
                    rgchRxn[cb.ichRxLineWR][ichRxCol] = '\0';
                    // move to new line 
                    ichRxCol = 0;
                    if(cb.cntRxLines == RX_NOLINES)
                    {
                        // circ buffer full, no new line available
                        fRxLock = 1;
                    }
                    else
                    {
                        //increase the number of line counter and check if maximum has been reached
                        cb.cntRxLines++;
                        if ((++cb.ichRxLineWR >= RX_NOLINES))
                        {
                            cb.ichRxLineWR = 0;                        
                        }                            
                    }
                }
	        }  
            else
            {
                //store the UART content in the new line allocated space
                rgchRxn[cb.ichRxLineWR][ichRxCol++] = bVal;
            }
    	}  	
    }  
	//Clear the Uart1 interrupt flag.
	IFS0bits.U1RXIF = 0;
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */
/***	UART_Init
**
**	Parameters:
**		unsigned int baud - UART baud rate.
**                                     for example 9600 corresponds to 9600 baud			
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the UART1 hardware interface involved in the UART module, 
**      the UART receive with interrupt mode.
**      The UART_TX digital pin is configured as digital output.
**      The UART_RX digital pin is configured as digital input.
**      The UART_TX and UART_RX are mapped over the UART1 interface.
**      The UART1 module of PIC32 is configured to work at the specified baud, no parity and 1 stop bit.
**      
**          
*/
void UART_Init(unsigned int baud)
{
    UART_ConfigureUartRXInt(baud);
}


/***	UART_PutString
**
**	Parameters:
**          char szData[] -   the zero terminated string containing characters to be transmitted over UART.
**
**	Return Value:
**		
**
**	Description:
**		This function transmits all the characters from a zero terminated string over UART1. The terminator character is not sent.
**      
**          
*/
void UART_PutString(char szData[])
{
    char *pData = szData;
    while(*pData)
    {
        UART_PutChar((*(pData++)));
        DelayAprox10Us(10);
    }
}

/***	UART_GetString
**
**	Parameters:
**		char* pchBuff   - pointer to a char buffer to hold the received zero terminated string 
**		int cchBuff     - size of the buffer to hold the zero terminated string
**          
**
**	Return Value:
**          uint8_t     -The receive status
**                  > 0 - the number of characters contained in the string
**                  0	- a CR/LF terminated string hasn't been received               
**
**	Description:
**		This function provides a zero terminated string received over UART1  
**      which was placed in the circular buffer by the UART interrupt handler.
**		If a received string is available in the circular buffer, the string
**		is copied in the pchBuff string and its length is returned.
**		Otherwise, the function returns 0.
**          
*/
uint8_t UART_GetString(char* pchBuff, int cchBuff )
{
	uint8_t ich;
	uint8_t cmdLen;
	
	// Have we finished receiving a CR/LF terminated string via UART1?
	if(cb.cntRxLines <= 0)
	{
		return 0;
	}
    cmdLen = strlen(rgchRxn[cb.ichRxLineRD]);
	// Does the user buffer have enough space to store the CR/LF terminated string?
	if(cchBuff < cmdLen)
	{
		// A buffer underrun occurred.
		cmdLen = cchBuff;
	}

    // copy the received chars to the destination location.
	for(ich = 0; ich <= cmdLen; ich++)
	{
		*pchBuff = rgchRxn[cb.ichRxLineRD][ich]; 
		pchBuff++;
	}

    // move to next
    cb.cntRxLines--;
    if ((++cb.ichRxLineRD >= RX_NOLINES))
    {
        cb.ichRxLineRD = 0;                        
    }                            
    fRxLock = 0;

	return ich;
}


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */


/***	UART_ConfigureUartRXInt
**
**	Parameters:
**		unsigned int baud - UART baud rate.
**                                     for example 115200 corresponds to 115200 baud
**
**	Return Value:
**		
**
**	Description:
**		This function configures the UART1 hardware interface of PIC32, according 
**      to the provided baud rate, no parity and 1 stop bit (by calling the UART_ConfigureUart function)
**      and additionally configures the interrupt on RX.
**      This is a low-level function called by initialization functions, so user should avoid calling it directly.   
**      
**          
*/
void UART_ConfigureUartRXInt(unsigned int baud)
{

    U1MODEbits.ON     = 0;
    U1MODEbits.SIDL   = 0;
    U1MODEbits.IREN   = 0; 
    U1MODEbits.RTSMD  = 0;
    U1MODEbits.UEN0   = 0; 
    U1MODEbits.UEN1   = 0;
    U1MODEbits.WAKE   = 0;
    U1MODEbits.LPBACK = 0; 
    U1MODEbits.ABAUD  = 0;
    U1MODEbits.RXINV  = 0; 
    U1MODEbits.PDSEL1 = 0; 
    U1MODEbits.PDSEL0 = 0; 
    U1MODEbits.STSEL  = 0;  

    
    U1MODEbits.BRGH   = 0; 

    U1BRG = (int)(((float)PB_FRQ/(16*baud) - 1) + 0.5); // add 0.5 just in order to implement the round using the floor approach

    U1STAbits.UTXEN    = 1;
    U1STAbits.URXEN    = 1;
    U1MODEbits.ON      = 1; 

    
    IPC6bits.U1IP = 6;
    IPC6bits.U1IS = 3;

	IFS0bits.U1RXIF = 0;    //Clear the Uart1 interrupt flag.
    IEC0bits.U1RXIE = 1;    // enable RX interrupt

    macro_enable_interrupts();  // enable interrupts 
    
    // configure circular buffer
    UART_InitCircBuffer();
}


/***	UART_PutChar
**
**	Parameters:
**          char ch -   the character to be transmitted over UART.
**
**	Return Value:
**		
**
**	Description:
**		This function transmits a character over UART1. 
**      
**          
*/
void UART_PutChar(char ch)
{
    while(U1STAbits.UTXBF == 1);
    U1TXREG = ch;
}

/***	UART_InitCircBuffer
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the variables used in the circular buffer approach, the lines and columns counters 
**      It is called when the UART interface is first initialized.
**     
**          
*/
void UART_InitCircBuffer()
{
    ichRxCol = 0;
    cb.ichRxLineWR = 0;
    cb.cntRxLines = 0;
}


/* *****************************************************************************
 End of File
 */
