/**************************************************************************
* 
* Copyright 2014 by Petr Gargulak. eGUI/D4CD Community.
* Copyright 2011-2013 by Petr Gargulak. Freescale Semiconductor, Inc.
*
***************************************************************************
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License Version 3 
* or later (the "LGPL").
*
* As a special exception, the copyright holders of the D4CD project give you
* permission to link the D4CD sources with independent modules to produce an
* executable, regardless of the license terms of these independent modules,
* and to copy and distribute the resulting executable under terms of your 
* choice, provided that you also meet, for each linked independent module,
* the terms and conditions of the license of that module.
* An independent module is a module which is not derived from or based 
* on this library. 
* If you modify the D4CD sources, you may extend this exception 
* to your version of the D4CD sources, but you are not obligated 
* to do so. If you do not wish to do so, delete this
* exception statement from your version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* You should have received a copy of the GNU General Public License
* and the GNU Lesser General Public License along with this program.
* If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************//*!
*
* @file      d4cdlcdhw_common.example.h
*
* @author    Petr Gargulak
* 
* @version   0.0.1.0
* 
* @date      Jan-6-2011
* 
* @brief     D4D low level driver SSD1289 hardware level LCD header configuration file 
*
*******************************************************************************/

#ifndef __D4DLCD_SSD1289_CFG_H
#define __D4DLCD_SSD1289_CFG_H

  /******************************************************************************
  * includes
  ******************************************************************************/
  
  #include "derivative.h"       /* include peripheral declarations and more for S08 and CV1 */
  //#include "support_common.h" /* include peripheral declarations and more for CV2 */


  /******************************************************************************
  * Constants
  ******************************************************************************/
 
  
  // Define the index of used communication interface
  #define D4DLCD_COMM_HW_ID 1
  

  
//#define D4DLCD_DISPLAY_MCU_USER_INIT;
  
  
 
  /*******************************************
  * Signals definition
  *******************************************/
  #define D4DLCD_CS              7         // CS   
  #define D4DLCD_CS_PORT         PTED      // CS 
  #define D4DLCD_CS_DDR          PTEDD     // CS    

  #define D4DLCD_DC              3         // DC 
  #define D4DLCD_DC_PORT         PTFD      // DC 
  #define D4DLCD_DC_DDR          PTFDD     // DC 
    

  /*********** Power macros - for modification uncoment and modify ***********/
  
  //#define D4DLCD_INIT_CS OUTPUT(D4DLCD_CS);
  //#define D4DLCD_ASSERT_CS RESET(D4DLCD_CS);
  //#define D4DLCD_DEASSERT_CS SET(D4DLCD_CS);
  
  
  //#define D4DLCD_INIT_DC OUTPUT(D4DLCD_DC);
  //#define D4DLCD_ASSERT_DC RESET(D4DLCD_DC);
  //#define D4DLCD_DEASSERT_DC SET(D4DLCD_DC);

#endif /* __D4DLCD_SSD1289_CFG_H */










