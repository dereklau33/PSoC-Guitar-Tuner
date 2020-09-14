/*******************************************************************************
* File Name: WaveGenerator.h  
* Version 2.10
*
* Description:
*  This file contains the function prototypes and constants used in
*  the 8-bit Waveform DAC (WaveDAC8) Component.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_WaveDAC8_WaveGenerator_H) 
#define CY_WaveDAC8_WaveGenerator_H

#include "cytypes.h"
#include "cyfitter.h"
#include <WaveGenerator_Wave1_DMA_dma.h>
#include <WaveGenerator_Wave2_DMA_dma.h>
#include <WaveGenerator_VDAC8.h>


/***************************************
*  Initial Parameter Constants
***************************************/

#define WaveGenerator_WAVE1_TYPE     (0u)     /* Waveform for wave1 */
#define WaveGenerator_WAVE2_TYPE     (0u)     /* Waveform for wave2 */
#define WaveGenerator_SINE_WAVE      (0u)
#define WaveGenerator_SQUARE_WAVE    (1u)
#define WaveGenerator_TRIANGLE_WAVE  (2u)
#define WaveGenerator_SAWTOOTH_WAVE  (3u)
#define WaveGenerator_ARB_DRAW_WAVE  (10u) /* Arbitrary (draw) */
#define WaveGenerator_ARB_FILE_WAVE  (11u) /* Arbitrary (from file) */

#define WaveGenerator_WAVE1_LENGTH   (100u)   /* Length for wave1 */
#define WaveGenerator_WAVE2_LENGTH   (100u)   /* Length for wave2 */
	
#define WaveGenerator_DEFAULT_RANGE    (0u) /* Default DAC range */
#define WaveGenerator_DAC_RANGE_1V     (0u)
#define WaveGenerator_DAC_RANGE_1V_BUF (16u)
#define WaveGenerator_DAC_RANGE_4V     (1u)
#define WaveGenerator_DAC_RANGE_4V_BUF (17u)
#define WaveGenerator_VOLT_MODE        (0u)
#define WaveGenerator_CURRENT_MODE     (1u)
#define WaveGenerator_DAC_MODE         (((WaveGenerator_DEFAULT_RANGE == WaveGenerator_DAC_RANGE_1V) || \
									  (WaveGenerator_DEFAULT_RANGE == WaveGenerator_DAC_RANGE_4V) || \
							  		  (WaveGenerator_DEFAULT_RANGE == WaveGenerator_DAC_RANGE_1V_BUF) || \
									  (WaveGenerator_DEFAULT_RANGE == WaveGenerator_DAC_RANGE_4V_BUF)) ? \
									   WaveGenerator_VOLT_MODE : WaveGenerator_CURRENT_MODE)

#define WaveGenerator_DACMODE WaveGenerator_DAC_MODE /* legacy definition for backward compatibility */

#define WaveGenerator_DIRECT_MODE (0u)
#define WaveGenerator_BUFFER_MODE (1u)
#define WaveGenerator_OUT_MODE    (((WaveGenerator_DEFAULT_RANGE == WaveGenerator_DAC_RANGE_1V_BUF) || \
								 (WaveGenerator_DEFAULT_RANGE == WaveGenerator_DAC_RANGE_4V_BUF)) ? \
								  WaveGenerator_BUFFER_MODE : WaveGenerator_DIRECT_MODE)

#if(WaveGenerator_OUT_MODE == WaveGenerator_BUFFER_MODE)
    #include <WaveGenerator_BuffAmp.h>
#endif /* WaveGenerator_OUT_MODE == WaveGenerator_BUFFER_MODE */

#define WaveGenerator_CLOCK_INT      (1u)
#define WaveGenerator_CLOCK_EXT      (0u)
#define WaveGenerator_CLOCK_SRC      (1u)

#if(WaveGenerator_CLOCK_SRC == WaveGenerator_CLOCK_INT)  
	#include <WaveGenerator_DacClk.h>
	#if defined(WaveGenerator_DacClk_PHASE)
		#define WaveGenerator_CLK_PHASE_0nS (1u)
	#endif /* defined(WaveGenerator_DacClk_PHASE) */
#endif /* WaveGenerator_CLOCK_SRC == WaveGenerator_CLOCK_INT */

#if (CY_PSOC3)
	#define WaveGenerator_HI16FLASHPTR   (0xFFu)
#endif /* CY_PSOC3 */

