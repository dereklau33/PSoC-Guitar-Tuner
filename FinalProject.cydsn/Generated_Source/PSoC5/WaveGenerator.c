/*******************************************************************************
* File Name: WaveGenerator.c
* Version 2.10
*
* Description:
*  This file provides the source code for the 8-bit Waveform DAC 
*  (WaveDAC8) Component.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "WaveGenerator.h"

uint8  WaveGenerator_initVar = 0u;

const uint8 CYCODE WaveGenerator_wave1[WaveGenerator_WAVE1_LENGTH] = { 128u,135u,143u,151u,159u,166u,174u,181u,188u,194u,201u,207u,213u,219u,224u,229u,233u,237u,241u,244u,246u,249u,250u,252u,252u,252u,252u,252u,250u,249u,246u,244u,241u,237u,233u,229u,224u,219u,213u,207u,201u,194u,188u,181u,174u,166u,159u,151u,143u,135u,128u,120u,112u,104u,96u,89u,81u,74u,67u,61u,54u,48u,42u,36u,31u,26u,22u,18u,14u,11u,9u,6u,5u,3u,3u,3u,3u,3u,5u,6u,9u,11u,14u,18u,22u,26u,31u,36u,42u,48u,54u,61u,67u,74u,81u,89u,96u,104u,112u,120u };
const uint8 CYCODE WaveGenerator_wave2[WaveGenerator_WAVE2_LENGTH] = { 128u,135u,143u,151u,159u,166u,174u,181u,188u,194u,201u,207u,213u,219u,224u,229u,233u,237u,241u,244u,246u,249u,250u,252u,252u,252u,252u,252u,250u,249u,246u,244u,241u,237u,233u,229u,224u,219u,213u,207u,201u,194u,188u,181u,174u,166u,159u,151u,143u,135u,128u,120u,112u,104u,96u,89u,81u,74u,67u,61u,54u,48u,42u,36u,31u,26u,22u,18u,14u,11u,9u,6u,5u,3u,3u,3u,3u,3u,5u,6u,9u,11u,14u,18u,22u,26u,31u,36u,42u,48u,54u,61u,67u,74u,81u,89u,96u,104u,112u,120u };

static uint8  WaveGenerator_Wave1Chan;
static uint8  WaveGenerator_Wave2Chan;
static uint8  WaveGenerator_Wave1TD;
static uint8  WaveGenerator_Wave2TD;


/*******************************************************************************
* Function Name: WaveGenerator_Init
********************************************************************************
*
* Summary:
*  Initializes component with parameters set in the customizer.
*
* Parameters:  
*  None
*
* Return: 
*  None
*
*******************************************************************************/
void WaveGenerator_Init(void) 
{
	WaveGenerator_VDAC8_Init();
	WaveGenerator_VDAC8_SetSpeed(WaveGenerator_HIGHSPEED);
	WaveGenerator_VDAC8_SetRange(WaveGenerator_DAC_RANGE);

	#if(WaveGenerator_DAC_MODE == WaveGenerator_CURRENT_MODE)
		WaveGenerator_IDAC8_SetPolarity(WaveGenerator_DAC_POL);
	#endif /* WaveGenerator_DAC_MODE == WaveGenerator_CURRENT_MODE */

	#if(WaveGenerator_OUT_MODE == WaveGenerator_BUFFER_MODE)
	   WaveGenerator_BuffAmp_Init();
	#endif /* WaveGenerator_OUT_MODE == WaveGenerator_BUFFER_MODE */

	/* Get the TD Number for the DMA channel 1 and 2   */
	WaveGenerator_Wave1TD = CyDmaTdAllocate();
	WaveGenerator_Wave2TD = CyDmaTdAllocate();
	
	/* Initialize waveform pointers  */
	WaveGenerator_Wave1Setup(WaveGenerator_wave1, WaveGenerator_WAVE1_LENGTH) ;
	WaveGenerator_Wave2Setup(WaveGenerator_wave2, WaveGenerator_WAVE2_LENGTH) ;
	
	/* Initialize the internal clock if one present  */
	#if defined(WaveGenerator_DacClk_PHASE)
	   WaveGenerator_DacClk_SetPhase(WaveGenerator_CLK_PHASE_0nS);
	#endif /* defined(WaveGenerator_DacClk_PHASE) */
}


