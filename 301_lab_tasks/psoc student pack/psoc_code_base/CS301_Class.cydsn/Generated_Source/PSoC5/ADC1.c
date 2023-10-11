/*******************************************************************************
* File Name: ADC1.c
* Version 2.10
*
* Description:
*  This file provides the API functionality of the ADC SAR Sequencer Component
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

#include "ADC1.h"
#if(ADC1_IRQ_REMOVE == 0u)
    #include "ADC1_IRQ.h"
#endif   /* End ADC1_IRQ_REMOVE */

int16  ADC1_finalArray[ADC1_NUMBER_OF_CHANNELS];
uint32 ADC1_initVar = 0u;
static uint8 ADC1_tempChan;
static uint8 ADC1_finalChan;
static uint8 ADC1_tempTD = CY_DMA_INVALID_TD;
static uint8 ADC1_finalTD = CY_DMA_INVALID_TD;


/****************************************************************************
* Function Name: ADC1_Disable()
*****************************************************************************
*
* Summary:
*  Disables the component without disabling the ADC SAR.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
****************************************************************************/
void ADC1_Disable(void)
{
    ADC1_CONTROL_REG &= ((uint8)(~ADC1_BASE_COMPONENT_ENABLE));

    (void) CyDmaChDisable(ADC1_tempChan);
    CyDmaTdFree(ADC1_tempTD);
    ADC1_tempTD = CY_DMA_INVALID_TD;

    (void) CyDmaChDisable(ADC1_finalChan);
    CyDmaTdFree(ADC1_finalTD);
    ADC1_finalTD = CY_DMA_INVALID_TD;
}


/*******************************************************************************
* Function Name: ADC1_Init
********************************************************************************
*
* Summary:
*  Inits channels for DMA transfer. Provides loading period to the AMUX address
*  selection counter
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC1_Init(void)
{
    /* Init DMA, 2 bytes bursts, each burst requires a request */
    ADC1_tempChan = ADC1_TempBuf_DmaInitialize(ADC1_TEMP_BYTES_PER_BURST,
        ADC1_REQUEST_PER_BURST, (uint16)(HI16(CYDEV_PERIPH_BASE)), (uint16)(HI16(CYDEV_SRAM_BASE)));

    /* Init DMA, (ADC1_NUMBER_OF_CHANNELS << 1u) bytes bursts, each burst requires a request */
    ADC1_finalChan = ADC1_FinalBuf_DmaInitialize((uint8)ADC1_FINAL_BYTES_PER_BURST,
        ADC1_REQUEST_PER_BURST, (uint16)(HI16(CYDEV_SRAM_BASE)), (uint16)(HI16(CYDEV_SRAM_BASE)));

    #if(ADC1_IRQ_REMOVE == 0u)
        /* Set the ISR to point to the ADC1_IRQ Interrupt. */
        ADC1_IRQ_SetVector(&ADC1_ISR);
        /* Set the priority. */
        ADC1_IRQ_SetPriority((uint8)ADC1_INTC_NUMBER);
    #endif   /* End ADC1_IRQ_REMOVE */

}


