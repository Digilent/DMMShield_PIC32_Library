/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    gpio.c

  @Description
        This file groups the functions that implement GPIO module.
 Basically there is one initialization function that initializes the digital pins involved in DMMShield.
       		

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
#include "stdint.h"
#include "gpio.h"
/* ************************************************************************** */

/***	GPIO_Init
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the digital pins used by DMMShield: 
**      The following digital pins are configured as digital outputs: SPI_CLK, SPI_MOSI, CS_EPROM, CS_DMM.
**      The following digital pins are configured as digital inputs: SPI_MISO.
**      The CS_EPROM and CS_DMM pins are deactivated.
**      This function is not intended to be called by user, as it is an internal low level function.
**      This function is called by SPI_Init().
**      The function guards against multiple calls using a static flag variable.
**          
*/
void GPIO_Init()
{
    static uint8_t fInitialized = 0;
    if(!fInitialized)
    {
        // Configure SPI signals as digital outputs.
        tris_SPI_CLK = 0;
        tris_SPI_MOSI = 0;
        // Configure SPI signals as digital inputs.
        tris_SPI_MISO = 1;
        //configure CTRL signals as outputs
        tris_CTRL_RLU = 0;
        tris_CTRL_RLD = 0;
        tris_CTRL_RLI = 0;    
        
        // Configure DMM Slave Select as digital output.
        tris_SPI_SS = 0;
        // // Deactivate CS_DMM
        GPIO_SetValue_CS_DMM(1); 
        
        // Configure EPROM Slave Select as digital output.
        tris_ESPI_SS = 0;
        
        // Deactivate EPROM SS
        GPIO_SetValue_CS_EPROM(0); 

        // configure relays as digital output
        tris_CTRL_RLU = 0;
        tris_CTRL_RLD = 0;
        tris_CTRL_RLI = 0;
        
        

        
        fInitialized = 1;
    }
}
/* *****************************************************************************
 End of File
 */
