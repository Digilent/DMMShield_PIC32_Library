/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    dmmcmd.h

  @Description
        This file contains the declaration for the functions of the DMMCMD module.

  @Versioning:
 	 Cristian Fatu - 2018/06/29 - Initial release, DMMShield Library

 */
/* ************************************************************************** */

#ifndef _DMMCMD_H    /* Guard against multiple inclusion */
#define _DMMCMD_H


//#ifdef __cplusplus
//extern "C" {
//#endif
// *****************************************************************************
// *****************************************************************************
// Section: Type definitions
// *****************************************************************************
// *****************************************************************************
typedef enum {
	CMD_NONE = -1, // No command
	INVALID = 0, // Invalid command
/* the implemented commands keys are below */
	CMD_Config,
	CMD_CalibP,
	CMD_CalibN,
	CMD_CalibZ,
	CMD_MeasureRep,
	CMD_MeasureStop,
	CMD_MeasureRaw,
	CMD_MeasureAvg,
	CMD_SaveEPROM,
	CMD_VerifyEPROM,
	CMD_ExportCalib,
	CMD_ImportCalib,
	CMD_MeasureForCalibP,
	CMD_MeasureForCalibN,
	CMD_FinalizeCalibP,
	CMD_FinalizeCalibN,
	CMD_RestoreFactCalibs,
	CMD_ReadSerialNo

} cmd_key_t;

// structure mapping command key to command string
typedef struct {
	char *pchCmd;
	cmd_key_t eCmd;
} cmd_map_t;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************


/************************** Function Prototypes ******************************/
uint8_t DMMCMD_Init();
void DMMCMD_CheckForCommand();


    /* Provide C++ Compatibility */
//#ifdef __cplusplus
//}
//#endif
#endif /* _DMMCMDJA_H */

/* *****************************************************************************
 End of File
 */