/*******************************************************************************
* Function Name: WaveGenerator_Enable
********************************************************************************
*  
* Summary: 
*  Enables the DAC block and DMA operation.
*
* Parameters:  
*  None
*
* Return: 
*  None
*
*******************************************************************************/
void WaveGenerator_Enable(void) 
{
	WaveGenerator_VDAC8_Enable();

	#if(WaveGenerator_OUT_MODE == WaveGenerator_BUFFER_MODE)
	   WaveGenerator_BuffAmp_Enable();
	#endif /* WaveGenerator_OUT_MODE == WaveGenerator_BUFFER_MODE */

	/* 
	* Enable the channel. It is configured to remember the TD value so that
	* it can be restored from the place where it has been stopped.
	*/
	(void)CyDmaChEnable(WaveGenerator_Wave1Chan, 1u);
	(void)CyDmaChEnable(WaveGenerator_Wave2Chan, 1u);
	
	/* set the initial value */
	WaveGenerator_SetValue(0u);
	
	#if(WaveGenerator_CLOCK_SRC == WaveGenerator_CLOCK_INT)  	
	   WaveGenerator_DacClk_Start();
	#endif /* WaveGenerator_CLOCK_SRC == WaveGenerator_CLOCK_INT */
}


/*******************************************************************************
* Function Name: WaveGenerator_Start
********************************************************************************
*
* Summary:
*  The start function initializes the voltage DAC with the default values, 
*  and sets the power to the given level.  A power level of 0, is the same as 
*  executing the stop function.
*
* Parameters:  
*  None
*
* Return: 
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void WaveGenerator_Start(void) 
{
	/* If not Initialized then initialize all required hardware and software */
	if(WaveGenerator_initVar == 0u)
	{
		WaveGenerator_Init();
		WaveGenerator_initVar = 1u;
	}
	
	WaveGenerator_Enable();
}


/*******************************************************************************
* Function Name: WaveGenerator_StartEx
********************************************************************************
*
* Summary:
*  The StartEx function sets pointers and sizes for both waveforms
*  and then starts the component.
*
* Parameters:  
*   uint8 * wavePtr1:     Pointer to the waveform 1 array.
*   uint16  sampleSize1:  The amount of samples in the waveform 1.
*   uint8 * wavePtr2:     Pointer to the waveform 2 array.
*   uint16  sampleSize2:  The amount of samples in the waveform 2.
*
* Return: 
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void WaveGenerator_StartEx(const uint8 * wavePtr1, uint16 sampleSize1, const uint8 * wavePtr2, uint16 sampleSize2)

{
	WaveGenerator_Wave1Setup(wavePtr1, sampleSize1);
	WaveGenerator_Wave2Setup(wavePtr2, sampleSize2);
	WaveGenerator_Start();
}


/*******************************************************************************
* Function Name: WaveGenerator_Stop
********************************************************************************
*
* Summary:
*  Stops the clock (if internal), disables the DMA channels
*  and powers down the DAC.
*
* Parameters:  
*  None  
*
* Return: 
*  None
*
*******************************************************************************/
void WaveGenerator_Stop(void) 
{
	/* Turn off internal clock, if one present */
	#if(WaveGenerator_CLOCK_SRC == WaveGenerator_CLOCK_INT)  	
	   WaveGenerator_DacClk_Stop();
	#endif /* WaveGenerator_CLOCK_SRC == WaveGenerator_CLOCK_INT */
	
	/* Disble DMA channels */
	(void)CyDmaChDisable(WaveGenerator_Wave1Chan);
	(void)CyDmaChDisable(WaveGenerator_Wave2Chan);

	/* Disable power to DAC */
	WaveGenerator_VDAC8_Stop();
}


/*******************************************************************************
* Function Name: WaveGenerator_Wave1Setup
********************************************************************************
*
* Summary:
*  Sets pointer and size for waveform 1.                                    
*
* Parameters:  
*  uint8 * WavePtr:     Pointer to the waveform array.
*  uint16  SampleSize:  The amount of samples in the waveform.
*
* Return: 
*  None 
*
*******************************************************************************/
void WaveGenerator_Wave1Setup(const uint8 * wavePtr, uint16 sampleSize)

