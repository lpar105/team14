/*******************************************************************************
* File Name: eoc.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_eoc_H)
#define CY_ISR_eoc_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void eoc_Start(void);
void eoc_StartEx(cyisraddress address);
void eoc_Stop(void);

CY_ISR_PROTO(eoc_Interrupt);

void eoc_SetVector(cyisraddress address);
cyisraddress eoc_GetVector(void);

void eoc_SetPriority(uint8 priority);
uint8 eoc_GetPriority(void);

void eoc_Enable(void);
uint8 eoc_GetState(void);
void eoc_Disable(void);

void eoc_SetPending(void);
void eoc_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the eoc ISR. */
#define eoc_INTC_VECTOR            ((reg32 *) eoc__INTC_VECT)

/* Address of the eoc ISR priority. */
#define eoc_INTC_PRIOR             ((reg8 *) eoc__INTC_PRIOR_REG)

/* Priority of the eoc interrupt. */
#define eoc_INTC_PRIOR_NUMBER      eoc__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable eoc interrupt. */
#define eoc_INTC_SET_EN            ((reg32 *) eoc__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the eoc interrupt. */
#define eoc_INTC_CLR_EN            ((reg32 *) eoc__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the eoc interrupt state to pending. */
#define eoc_INTC_SET_PD            ((reg32 *) eoc__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the eoc interrupt. */
#define eoc_INTC_CLR_PD            ((reg32 *) eoc__INTC_CLR_PD_REG)


#endif /* CY_ISR_eoc_H */


/* [] END OF FILE */
