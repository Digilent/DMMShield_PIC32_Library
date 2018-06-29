/* ************************************************************************** */
/** Descriptive File Name

  @Company
 Digilent

  @File Name
    utils.h

  @Description
        This file contains the declaration for additional utilities functions used in various other modules.
        Include the file in the project when is needed.

  @Author
    Cristian Fatu 
    cristian.fatu@digilent.ro

  @Versioning:
 	 Cristian Fatu - 2018/06/29 - Initial release, DMMShield Library

 */
/* ************************************************************************** */

#ifndef _UTILS_H    /* Guard against multiple inclusion */
#define _UTILS_H

void DelayAprox10Us( unsigned int tusDelay );
uint8_t GetBufferChecksum(uint8_t *pBuf, int len);
#endif /* _UTILS_H */

/* *****************************************************************************
 End of File
 */
