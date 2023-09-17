/*******************************************************************************
* File Name: middleLineSensor.h  
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

#if !defined(CY_PINS_middleLineSensor_H) /* Pins middleLineSensor_H */
#define CY_PINS_middleLineSensor_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "middleLineSensor_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 middleLineSensor__PORT == 15 && ((middleLineSensor__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    middleLineSensor_Write(uint8 value);
void    middleLineSensor_SetDriveMode(uint8 mode);
uint8   middleLineSensor_ReadDataReg(void);
uint8   middleLineSensor_Read(void);
void    middleLineSensor_SetInterruptMode(uint16 position, uint16 mode);
uint8   middleLineSensor_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the middleLineSensor_SetDriveMode() function.
     *  @{
     */
        #define middleLineSensor_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define middleLineSensor_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define middleLineSensor_DM_RES_UP          PIN_DM_RES_UP
        #define middleLineSensor_DM_RES_DWN         PIN_DM_RES_DWN
        #define middleLineSensor_DM_OD_LO           PIN_DM_OD_LO
        #define middleLineSensor_DM_OD_HI           PIN_DM_OD_HI
        #define middleLineSensor_DM_STRONG          PIN_DM_STRONG
        #define middleLineSensor_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define middleLineSensor_MASK               middleLineSensor__MASK
#define middleLineSensor_SHIFT              middleLineSensor__SHIFT
#define middleLineSensor_WIDTH              1u

/* Interrupt constants */
#if defined(middleLineSensor__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in middleLineSensor_SetInterruptMode() function.
     *  @{
     */
        #define middleLineSensor_INTR_NONE      (uint16)(0x0000u)
        #define middleLineSensor_INTR_RISING    (uint16)(0x0001u)
        #define middleLineSensor_INTR_FALLING   (uint16)(0x0002u)
        #define middleLineSensor_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define middleLineSensor_INTR_MASK      (0x01u) 
#endif /* (middleLineSensor__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define middleLineSensor_PS                     (* (reg8 *) middleLineSensor__PS)
/* Data Register */
#define middleLineSensor_DR                     (* (reg8 *) middleLineSensor__DR)
/* Port Number */
#define middleLineSensor_PRT_NUM                (* (reg8 *) middleLineSensor__PRT) 
/* Connect to Analog Globals */                                                  
#define middleLineSensor_AG                     (* (reg8 *) middleLineSensor__AG)                       
/* Analog MUX bux enable */
#define middleLineSensor_AMUX                   (* (reg8 *) middleLineSensor__AMUX) 
/* Bidirectional Enable */                                                        
#define middleLineSensor_BIE                    (* (reg8 *) middleLineSensor__BIE)
/* Bit-mask for Aliased Register Access */
#define middleLineSensor_BIT_MASK               (* (reg8 *) middleLineSensor__BIT_MASK)
/* Bypass Enable */
#define middleLineSensor_BYP                    (* (reg8 *) middleLineSensor__BYP)
/* Port wide control signals */                                                   
#define middleLineSensor_CTL                    (* (reg8 *) middleLineSensor__CTL)
/* Drive Modes */
#define middleLineSensor_DM0                    (* (reg8 *) middleLineSensor__DM0) 
#define middleLineSensor_DM1                    (* (reg8 *) middleLineSensor__DM1)
#define middleLineSensor_DM2                    (* (reg8 *) middleLineSensor__DM2) 
/* Input Buffer Disable Override */
#define middleLineSensor_INP_DIS                (* (reg8 *) middleLineSensor__INP_DIS)
/* LCD Common or Segment Drive */
#define middleLineSensor_LCD_COM_SEG            (* (reg8 *) middleLineSensor__LCD_COM_SEG)
/* Enable Segment LCD */
#define middleLineSensor_LCD_EN                 (* (reg8 *) middleLineSensor__LCD_EN)
/* Slew Rate Control */
#define middleLineSensor_SLW                    (* (reg8 *) middleLineSensor__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define middleLineSensor_PRTDSI__CAPS_SEL       (* (reg8 *) middleLineSensor__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define middleLineSensor_PRTDSI__DBL_SYNC_IN    (* (reg8 *) middleLineSensor__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define middleLineSensor_PRTDSI__OE_SEL0        (* (reg8 *) middleLineSensor__PRTDSI__OE_SEL0) 
#define middleLineSensor_PRTDSI__OE_SEL1        (* (reg8 *) middleLineSensor__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define middleLineSensor_PRTDSI__OUT_SEL0       (* (reg8 *) middleLineSensor__PRTDSI__OUT_SEL0) 
#define middleLineSensor_PRTDSI__OUT_SEL1       (* (reg8 *) middleLineSensor__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define middleLineSensor_PRTDSI__SYNC_OUT       (* (reg8 *) middleLineSensor__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(middleLineSensor__SIO_CFG)
    #define middleLineSensor_SIO_HYST_EN        (* (reg8 *) middleLineSensor__SIO_HYST_EN)
    #define middleLineSensor_SIO_REG_HIFREQ     (* (reg8 *) middleLineSensor__SIO_REG_HIFREQ)
    #define middleLineSensor_SIO_CFG            (* (reg8 *) middleLineSensor__SIO_CFG)
    #define middleLineSensor_SIO_DIFF           (* (reg8 *) middleLineSensor__SIO_DIFF)
#endif /* (middleLineSensor__SIO_CFG) */

/* Interrupt Registers */
#if defined(middleLineSensor__INTSTAT)
    #define middleLineSensor_INTSTAT            (* (reg8 *) middleLineSensor__INTSTAT)
    #define middleLineSensor_SNAP               (* (reg8 *) middleLineSensor__SNAP)
    
	#define middleLineSensor_0_INTTYPE_REG 		(* (reg8 *) middleLineSensor__0__INTTYPE)
#endif /* (middleLineSensor__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_middleLineSensor_H */


/* [] END OF FILE */
