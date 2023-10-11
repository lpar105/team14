/*******************************************************************************
* File Name: ADC1.h
* Version 2.10
*
* Description:
*  Contains the function prototypes, constants and register definition of the
*  ADC SAR Sequencer Component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_ADC1_H)
    #define CY_ADC1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"
#include "ADC1_TempBuf_dma.h"
#include "ADC1_FinalBuf_dma.h"
#include "ADC1_SAR.h"

#define ADC1_NUMBER_OF_CHANNELS    (6u)
#define ADC1_SAMPLE_MODE           (0u)
#define ADC1_CLOCK_SOURCE          (0u)

extern int16  ADC1_finalArray[ADC1_NUMBER_OF_CHANNELS];
extern uint32 ADC1_initVar;

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component ADC_SAR_SEQ_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */


/***************************************
*        Function Prototypes
***************************************/
void ADC1_Init(void);
void ADC1_Enable(void);
void ADC1_Disable(void);
void ADC1_Start(void);
void ADC1_Stop(void);

uint32 ADC1_IsEndConversion(uint8 retMode);
int16 ADC1_GetResult16(uint16 chan);
int16 ADC1_GetAdcResult(void);
void ADC1_SetOffset(int32 offset);
void ADC1_SetResolution(uint8 resolution);
void ADC1_SetScaledGain(int32 adcGain);
int32 ADC1_CountsTo_mVolts(int16 adcCounts);
int32 ADC1_CountsTo_uVolts(int16 adcCounts);
float32 ADC1_CountsTo_Volts(int16 adcCounts);
void ADC1_Sleep(void);
void ADC1_Wakeup(void);
void ADC1_SaveConfig(void);
void ADC1_RestoreConfig(void);

CY_ISR_PROTO( ADC1_ISR );

/* Obsolete API for backward compatibility.
*  Should not be used in new designs.
*/
void ADC1_SetGain(int32 adcGain);


/**************************************
*    Initial Parameter Constants
**************************************/
#define ADC1_IRQ_REMOVE             (0u)                /* Removes internal interrupt */


/***************************************
*             Registers
***************************************/
#define ADC1_CYCLE_COUNTER_AUX_CONTROL_REG \
                                               (*(reg8 *) ADC1_bSAR_SEQ_ChannelCounter__CONTROL_AUX_CTL_REG)
#define ADC1_CYCLE_COUNTER_AUX_CONTROL_PTR \
                                               ( (reg8 *) ADC1_bSAR_SEQ_ChannelCounter__CONTROL_AUX_CTL_REG)
#define ADC1_CONTROL_REG    (*(reg8 *) \
                                             ADC1_bSAR_SEQ_CtrlReg__CONTROL_REG)
#define ADC1_CONTROL_PTR    ( (reg8 *) \
                                             ADC1_bSAR_SEQ_CtrlReg__CONTROL_REG)
#define ADC1_COUNT_REG      (*(reg8 *) \
                                             ADC1_bSAR_SEQ_ChannelCounter__COUNT_REG)
#define ADC1_COUNT_PTR      ( (reg8 *) \
                                             ADC1_bSAR_SEQ_ChannelCounter__COUNT_REG)
#define ADC1_STATUS_REG     (*(reg8 *) ADC1_bSAR_SEQ_EOCSts__STATUS_REG)
#define ADC1_STATUS_PTR     ( (reg8 *) ADC1_bSAR_SEQ_EOCSts__STATUS_REG)

#define ADC1_SAR_DATA_ADDR_0 (ADC1_SAR_ADC_SAR__WRK0)
#define ADC1_SAR_DATA_ADDR_1 (ADC1_SAR_ADC_SAR__WRK1)
#define ADC1_SAR_DATA_ADDR_0_REG (*(reg8 *) \
                                              ADC1_SAR_ADC_SAR__WRK0)
#define ADC1_SAR_DATA_ADDR_1_REG (*(reg8 *) \
                                              ADC1_SAR_ADC_SAR__WRK1)


/**************************************
*       Register Constants
**************************************/

#if(ADC1_IRQ_REMOVE == 0u)

    /* Priority of the ADC_SAR_IRQ interrupt. */
    #define ADC1_INTC_PRIOR_NUMBER          (uint8)(ADC1_IRQ__INTC_PRIOR_NUM)

    /* ADC_SAR_IRQ interrupt number */
    #define ADC1_INTC_NUMBER                (uint8)(ADC1_IRQ__INTC_NUMBER)

#endif   /* End ADC1_IRQ_REMOVE */


/***************************************
*       API Constants
***************************************/

/* Constants for IsEndConversion() "retMode" parameter */
#define ADC1_RETURN_STATUS              (0x01u)
#define ADC1_WAIT_FOR_RESULT            (0x00u)

/* Defines for the Resolution parameter */
#define ADC1_BITS_12    ADC1_SAR__BITS_12
#define ADC1_BITS_10    ADC1_SAR__BITS_10
#define ADC1_BITS_8     ADC1_SAR__BITS_8

#define ADC1_CYCLE_COUNTER_ENABLE    (0x20u)
#define ADC1_BASE_COMPONENT_ENABLE   (0x01u)
#define ADC1_LOAD_COUNTER_PERIOD     (0x02u)
#define ADC1_SOFTWARE_SOC_PULSE      (0x04u)

/* Generic DMA Configuration parameters */
#define ADC1_TEMP_BYTES_PER_BURST     (uint8)(2u)
#define ADC1_TEMP_TRANSFER_COUNT      ((uint16)ADC1_NUMBER_OF_CHANNELS << 1u)
#define ADC1_FINAL_BYTES_PER_BURST    ((uint16)ADC1_NUMBER_OF_CHANNELS << 1u)
#define ADC1_REQUEST_PER_BURST        (uint8)(1u)

#define ADC1_GET_RESULT_INDEX_OFFSET    ((uint8)ADC1_NUMBER_OF_CHANNELS - 1u)

/* Define for Sample Mode  */
#define ADC1_SAMPLE_MODE_FREE_RUNNING    (0x00u)
#define ADC1_SAMPLE_MODE_SW_TRIGGERED    (0x01u)
#define ADC1_SAMPLE_MODE_HW_TRIGGERED    (0x02u)

/* Define for Clock Source  */
#define ADC1_CLOCK_INTERNAL              (0x00u)
#define ADC1_CLOCK_EXTERNAL              (0x01u)


/***************************************
*        Optional Function Prototypes
***************************************/
#if(ADC1_SAMPLE_MODE != ADC1_SAMPLE_MODE_HW_TRIGGERED)
    void ADC1_StartConvert(void);
    void ADC1_StopConvert(void);
#endif /* ADC1_SAMPLE_MODE != ADC1_SAMPLE_MODE_HW_TRIGGERED */

#endif  /* !defined(CY_ADC1_H) */

/* [] END OF FILE */