{
	#if (CY_PSOC3)
		uint16 memoryType; /* determining the source memory type */
		memoryType = (WaveGenerator_HI16FLASHPTR == HI16(wavePtr)) ? HI16(CYDEV_FLS_BASE) : HI16(CYDEV_SRAM_BASE);
		
		WaveGenerator_Wave1Chan = WaveGenerator_Wave1_DMA_DmaInitialize(
		WaveGenerator_Wave1_DMA_BYTES_PER_BURST, WaveGenerator_Wave1_DMA_REQUEST_PER_BURST,
		memoryType, HI16(CYDEV_PERIPH_BASE));
	#else /* PSoC 5 */
		WaveGenerator_Wave1Chan = WaveGenerator_Wave1_DMA_DmaInitialize(
		WaveGenerator_Wave1_DMA_BYTES_PER_BURST, WaveGenerator_Wave1_DMA_REQUEST_PER_BURST,
		HI16(wavePtr), HI16(WaveGenerator_DAC8__D));
	#endif /* CY_PSOC3 */
	
	/*
	* TD is looping on itself. 
    * Increment the source address, but not the destination address. 
	*/
	(void)CyDmaTdSetConfiguration(WaveGenerator_Wave1TD, sampleSize, WaveGenerator_Wave1TD, 
                                    (uint8)CY_DMA_TD_INC_SRC_ADR | (uint8)WaveGenerator_Wave1_DMA__TD_TERMOUT_EN); 
	
	/* Set the TD source and destination address */
	(void)CyDmaTdSetAddress(WaveGenerator_Wave1TD, LO16((uint32)wavePtr), LO16(WaveGenerator_DAC8__D));
	
	/* Associate the TD with the channel */
	(void)CyDmaChSetInitialTd(WaveGenerator_Wave1Chan, WaveGenerator_Wave1TD);
}


/*******************************************************************************
* Function Name: WaveGenerator_Wave2Setup
********************************************************************************
*
* Summary:
*  Sets pointer and size for waveform 2.                                    
*
* Parameters:  
*  uint8 * WavePtr:     Pointer to the waveform array.
*  uint16  SampleSize:  The amount of samples in the waveform.
*
* Return: 
*  None
*
*******************************************************************************/
void WaveGenerator_Wave2Setup(const uint8 * wavePtr, uint16 sampleSize)
 
{
	#if (CY_PSOC3)
		uint16 memoryType; /* determining the source memory type */
		memoryType = (WaveGenerator_HI16FLASHPTR == HI16(wavePtr)) ? HI16(CYDEV_FLS_BASE) : HI16(CYDEV_SRAM_BASE);
			
		WaveGenerator_Wave2Chan = WaveGenerator_Wave2_DMA_DmaInitialize(
		WaveGenerator_Wave2_DMA_BYTES_PER_BURST, WaveGenerator_Wave2_DMA_REQUEST_PER_BURST,
		memoryType, HI16(CYDEV_PERIPH_BASE));
	#else /* PSoC 5 */
		WaveGenerator_Wave2Chan = WaveGenerator_Wave2_DMA_DmaInitialize(
		WaveGenerator_Wave2_DMA_BYTES_PER_BURST, WaveGenerator_Wave2_DMA_REQUEST_PER_BURST,
		HI16(wavePtr), HI16(WaveGenerator_DAC8__D));
	#endif /* CY_PSOC3 */
	
	/*
	* TD is looping on itself. 
	* Increment the source address, but not the destination address. 
	*/
	(void)CyDmaTdSetConfiguration(WaveGenerator_Wave2TD, sampleSize, WaveGenerator_Wave2TD, 
                                    (uint8)CY_DMA_TD_INC_SRC_ADR | (uint8)WaveGenerator_Wave2_DMA__TD_TERMOUT_EN); 
	
	/* Set the TD source and destination address */
	(void)CyDmaTdSetAddress(WaveGenerator_Wave2TD, LO16((uint32)wavePtr), LO16(WaveGenerator_DAC8__D));
	
	/* Associate the TD with the channel */
	(void)CyDmaChSetInitialTd(WaveGenerator_Wave2Chan, WaveGenerator_Wave2TD);
}


/* [] END OF FILE */