/*******************************************************************************
* Function Name: ADC1_Enable
********************************************************************************
*
* Summary:
*  Enables DMA channels, address selection counter and FSM of Base component
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC1_Enable(void)
{
    uint8 enableInterrupts;

    static int16 ADC1_tempArray[ADC1_NUMBER_OF_CHANNELS];
    
    (void)CyDmaClearPendingDrq(ADC1_tempChan);
    (void)CyDmaClearPendingDrq(ADC1_finalChan);
    
    
    /* Provides initialization procedure for the TempBuf DMA
    * Configure this Td as follows:
    *  - The TD is looping on itself
    *  - Increment the destination address, but not the source address
    */

    if (ADC1_tempTD == DMA_INVALID_TD)
    {
        ADC1_tempTD = CyDmaTdAllocate();
    }

    (void) CyDmaTdSetConfiguration(ADC1_tempTD, ADC1_TEMP_TRANSFER_COUNT,
        ADC1_tempTD, ((uint8)ADC1_TempBuf__TD_TERMOUT_EN | (uint8)TD_INC_DST_ADR));

    /* From the SAR to the TempArray */
    (void) CyDmaTdSetAddress(ADC1_tempTD, (uint16)(LO16((uint32)ADC1_SAR_DATA_ADDR_0)),
        (uint16)(LO16((uint32)ADC1_tempArray)));

    /* Associate the TD with the channel */
    (void) CyDmaChSetInitialTd(ADC1_tempChan, ADC1_tempTD);


    /* Provides initialization procedure for the FinalBuf DMA
    * Configure this Td as follows:
    *  - The TD is looping on itself
    *  - Increment the source and destination address
    */

    if (ADC1_finalTD == DMA_INVALID_TD)
    {
        ADC1_finalTD = CyDmaTdAllocate();
    }
    
    (void) CyDmaTdSetConfiguration(ADC1_finalTD, (ADC1_FINAL_BYTES_PER_BURST),
        ADC1_finalTD, ((uint8)(ADC1_FinalBuf__TD_TERMOUT_EN) | (uint8)TD_INC_SRC_ADR |
            (uint8)TD_INC_DST_ADR));

    /* From the the TempArray to Final Array */
    (void) CyDmaTdSetAddress(ADC1_finalTD, (uint16)(LO16((uint32)ADC1_tempArray)),
        (uint16)(LO16((uint32)ADC1_finalArray)));

    /* Associate the TD with the channel */
    (void) CyDmaChSetInitialTd(ADC1_finalChan, ADC1_finalTD);
    
    (void) CyDmaChEnable(ADC1_tempChan, 1u);
    (void) CyDmaChEnable(ADC1_finalChan, 1u);

    /* Enable Counter and give Enable pulse to set an address of the last channel */
    enableInterrupts = CyEnterCriticalSection();
    ADC1_CYCLE_COUNTER_AUX_CONTROL_REG |= ((uint8)(ADC1_CYCLE_COUNTER_ENABLE));
    CyExitCriticalSection(enableInterrupts);

    /* Enable FSM of the Base Component */
    ADC1_CONTROL_REG |= ((uint8)(ADC1_BASE_COMPONENT_ENABLE));
    ADC1_CONTROL_REG |= ((uint8)(ADC1_LOAD_COUNTER_PERIOD));

    #if(ADC1_IRQ_REMOVE == 0u)
        /* Clear a pending interrupt */
        CyIntClearPending(ADC1_INTC_NUMBER);
    #endif   /* End ADC1_IRQ_REMOVE */
}


/*******************************************************************************
* Function Name: ADC1_Start
********************************************************************************
*
* Summary:
*  Starts component
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  If the initVar variable is already set, this function only calls the 
*  ADC1_Enable() function
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC1_Start(void)
{
    if(ADC1_initVar == 0u)
    {
        ADC1_Init();
        ADC1_initVar = 1u;
    }

    ADC1_SAR_Start();
    ADC1_Enable();
    (void) CY_GET_REG8(ADC1_STATUS_PTR);
}


/*******************************************************************************
* Function Name: ADC1_Stop
********************************************************************************
*
* Summary:
*  Stops component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC1_Stop(void)
{
    ADC1_SAR_Stop();
    ADC1_Disable();
}

#if(ADC1_SAMPLE_MODE != ADC1_SAMPLE_MODE_HW_TRIGGERED)

    /*******************************************************************************
    * Function Name: ADC1_StartConvert
    ********************************************************************************
    *
    * Summary:
    *  When the Sample Mode parameter is set to 'Free Running', the component will
    * operate in a continuous mode. The channels will be scanned continuously until
    * _StopConvert()or  _Stop() is called
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Calling ADC1_StartConvert() disables the external SOC pin.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void ADC1_StartConvert(void)
    {
        #if(ADC1_SAMPLE_MODE != ADC1_SAMPLE_MODE_FREE_RUNNING)

            ADC1_CONTROL_REG |= ((uint8)(ADC1_SOFTWARE_SOC_PULSE));

        #else

            ADC1_SAR_StartConvert();

        #endif /*
                 #if(ADC1_SAMPLE_MODE !=
                 ADC1_SAMPLE_MODE_FREE_RUNNING)
               */
    }


    /*******************************************************************************
    * Function Name: ADC1_StopConvert
    ********************************************************************************
    *
    * Summary:
    *  Forces the component to stop all conversions
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  In free-running and software trigger mode, this function sets a software 
    *  version of the SOC to low level and switches the SOC source to hardware SOC 
    *  input (Hardware trigger).
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void ADC1_StopConvert(void)
    {
        ADC1_SAR_StopConvert();
    }

#endif /* ADC1_SAMPLE_MODE != ADC1_SAMPLE_MODE_HW_TRIGGERED */


