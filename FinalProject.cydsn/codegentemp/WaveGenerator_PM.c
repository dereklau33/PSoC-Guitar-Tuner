/*******************************************************************************
* File Name: WaveGenerator_PM.c  
* Version 2.10
*
* Description:
*  This file provides the power manager source code to the API for 
*  the WaveDAC8 component.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "WaveGenerator.h"

static WaveGenerator_BACKUP_STRUCT  WaveGenerator_backup;


/*******************************************************************************
* Function Name: WaveGenerator_Sleep
********************************************************************************
*
* Summary:
*  Stops the component and saves its configuration. Should be called 
*  just prior to entering sleep.
*  
* Parameters:  
*  None
*
* Return: 
*  None
*
* Global variables:
*  WaveGenerator_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void WaveGenerator_Sleep(void) 
{
	/* Save DAC8's enable state */

	WaveGenerator_backup.enableState = (WaveGenerator_VDAC8_ACT_PWR_EN == 
		(WaveGenerator_VDAC8_PWRMGR_REG & WaveGenerator_VDAC8_ACT_PWR_EN)) ? 1u : 0u ;
	
	WaveGenerator_Stop();
	WaveGenerator_SaveConfig();
}


/*******************************************************************************
* Function Name: WaveGenerator_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component configuration. Should be called
*  just after awaking from sleep.
*  
* Parameters:  
*  None
*
* Return: 
*  void
*
* Global variables:
*  WaveGenerator_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void WaveGenerator_Wakeup(void) 
{
	WaveGenerator_RestoreConfig();

	if(WaveGenerator_backup.enableState == 1u)
	{
		WaveGenerator_Enable();
	}
}


/* [] END OF FILE */
