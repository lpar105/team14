/*******************************************************************************
* File Name: turnCompleteSensor.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_turnCompleteSensor_H) /* Pins turnCompleteSensor_H */
#define CY_PINS_turnCompleteSensor_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "turnCompleteSensor_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 turnCompleteSensor__PORT == 15 && ((turnCompleteSensor__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    turnCompleteSensor_Write(uint8 value);
void    turnCompleteSensor_SetDriveMode(uint8 mode);
uint8   turnCompleteSensor_ReadDataReg(void);
uint8   turnCompleteSensor_Read(void);
void    turnCompleteSensor_SetInterruptMode(uint16 position, uint16 mode);
uint8   turnCompleteSensor_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the turnCompleteSensor_SetDriveMode() function.
     *  @{
     */
        #define turnCompleteSensor_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define turnCompleteSensor_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define turnCompleteSensor_DM_RES_UP          PIN_DM_RES_UP
        #define turnCompleteSensor_DM_RES_DWN         PIN_DM_RES_DWN
        #define turnCompleteSensor_DM_OD_LO           PIN_DM_OD_LO
        #define turnCompleteSensor_DM_OD_HI           PIN_DM_OD_HI
        #define turnCompleteSensor_DM_STRONG          PIN_DM_STRONG
        #define turnCompleteSensor_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define turnCompleteSensor_MASK               turnCompleteSensor__MASK
#define turnCompleteSensor_SHIFT              turnCompleteSensor__SHIFT
#define turnCompleteSensor_WIDTH              1u

/* Interrupt constants */
#if defined(turnCompleteSensor__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in turnCompleteSensor_SetInterruptMode() function.
     *  @{
     */
        #define turnCompleteSensor_INTR_NONE      (uint16)(0x0000u)
        #define turnCompleteSensor_INTR_RISING    (uint16)(0x0001u)
        #define turnCompleteSensor_INTR_FALLING   (uint16)(0x0002u)
        #define turnCompleteSensor_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define turnCompleteSensor_INTR_MASK      (0x01u) 
#endif /* (turnCompleteSensor__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define turnCompleteSensor_PS                     (* (reg8 *) turnCompleteSensor__PS)
/* Data Register */
#define turnCompleteSensor_DR                     (* (reg8 *) turnCompleteSensor__DR)
/* Port Number */
#define turnCompleteSensor_PRT_NUM                (* (reg8 *) turnCompleteSensor__PRT) 
/* Connect to Analog Globals */                                                  
#define turnCompleteSensor_AG                     (* (reg8 *) turnCompleteSensor__AG)                       
/* Analog MUX bux enable */
#define turnCompleteSensor_AMUX                   (* (reg8 *) turnCompleteSensor__AMUX) 
/* Bidirectional Enable */                                                        
#define turnCompleteSensor_BIE                    (* (reg8 *) turnCompleteSensor__BIE)
/* Bit-mask for Aliased Register Access */
#define turnCompleteSensor_BIT_MASK               (* (reg8 *) turnCompleteSensor__BIT_MASK)
/* Bypass Enable */
#define turnCompleteSensor_BYP                    (* (reg8 *) turnCompleteSensor__BYP)
/* Port wide control signals */                                                   
#define turnCompleteSensor_CTL                    (* (reg8 *) turnCompleteSensor__CTL)
/* Drive Modes */
#define turnCompleteSensor_DM0                    (* (reg8 *) turnCompleteSensor__DM0) 
#define turnCompleteSensor_DM1                    (* (reg8 *) turnCompleteSensor__DM1)
#define turnCompleteSensor_DM2                    (* (reg8 *) turnCompleteSensor__DM2) 
/* Input Buffer Disable Override */
#define turnCompleteSensor_INP_DIS                (* (reg8 *) turnCompleteSensor__INP_DIS)
/* LCD Common or Segment Drive */
#define turnCompleteSensor_LCD_COM_SEG            (* (reg8 *) turnCompleteSensor__LCD_COM_SEG)
/* Enable Segment LCD */
#define turnCompleteSensor_LCD_EN                 (* (reg8 *) turnCompleteSensor__LCD_EN)
/* Slew Rate Control */
#define turnCompleteSensor_SLW                    (* (reg8 *) turnCompleteSensor__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define turnCompleteSensor_PRTDSI__CAPS_SEL       (* (reg8 *) turnCompleteSensor__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define turnCompleteSensor_PRTDSI__DBL_SYNC_IN    (* (reg8 *) turnCompleteSensor__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define turnCompleteSensor_PRTDSI__OE_SEL0        (* (reg8 *) turnCompleteSensor__PRTDSI__OE_SEL0) 
#define turnCompleteSensor_PRTDSI__OE_SEL1        (* (reg8 *) turnCompleteSensor__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define turnCompleteSensor_PRTDSI__OUT_SEL0       (* (reg8 *) turnCompleteSensor__PRTDSI__OUT_SEL0) 
#define turnCompleteSensor_PRTDSI__OUT_SEL1       (* (reg8 *) turnCompleteSensor__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define turnCompleteSensor_PRTDSI__SYNC_OUT       (* (reg8 *) turnCompleteSensor__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(turnCompleteSensor__SIO_CFG)
    #define turnCompleteSensor_SIO_HYST_EN        (* (reg8 *) turnCompleteSensor__SIO_HYST_EN)
    #define turnCompleteSensor_SIO_REG_HIFREQ     (* (reg8 *) turnCompleteSensor__SIO_REG_HIFREQ)
    #define turnCompleteSensor_SIO_CFG            (* (reg8 *) turnCompleteSensor__SIO_CFG)
    #define turnCompleteSensor_SIO_DIFF           (* (reg8 *) turnCompleteSensor__SIO_DIFF)
#endif /* (turnCompleteSensor__SIO_CFG) */

/* Interrupt Registers */
#if defined(turnCompleteSensor__INTSTAT)
    #define turnCompleteSensor_INTSTAT            (* (reg8 *) turnCompleteSensor__INTSTAT)
    #define turnCompleteSensor_SNAP               (* (reg8 *) turnCompleteSensor__SNAP)
    
	#define turnCompleteSensor_0_INTTYPE_REG 		(* (reg8 *) turnCompleteSensor__0__INTTYPE)
#endif /* (turnCompleteSensor__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_turnCompleteSensor_H */


/* [] END OF FILE */
