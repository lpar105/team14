/*******************************************************************************
* File Name: rightLineSensor.h  
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

#if !defined(CY_PINS_rightLineSensor_H) /* Pins rightLineSensor_H */
#define CY_PINS_rightLineSensor_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "rightLineSensor_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 rightLineSensor__PORT == 15 && ((rightLineSensor__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    rightLineSensor_Write(uint8 value);
void    rightLineSensor_SetDriveMode(uint8 mode);
uint8   rightLineSensor_ReadDataReg(void);
uint8   rightLineSensor_Read(void);
void    rightLineSensor_SetInterruptMode(uint16 position, uint16 mode);
uint8   rightLineSensor_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the rightLineSensor_SetDriveMode() function.
     *  @{
     */
        #define rightLineSensor_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define rightLineSensor_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define rightLineSensor_DM_RES_UP          PIN_DM_RES_UP
        #define rightLineSensor_DM_RES_DWN         PIN_DM_RES_DWN
        #define rightLineSensor_DM_OD_LO           PIN_DM_OD_LO
        #define rightLineSensor_DM_OD_HI           PIN_DM_OD_HI
        #define rightLineSensor_DM_STRONG          PIN_DM_STRONG
        #define rightLineSensor_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define rightLineSensor_MASK               rightLineSensor__MASK
#define rightLineSensor_SHIFT              rightLineSensor__SHIFT
#define rightLineSensor_WIDTH              1u

/* Interrupt constants */
#if defined(rightLineSensor__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in rightLineSensor_SetInterruptMode() function.
     *  @{
     */
        #define rightLineSensor_INTR_NONE      (uint16)(0x0000u)
        #define rightLineSensor_INTR_RISING    (uint16)(0x0001u)
        #define rightLineSensor_INTR_FALLING   (uint16)(0x0002u)
        #define rightLineSensor_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define rightLineSensor_INTR_MASK      (0x01u) 
#endif /* (rightLineSensor__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define rightLineSensor_PS                     (* (reg8 *) rightLineSensor__PS)
/* Data Register */
#define rightLineSensor_DR                     (* (reg8 *) rightLineSensor__DR)
/* Port Number */
#define rightLineSensor_PRT_NUM                (* (reg8 *) rightLineSensor__PRT) 
/* Connect to Analog Globals */                                                  
#define rightLineSensor_AG                     (* (reg8 *) rightLineSensor__AG)                       
/* Analog MUX bux enable */
#define rightLineSensor_AMUX                   (* (reg8 *) rightLineSensor__AMUX) 
/* Bidirectional Enable */                                                        
#define rightLineSensor_BIE                    (* (reg8 *) rightLineSensor__BIE)
/* Bit-mask for Aliased Register Access */
#define rightLineSensor_BIT_MASK               (* (reg8 *) rightLineSensor__BIT_MASK)
/* Bypass Enable */
#define rightLineSensor_BYP                    (* (reg8 *) rightLineSensor__BYP)
/* Port wide control signals */                                                   
#define rightLineSensor_CTL                    (* (reg8 *) rightLineSensor__CTL)
/* Drive Modes */
#define rightLineSensor_DM0                    (* (reg8 *) rightLineSensor__DM0) 
#define rightLineSensor_DM1                    (* (reg8 *) rightLineSensor__DM1)
#define rightLineSensor_DM2                    (* (reg8 *) rightLineSensor__DM2) 
/* Input Buffer Disable Override */
#define rightLineSensor_INP_DIS                (* (reg8 *) rightLineSensor__INP_DIS)
/* LCD Common or Segment Drive */
#define rightLineSensor_LCD_COM_SEG            (* (reg8 *) rightLineSensor__LCD_COM_SEG)
/* Enable Segment LCD */
#define rightLineSensor_LCD_EN                 (* (reg8 *) rightLineSensor__LCD_EN)
/* Slew Rate Control */
#define rightLineSensor_SLW                    (* (reg8 *) rightLineSensor__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define rightLineSensor_PRTDSI__CAPS_SEL       (* (reg8 *) rightLineSensor__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define rightLineSensor_PRTDSI__DBL_SYNC_IN    (* (reg8 *) rightLineSensor__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define rightLineSensor_PRTDSI__OE_SEL0        (* (reg8 *) rightLineSensor__PRTDSI__OE_SEL0) 
#define rightLineSensor_PRTDSI__OE_SEL1        (* (reg8 *) rightLineSensor__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define rightLineSensor_PRTDSI__OUT_SEL0       (* (reg8 *) rightLineSensor__PRTDSI__OUT_SEL0) 
#define rightLineSensor_PRTDSI__OUT_SEL1       (* (reg8 *) rightLineSensor__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define rightLineSensor_PRTDSI__SYNC_OUT       (* (reg8 *) rightLineSensor__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(rightLineSensor__SIO_CFG)
    #define rightLineSensor_SIO_HYST_EN        (* (reg8 *) rightLineSensor__SIO_HYST_EN)
    #define rightLineSensor_SIO_REG_HIFREQ     (* (reg8 *) rightLineSensor__SIO_REG_HIFREQ)
    #define rightLineSensor_SIO_CFG            (* (reg8 *) rightLineSensor__SIO_CFG)
    #define rightLineSensor_SIO_DIFF           (* (reg8 *) rightLineSensor__SIO_DIFF)
#endif /* (rightLineSensor__SIO_CFG) */

/* Interrupt Registers */
#if defined(rightLineSensor__INTSTAT)
    #define rightLineSensor_INTSTAT            (* (reg8 *) rightLineSensor__INTSTAT)
    #define rightLineSensor_SNAP               (* (reg8 *) rightLineSensor__SNAP)
    
	#define rightLineSensor_0_INTTYPE_REG 		(* (reg8 *) rightLineSensor__0__INTTYPE)
#endif /* (rightLineSensor__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_rightLineSensor_H */


/* [] END OF FILE */