/*******************************************************************************
* Function Name: ADC1_IsEndConversion
********************************************************************************
*
* Summary:
*  Checks for ADC end of conversion for the case one channel and end of scan
*  for the case of multiple channels
*
* Parameters:
*  retMode: Check conversion return mode
*   Values:
*         - ADC1_RETURN_STATUS      - Immediately returns the 
*                                                 status
*         - ADC1_WAIT_FOR_RESULT    - Does not return a result 
*                                                 until the conversion 
*                                                 is complete
*
* Return:
*  If a nonzero value is returned, the last conversion is complete. If the 
*  returned value is zero, the ADC_SAR_Seq is still calculating the last result
*
* Side Effects:
*  This function reads the end of conversion status, which is cleared on read
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint32 ADC1_IsEndConversion(uint8 retMode)
{
    uint8 status;

    do
    {
      status = ADC1_STATUS_REG;
    } while ((status == 0u) && (retMode == ADC1_WAIT_FOR_RESULT));

    return((uint32)status);
}


/*******************************************************************************
* Function Name: ADC1_GetResult16
********************************************************************************
*
* Summary:
*  Returns the ADC result for channel chan
*
* Parameters:
*  chan: The ADC channel in which to return the result. The first channel is 0 
*        and the last channel is the total number of channels - 1
*
* Return:
*  Returns converted data as a signed 16-bit integer
*
* Side Effects:
*  Converts the ADC counts to the 2's complement form
*
* Reentrant:
*  No.
*
*******************************************************************************/
int16 ADC1_GetResult16(uint16 chan)
{
    return (ADC1_finalArray[ADC1_GET_RESULT_INDEX_OFFSET - chan] - ADC1_SAR_shift);
}


/*******************************************************************************
* Function Name: ADC1_GetAdcResult
********************************************************************************
*
* Summary:
*  Gets the data available in the SAR DATA register, not the results buffer
*
* Parameters:
*  None.
*
* Return:
*  The last ADC conversion result
*
* Side Effects:
*  Converts the ADC counts to the 2's complement form
*
* Reentrant:
*  No.
*
*******************************************************************************/
int16 ADC1_GetAdcResult(void)
{
    return (ADC1_SAR_GetResult16());
}


