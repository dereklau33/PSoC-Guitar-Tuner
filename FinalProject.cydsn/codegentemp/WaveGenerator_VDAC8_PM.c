/*******************************************************************************
* File Name: WaveGenerator_VDAC8_PM.c  
* Version 1.90
*
* Description:
*  This file provides the power management source code to API for the
*  VDAC8.  
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "WaveGenerator_VDAC8.h"

static WaveGenerator_VDAC8_backupStruct WaveGenerator_VDAC8_backup;


/*******************************************************************************
* Function Name: WaveGenerator_VDAC8_SaveConfig
********************************************************************************
* Summary:
*  Save the current user configuration
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void WaveGenerator_VDAC8_SaveConfig(void) 
{
    if (!((WaveGenerator_VDAC8_CR1 & WaveGenerator_VDAC8_SRC_MASK) == WaveGenerator_VDAC8_SRC_UDB))
    {
        WaveGenerator_VDAC8_backup.data_value = WaveGenerator_VDAC8_Data;
    }
}


/*******************************************************************************
* Function Name: WaveGenerator_VDAC8_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void WaveGenerator_VDAC8_RestoreConfig(void) 
{
    if (!((WaveGenerator_VDAC8_CR1 & WaveGenerator_VDAC8_SRC_MASK) == WaveGenerator_VDAC8_SRC_UDB))
    {
        if((WaveGenerator_VDAC8_Strobe & WaveGenerator_VDAC8_STRB_MASK) == WaveGenerator_VDAC8_STRB_EN)
        {
            WaveGenerator_VDAC8_Strobe &= (uint8)(~WaveGenerator_VDAC8_STRB_MASK);
            WaveGenerator_VDAC8_Data = WaveGenerator_VDAC8_backup.data_value;
            WaveGenerator_VDAC8_Strobe |= WaveGenerator_VDAC8_STRB_EN;
        }
        else
        {
            WaveGenerator_VDAC8_Data = WaveGenerator_VDAC8_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: WaveGenerator_VDAC8_Sleep
********************************************************************************
* Summary:
*  Stop and Save the user configuration
*
* Parameters:  
*  void:  
*
* Return: 
*  void
*
* Global variables:
*  WaveGenerator_VDAC8_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void WaveGenerator_VDAC8_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(WaveGenerator_VDAC8_ACT_PWR_EN == (WaveGenerator_VDAC8_PWRMGR & WaveGenerator_VDAC8_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        WaveGenerator_VDAC8_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        WaveGenerator_VDAC8_backup.enableState = 0u;
    }
    
    WaveGenerator_VDAC8_Stop();
    WaveGenerator_VDAC8_SaveConfig();
}


/*******************************************************************************
* Function Name: WaveGenerator_VDAC8_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  WaveGenerator_VDAC8_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void WaveGenerator_VDAC8_Wakeup(void) 
{
    WaveGenerator_VDAC8_RestoreConfig();
    
    if(WaveGenerator_VDAC8_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        WaveGenerator_VDAC8_Enable();

        /* Restore the data register */
        WaveGenerator_VDAC8_SetValue(WaveGenerator_VDAC8_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