#define WaveGenerator_Wave1_DMA_BYTES_PER_BURST      (1u)
#define WaveGenerator_Wave1_DMA_REQUEST_PER_BURST    (1u)
#define WaveGenerator_Wave2_DMA_BYTES_PER_BURST      (1u)
#define WaveGenerator_Wave2_DMA_REQUEST_PER_BURST    (1u)


/***************************************
*   Data Struct Definition
***************************************/

/* Low power Mode API Support */
typedef struct
{
	uint8   enableState;
}WaveGenerator_BACKUP_STRUCT;


/***************************************
*        Function Prototypes 
***************************************/

void WaveGenerator_Start(void)             ;
void WaveGenerator_StartEx(const uint8 * wavePtr1, uint16 sampleSize1, const uint8 * wavePtr2, uint16 sampleSize2)
                                        ;
void WaveGenerator_Init(void)              ;
void WaveGenerator_Enable(void)            ;
void WaveGenerator_Stop(void)              ;

void WaveGenerator_Wave1Setup(const uint8 * wavePtr, uint16 sampleSize)
                                        ;
void WaveGenerator_Wave2Setup(const uint8 * wavePtr, uint16 sampleSize)
                                        ;

void WaveGenerator_Sleep(void)             ;
void WaveGenerator_Wakeup(void)            ;

#define WaveGenerator_SetSpeed       WaveGenerator_VDAC8_SetSpeed
#define WaveGenerator_SetRange       WaveGenerator_VDAC8_SetRange
#define WaveGenerator_SetValue       WaveGenerator_VDAC8_SetValue
#define WaveGenerator_DacTrim        WaveGenerator_VDAC8_DacTrim
#define WaveGenerator_SaveConfig     WaveGenerator_VDAC8_SaveConfig
#define WaveGenerator_RestoreConfig  WaveGenerator_VDAC8_RestoreConfig


/***************************************
*    Variable with external linkage 
***************************************/

extern uint8 WaveGenerator_initVar;

extern const uint8 CYCODE WaveGenerator_wave1[WaveGenerator_WAVE1_LENGTH];
extern const uint8 CYCODE WaveGenerator_wave2[WaveGenerator_WAVE2_LENGTH];


/***************************************
*            API Constants
***************************************/

/* SetRange constants */
#if(WaveGenerator_DAC_MODE == WaveGenerator_VOLT_MODE)
    #define WaveGenerator_RANGE_1V       (0x00u)
    #define WaveGenerator_RANGE_4V       (0x04u)
#else /* current mode */
    #define WaveGenerator_RANGE_32uA     (0x00u)
    #define WaveGenerator_RANGE_255uA    (0x04u)
    #define WaveGenerator_RANGE_2mA      (0x08u)
    #define WaveGenerator_RANGE_2048uA   WaveGenerator_RANGE_2mA
#endif /* WaveGenerator_DAC_MODE == WaveGenerator_VOLT_MODE */

/* Power setting for SetSpeed API */
#define WaveGenerator_LOWSPEED       (0x00u)
#define WaveGenerator_HIGHSPEED      (0x02u)


/***************************************
*              Registers        
***************************************/

#define WaveGenerator_DAC8__D WaveGenerator_VDAC8_viDAC8__D


/***************************************
*         Register Constants       
***************************************/

/* CR0 vDac Control Register 0 definitions */

/* Bit Field  DAC_HS_MODE */
#define WaveGenerator_HS_MASK        (0x02u)
#define WaveGenerator_HS_LOWPOWER    (0x00u)
#define WaveGenerator_HS_HIGHSPEED   (0x02u)

/* Bit Field  DAC_MODE */
#define WaveGenerator_MODE_MASK      (0x10u)
#define WaveGenerator_MODE_V         (0x00u)
#define WaveGenerator_MODE_I         (0x10u)

/* Bit Field  DAC_RANGE */
#define WaveGenerator_RANGE_MASK     (0x0Cu)
#define WaveGenerator_RANGE_0        (0x00u)
#define WaveGenerator_RANGE_1        (0x04u)
#define WaveGenerator_RANGE_2        (0x08u)
#define WaveGenerator_RANGE_3        (0x0Cu)
#define WaveGenerator_IDIR_MASK      (0x04u)

#define WaveGenerator_DAC_RANGE      ((uint8)(0u << 2u) & WaveGenerator_RANGE_MASK)
#define WaveGenerator_DAC_POL        ((uint8)(0u >> 1u) & WaveGenerator_IDIR_MASK)


#endif /* CY_WaveDAC8_WaveGenerator_H  */

/* [] END OF FILE */