/*******************************************************************************
* Function Name: ADC1_SetOffset
********************************************************************************
*
* Summary:
*  Sets the ADC offset which is used by the functions _CountsTo_uVolts(),
*  _CountsTo_mVolts() and _CountsTo_Volts() to substract the offset from the
*  given reading before calculating the voltage conversion
*
* Parameters:
*  offset: This value is measured when the inputs are shorted or connected to
*  the same input voltage
*
* Return:
*  None.
*
* Side Effects:
*  Affects ADC1_CountsTo_Volts(), 
*  ADC1_CountsTo_mVolts(), and ADC1_CountsTo_uVolts() 
*  by subtracting the given offset.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC1_SetOffset(int32 offset)
{
    ADC1_SAR_SetOffset((int16)offset);
}


/*******************************************************************************
* Function Name: ADC1_SetResolution
********************************************************************************
*
* Summary:
*  Sets the Relution of the SAR.
*  This function does not affect the actual conversion with PSoC5 ES1 silicon.
*
* Parameters:
*  resolution:
*  12 ->    RES12
*  10 ->    RES10
*  8  ->    RES8
*
* Return:
*  None.
*
* Side Effects:
*  The ADC_SAR_Seq resolution cannot be changed during a conversion cycle. The
*  recommended best practice is to stop conversions with
*  ADC1_StopConvert(), change the resolution, then restart the
*  conversions with ADC1_StartConvert().
*  If you decide not to stop conversions before calling this API, you
*  should use ADC1_IsEndConversion() to wait until conversion is 
*  complete  before changing the resolution.
*  If you call ADC_SetResolution() during a conversion, the resolution will
*  not be changed until the current conversion is complete. Data will not be
*  available in the new resolution for another 6 + "New Resolution(in bits)"
*  clock cycles.
*  You may need add a delay of this number of clock cycles after
*  ADC1_SetResolution() is called before data is valid again.
*  Affects ADC1_CountsTo_Volts(), ADC1_CountsTo_mVolts(), 
*  and ADC1_CountsTo_uVolts() by calculating the correct conversion 
*  between ADC counts and the applied input voltage. Calculation depends on 
*  resolution, input range, and voltage reference.
*
*******************************************************************************/
void ADC1_SetResolution(uint8 resolution)
{
    ADC1_SAR_SetResolution(resolution);
}


/*******************************************************************************
* Function Name: ADC1_SetGain
********************************************************************************
*
* Summary:
*  Sets the ADC gain in counts per volt for the voltage conversion
*
* Parameters:
*  adcGain: counts per volt
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC1_SetGain(int32 adcGain)
{
    ADC1_SAR_SetGain((int16)adcGain);
}


/*******************************************************************************
* Function Name: ADC1_SetScaledGain
********************************************************************************
*
* Summary:
*  Sets the ADC gain in counts per 10 volts for the voltage conversion functions
*  that follow. This value is set by default by the reference and input range
*  settings. It should only be used to further calibrate the ADC with a known
*  input or if the ADC is using an external reference.
*
* Parameters:
*  int32  adcGain  counts per 10 volt
*
* Return:
*  None.
*
* Side Effects:
*  Affects ADC1_CountsTo_Volts(), ADC1_CountsTo_mVolts(),
*  ADC1_CountsTo_uVolts() by supplying the correct conversion 
*  between ADC counts and the applied input voltage
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC1_SetScaledGain(int32 adcGain)
{
    ADC1_SAR_SetScaledGain(adcGain);
}


/*******************************************************************************
* Function Name: ADC1_CountsTo_mVolts
********************************************************************************
*
* Summary:
*  Converts the ADC output to mVolts as a 32-bit integer
*
* Parameters:
*  adcCounts: Result from the ADC_SAR_Seq conversion
*
* Return:
*  Result in mV
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
int32 ADC1_CountsTo_mVolts(int16 adcCounts)
{
    return ((int32) ADC1_SAR_CountsTo_mVolts(adcCounts));
}


/*******************************************************************************
* Function Name: ADC1_CountsTo_uVolts
********************************************************************************
*
* Summary:
*  Converts the ADC output to uVolts as a 32-bit integer
*
* Parameters:
*  adcCounts: Result from the ADC conversion
*
* Return:
*  Result in uV
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
int32 ADC1_CountsTo_uVolts(int16 adcCounts)
{
    return (ADC1_SAR_CountsTo_uVolts(adcCounts));
}


/*******************************************************************************
* Function Name: ADC1_CountsTo_Volts
********************************************************************************
*
* Summary:
*  Converts the ADC output to Volts as a floating point number
*
* Parameters:
*  adcCounts: Result from the ADC_SAR_Seq conversion
*
* Return:
*  Result in volts
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
float32 ADC1_CountsTo_Volts(int16 adcCounts)
{
    return (ADC1_SAR_CountsTo_Volts(adcCounts));
}


/* [] END OF FILE */
