/*******************************************************************************
* File Name: WaveSelector_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "WaveSelector.h"

/* Check for removal by optimization */
#if !defined(WaveSelector_Sync_ctrl_reg__REMOVED)

static WaveSelector_BACKUP_STRUCT  WaveSelector_backup = {0u};

    
/*******************************************************************************
* Function Name: WaveSelector_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void WaveSelector_SaveConfig(void) 
{
    WaveSelector_backup.controlState = WaveSelector_Control;
}


/*******************************************************************************
* Function Name: WaveSelector_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void WaveSelector_RestoreConfig(void) 
{
     WaveSelector_Control = WaveSelector_backup.controlState;
}


/*******************************************************************************
* Function Name: WaveSelector_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void WaveSelector_Sleep(void) 
{
    WaveSelector_SaveConfig();
}


/*******************************************************************************
* Function Name: WaveSelector_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void WaveSelector_Wakeup(void)  
{
    WaveSelector_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
