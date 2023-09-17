/*******************************************************************************
* File Name: leftLineSensor.h  
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

#if !defined(CY_PINS_leftLineSensor_H) /* Pins leftLineSensor_H */
#define CY_PINS_leftLineSensor_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "leftLineSensor_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 leftLineSensor__PORT == 15 && ((leftLineSensor__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    leftLineSensor_Write(uint8 value);
void    leftLineSensor_SetDriveMode(uint8 mode);
uint8   leftLineSensor_ReadDataReg(void);
uint8   leftLineSensor_Read(void);
void    leftLineSensor_SetInterruptMode(uint16 position, uint16 mode);
uint8   leftLineSensor_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the leftLineSensor_SetDriveMode() function.
     *  @{
     */
        #define leftLineSensor_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define leftLineSensor_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define leftLineSensor_DM_RES_UP          PIN_DM_RES_UP
        #define leftLineSensor_DM_RES_DWN         PIN_DM_RES_DWN
        #define leftLineSensor_DM_OD_LO           PIN_DM_OD_LO
        #define leftLineSensor_DM_OD_HI           PIN_DM_OD_HI
        #define leftLineSensor_DM_STRONG          PIN_DM_STRONG
        #define leftLineSensor_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define leftLineSensor_MASK               leftLineSensor__MASK
#define leftLineSensor_SHIFT              leftLineSensor__SHIFT
#define leftLineSensor_WIDTH              1u

/* Interrupt constants */
#if defined(leftLineSensor__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in leftLineSensor_SetInterruptMode() function.
     *  @{
     */
        #define leftLineSensor_INTR_NONE      (uint16)(0x0000u)
        #define leftLineSensor_INTR_RISING    (uint16)(0x0001u)
        #define leftLineSensor_INTR_FALLING   (uint16)(0x0002u)
        #define leftLineSensor_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define leftLineSensor_INTR_MASK      (0x01u) 
#endif /* (leftLineSensor__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define leftLineSensor_PS                     (* (reg8 *) leftLineSensor__PS)
/* Data Register */
#define leftLineSensor_DR                     (* (reg8 *) leftLineSensor__DR)
/* Port Number */
#define leftLineSensor_PRT_NUM                (* (reg8 *) leftLineSensor__PRT) 
/* Connect to Analog Globals */                                                  
#define leftLineSensor_AG                     (* (reg8 *) leftLineSensor__AG)                       
/* Analog MUX bux enable */
#define leftLineSensor_AMUX                   (* (reg8 *) leftLineSensor__AMUX) 
/* Bidirectional Enable */                                                        
#define leftLineSensor_BIE                    (* (reg8 *) leftLineSensor__BIE)
/* Bit-mask for Aliased Register Access */
#define leftLineSensor_BIT_MASK               (* (reg8 *) leftLineSensor__BIT_MASK)
/* Bypass Enable */
#define leftLineSensor_BYP                    (* (reg8 *) leftLineSensor__BYP)
/* Port wide control signals */                                                   
#define leftLineSensor_CTL                    (* (reg8 *) leftLineSensor__CTL)
/* Drive Modes */
#define leftLineSensor_DM0                    (* (reg8 *) leftLineSensor__DM0) 
#define leftLineSensor_DM1                    (* (reg8 *) leftLineSensor__DM1)
#define leftLineSensor_DM2                    (* (reg8 *) leftLineSensor__DM2) 
/* Input Buffer Disable Override */
#define leftLineSensor_INP_DIS                (* (reg8 *) leftLineSensor__INP_DIS)
/* LCD Common or Segment Drive */
#define leftLineSensor_LCD_COM_SEG            (* (reg8 *) leftLineSensor__LCD_COM_SEG)
/* Enable Segment LCD */
#define leftLineSensor_LCD_EN                 (* (reg8 *) leftLineSensor__LCD_EN)
/* Slew Rate Control */
#define leftLineSensor_SLW                    (* (reg8 *) leftLineSensor__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define leftLineSensor_PRTDSI__CAPS_SEL       (* (reg8 *) leftLineSensor__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define leftLineSensor_PRTDSI__DBL_SYNC_IN    (* (reg8 *) leftLineSensor__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define leftLineSensor_PRTDSI__OE_SEL0        (* (reg8 *) leftLineSensor__PRTDSI__OE_SEL0) 
#define leftLineSensor_PRTDSI__OE_SEL1        (* (reg8 *) leftLineSensor__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define leftLineSensor_PRTDSI__OUT_SEL0       (* (reg8 *) leftLineSensor__PRTDSI__OUT_SEL0) 
#define leftLineSensor_PRTDSI__OUT_SEL1       (* (reg8 *) leftLineSensor__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define leftLineSensor_PRTDSI__SYNC_OUT       (* (reg8 *) leftLineSensor__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(leftLineSensor__SIO_CFG)
    #define leftLineSensor_SIO_HYST_EN        (* (reg8 *) leftLineSensor__SIO_HYST_EN)
    #define leftLineSensor_SIO_REG_HIFREQ     (* (reg8 *) leftLineSensor__SIO_REG_HIFREQ)
    #define leftLineSensor_SIO_CFG            (* (reg8 *) leftLineSensor__SIO_CFG)
    #define leftLineSensor_SIO_DIFF           (* (reg8 *) leftLineSensor__SIO_DIFF)
#endif /* (leftLineSensor__SIO_CFG) */

/* Interrupt Registers */
#if defined(leftLineSensor__INTSTAT)
    #define leftLineSensor_INTSTAT            (* (reg8 *) leftLineSensor__INTSTAT)
    #define leftLineSensor_SNAP               (* (reg8 *) leftLineSensor__SNAP)
    
	#define leftLineSensor_0_INTTYPE_REG 		(* (reg8 *) leftLineSensor__0__INTTYPE)
#endif /* (leftLineSensor__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_leftLineSensor_H */


/* [] END OF FILE */
