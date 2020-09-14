/*******************************************************************************
* File Name: WaveGenerator_DacClk.h
* Version 2.10
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_WaveGenerator_DacClk_H)
#define CY_CLOCK_WaveGenerator_DacClk_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void WaveGenerator_DacClk_Start(void) ;
void WaveGenerator_DacClk_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void WaveGenerator_DacClk_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void WaveGenerator_DacClk_StandbyPower(uint8 state) ;
void WaveGenerator_DacClk_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 WaveGenerator_DacClk_GetDividerRegister(void) ;
void WaveGenerator_DacClk_SetModeRegister(uint8 modeBitMask) ;
void WaveGenerator_DacClk_ClearModeRegister(uint8 modeBitMask) ;
uint8 WaveGenerator_DacClk_GetModeRegister(void) ;
void WaveGenerator_DacClk_SetSourceRegister(uint8 clkSource) ;
uint8 WaveGenerator_DacClk_GetSourceRegister(void) ;
#if defined(WaveGenerator_DacClk__CFG3)
void WaveGenerator_DacClk_SetPhaseRegister(uint8 clkPhase) ;
uint8 WaveGenerator_DacClk_GetPhaseRegister(void) ;
#endif /* defined(WaveGenerator_DacClk__CFG3) */

#define WaveGenerator_DacClk_Enable()                       WaveGenerator_DacClk_Start()
#define WaveGenerator_DacClk_Disable()                      WaveGenerator_DacClk_Stop()
#define WaveGenerator_DacClk_SetDivider(clkDivider)         WaveGenerator_DacClk_SetDividerRegister(clkDivider, 1u)
#define WaveGenerator_DacClk_SetDividerValue(clkDivider)    WaveGenerator_DacClk_SetDividerRegister((clkDivider) - 1u, 1u)
#define WaveGenerator_DacClk_SetMode(clkMode)               WaveGenerator_DacClk_SetModeRegister(clkMode)
#define WaveGenerator_DacClk_SetSource(clkSource)           WaveGenerator_DacClk_SetSourceRegister(clkSource)
#if defined(WaveGenerator_DacClk__CFG3)
#define WaveGenerator_DacClk_SetPhase(clkPhase)             WaveGenerator_DacClk_SetPhaseRegister(clkPhase)
#define WaveGenerator_DacClk_SetPhaseValue(clkPhase)        WaveGenerator_DacClk_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(WaveGenerator_DacClk__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define WaveGenerator_DacClk_CLKEN              (* (reg8 *) WaveGenerator_DacClk__PM_ACT_CFG)
#define WaveGenerator_DacClk_CLKEN_PTR          ((reg8 *) WaveGenerator_DacClk__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define WaveGenerator_DacClk_CLKSTBY            (* (reg8 *) WaveGenerator_DacClk__PM_STBY_CFG)
#define WaveGenerator_DacClk_CLKSTBY_PTR        ((reg8 *) WaveGenerator_DacClk__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define WaveGenerator_DacClk_DIV_LSB            (* (reg8 *) WaveGenerator_DacClk__CFG0)
#define WaveGenerator_DacClk_DIV_LSB_PTR        ((reg8 *) WaveGenerator_DacClk__CFG0)
#define WaveGenerator_DacClk_DIV_PTR            ((reg16 *) WaveGenerator_DacClk__CFG0)

/* Clock MSB divider configuration register. */
#define WaveGenerator_DacClk_DIV_MSB            (* (reg8 *) WaveGenerator_DacClk__CFG1)
#define WaveGenerator_DacClk_DIV_MSB_PTR        ((reg8 *) WaveGenerator_DacClk__CFG1)

/* Mode and source configuration register */
#define WaveGenerator_DacClk_MOD_SRC            (* (reg8 *) WaveGenerator_DacClk__CFG2)
#define WaveGenerator_DacClk_MOD_SRC_PTR        ((reg8 *) WaveGenerator_DacClk__CFG2)

#if defined(WaveGenerator_DacClk__CFG3)
/* Analog clock phase configuration register */
#define WaveGenerator_DacClk_PHASE              (* (reg8 *) WaveGenerator_DacClk__CFG3)
#define WaveGenerator_DacClk_PHASE_PTR          ((reg8 *) WaveGenerator_DacClk__CFG3)
#endif /* defined(WaveGenerator_DacClk__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define WaveGenerator_DacClk_CLKEN_MASK         WaveGenerator_DacClk__PM_ACT_MSK
#define WaveGenerator_DacClk_CLKSTBY_MASK       WaveGenerator_DacClk__PM_STBY_MSK

/* CFG2 field masks */
#define WaveGenerator_DacClk_SRC_SEL_MSK        WaveGenerator_DacClk__CFG2_SRC_SEL_MASK
#define WaveGenerator_DacClk_MODE_MASK          (~(WaveGenerator_DacClk_SRC_SEL_MSK))

#if defined(WaveGenerator_DacClk__CFG3)
/* CFG3 phase mask */
#define WaveGenerator_DacClk_PHASE_MASK         WaveGenerator_DacClk__CFG3_PHASE_DLY_MASK
#endif /* defined(WaveGenerator_DacClk__CFG3) */

#endif /* CY_CLOCK_WaveGenerator_DacClk_H */


/* [] END OF FILE */
