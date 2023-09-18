/*******************************************************************************
* File Name: force_g.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_force_g_ALIASES_H) /* Pins force_g_ALIASES_H */
#define CY_PINS_force_g_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define force_g_0			(force_g__0__PC)
#define force_g_0_INTR	((uint16)((uint16)0x0001u << force_g__0__SHIFT))

#define force_g_INTR_ALL	 ((uint16)(force_g_0_INTR))

#endif /* End Pins force_g_ALIASES_H */


/* [] END OF FILE */
