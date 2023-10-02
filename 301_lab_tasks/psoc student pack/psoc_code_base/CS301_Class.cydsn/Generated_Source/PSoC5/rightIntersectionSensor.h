/*******************************************************************************
* File Name: rightIntersectionSensor.h  
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

#if !defined(CY_PINS_rightIntersectionSensor_H) /* Pins rightIntersectionSensor_H */
#define CY_PINS_rightIntersectionSensor_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "rightIntersectionSensor_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 rightIntersectionSensor__PORT == 15 && ((rightIntersectionSensor__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    rightIntersectionSensor_Write(uint8 value);
void    rightIntersectionSensor_SetDriveMode(uint8 mode);
uint8   rightIntersectionSensor_ReadDataReg(void);
uint8   rightIntersectionSensor_Read(void);
void    rightIntersectionSensor_SetInterruptMode(uint16 position, uint16 mode);
uint8   rightIntersectionSensor_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the rightIntersectionSensor_SetDriveMode() function.
     *  @{
     */
        #define rightIntersectionSensor_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define rightIntersectionSensor_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define rightIntersectionSensor_DM_RES_UP          PIN_DM_RES_UP
        #define rightIntersectionSensor_DM_RES_DWN         PIN_DM_RES_DWN
        #define rightIntersectionSensor_DM_OD_LO           PIN_DM_OD_LO
        #define rightIntersectionSensor_DM_OD_HI           PIN_DM_OD_HI
        #define rightIntersectionSensor_DM_STRONG          PIN_DM_STRONG
        #define rightIntersectionSensor_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define rightIntersectionSensor_MASK               rightIntersectionSensor__MASK
#define rightIntersectionSensor_SHIFT              rightIntersectionSensor__SHIFT
#define rightIntersectionSensor_WIDTH              1u

/* Interrupt constants */
#if defined(rightIntersectionSensor__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in rightIntersectionSensor_SetInterruptMode() function.
     *  @{
     */
        #define rightIntersectionSensor_INTR_NONE      (uint16)(0x0000u)
        #define rightIntersectionSensor_INTR_RISING    (uint16)(0x0001u)
        #define rightIntersectionSensor_INTR_FALLING   (uint16)(0x0002u)
        #define rightIntersectionSensor_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define rightIntersectionSensor_INTR_MASK      (0x01u) 
#endif /* (rightIntersectionSensor__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define rightIntersectionSensor_PS                     (* (reg8 *) rightIntersectionSensor__PS)
/* Data Register */
#define rightIntersectionSensor_DR                     (* (reg8 *) rightIntersectionSensor__DR)
/* Port Number */
#define rightIntersectionSensor_PRT_NUM                (* (reg8 *) rightIntersectionSensor__PRT) 
/* Connect to Analog Globals */                                                  
#define rightIntersectionSensor_AG                     (* (reg8 *) rightIntersectionSensor__AG)                       
/* Analog MUX bux enable */
#define rightIntersectionSensor_AMUX                   (* (reg8 *) rightIntersectionSensor__AMUX) 
/* Bidirectional Enable */                                                        
#define rightIntersectionSensor_BIE                    (* (reg8 *) rightIntersectionSensor__BIE)
/* Bit-mask for Aliased Register Access */
#define rightIntersectionSensor_BIT_MASK               (* (reg8 *) rightIntersectionSensor__BIT_MASK)
/* Bypass Enable */
#define rightIntersectionSensor_BYP                    (* (reg8 *) rightIntersectionSensor__BYP)
/* Port wide control signals */                                                   
#define rightIntersectionSensor_CTL                    (* (reg8 *) rightIntersectionSensor__CTL)
/* Drive Modes */
#define rightIntersectionSensor_DM0                    (* (reg8 *) rightIntersectionSensor__DM0) 
#define rightIntersectionSensor_DM1                    (* (reg8 *) rightIntersectionSensor__DM1)
#define rightIntersectionSensor_DM2                    (* (reg8 *) rightIntersectionSensor__DM2) 
/* Input Buffer Disable Override */
#define rightIntersectionSensor_INP_DIS                (* (reg8 *) rightIntersectionSensor__INP_DIS)
/* LCD Common or Segment Drive */
#define rightIntersectionSensor_LCD_COM_SEG            (* (reg8 *) rightIntersectionSensor__LCD_COM_SEG)
/* Enable Segment LCD */
#define rightIntersectionSensor_LCD_EN                 (* (reg8 *) rightIntersectionSensor__LCD_EN)
/* Slew Rate Control */
#define rightIntersectionSensor_SLW                    (* (reg8 *) rightIntersectionSensor__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define rightIntersectionSensor_PRTDSI__CAPS_SEL       (* (reg8 *) rightIntersectionSensor__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define rightIntersectionSensor_PRTDSI__DBL_SYNC_IN    (* (reg8 *) rightIntersectionSensor__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define rightIntersectionSensor_PRTDSI__OE_SEL0        (* (reg8 *) rightIntersectionSensor__PRTDSI__OE_SEL0) 
#define rightIntersectionSensor_PRTDSI__OE_SEL1        (* (reg8 *) rightIntersectionSensor__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define rightIntersectionSensor_PRTDSI__OUT_SEL0       (* (reg8 *) rightIntersectionSensor__PRTDSI__OUT_SEL0) 
#define rightIntersectionSensor_PRTDSI__OUT_SEL1       (* (reg8 *) rightIntersectionSensor__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define rightIntersectionSensor_PRTDSI__SYNC_OUT       (* (reg8 *) rightIntersectionSensor__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(rightIntersectionSensor__SIO_CFG)
    #define rightIntersectionSensor_SIO_HYST_EN        (* (reg8 *) rightIntersectionSensor__SIO_HYST_EN)
    #define rightIntersectionSensor_SIO_REG_HIFREQ     (* (reg8 *) rightIntersectionSensor__SIO_REG_HIFREQ)
    #define rightIntersectionSensor_SIO_CFG            (* (reg8 *) rightIntersectionSensor__SIO_CFG)
    #define rightIntersectionSensor_SIO_DIFF           (* (reg8 *) rightIntersectionSensor__SIO_DIFF)
#endif /* (rightIntersectionSensor__SIO_CFG) */

/* Interrupt Registers */
#if defined(rightIntersectionSensor__INTSTAT)
    #define rightIntersectionSensor_INTSTAT            (* (reg8 *) rightIntersectionSensor__INTSTAT)
    #define rightIntersectionSensor_SNAP               (* (reg8 *) rightIntersectionSensor__SNAP)
    
	#define rightIntersectionSensor_0_INTTYPE_REG 		(* (reg8 *) rightIntersectionSensor__0__INTTYPE)
#endif /* (rightIntersectionSensor__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_rightIntersectionSensor_H */


/* [] END OF FILE */
