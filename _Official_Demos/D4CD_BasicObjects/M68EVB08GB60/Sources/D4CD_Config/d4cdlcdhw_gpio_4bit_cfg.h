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
* @file      d4cdlcdhw_gpio_4bit_cfg.h
*
* @author    Petr Gargulak
* 
* @version   0.0.1.0
* 
* @date      Jan-14-2011
* 
* @brief     D4CD driver - gpio_4bit hardware lcd driver configuration header file 
*
*******************************************************************************/

#ifndef __D4CDLCDHW_GPIO_4BIT_CFG_H
#define __D4CDLCDHW_GPIO_4BIT_CFG_H


  /******************************************************************************
  * includes
  ******************************************************************************/
  // include here what the driver need for run for example "derivative.h"
   #include "derivative.h"    /* include peripheral declarations and more for S08 and CV1 */ 
  
  /******************************************************************************
  * Constants
  ******************************************************************************/
  
  //#define D4CDLCD_DISPLAY_MCU_USER_INIT;
  
  /*******************************************
  * Signals definition
  *******************************************/
  #define D4CDLCD_EN              7         // EN   
  #define D4CDLCD_EN_PORT         PTED      // EN 
  #define D4CDLCD_EN_DDR          PTEDD     // EN    

  #define D4CDLCD_RS              6         // RS 
  #define D4CDLCD_RS_PORT         PTED      // RS 
  #define D4CDLCD_RS_DDR          PTEDD     // RS 
    
  #define D4CDLCD_RW              3         // RW 
  #define D4CDLCD_RW_PORT         PTGD      // RW 
  #define D4CDLCD_RW_DDR          PTGDD     // RW 
  
  //#define D4CDLCD_BACKLIGHT       4         // BACKLIGHT 
  //#define D4CDLCD_BACKLIGHT_PORT  PTFD      // BACKLIGHT 
  //#define D4CDLCD_BACKLIGHT_DDR   PTFDD     // BACKLIGHT 
  
  
  // I/O port CONFIG for parallel transfer

  #define D4CDLCD_DATA            PTGD      // 4bit bus
  #define D4CDLCD_DATA_DDR        PTGDD     // 4bit bus DDR 
  #define D4CDLCD_DATA_PE         PTGPE     // 4bit bus PE
  #define D4CDLCD_DATA_MASK       0xF0      // 4bit mask
  #define D4CDLCD_DATA_SHIFT      4         // 4bit shift

  /*********** Power macros - for modification uncoment and modify ***********/
  
  //#define D4CDLCD_INIT_EN OUTPUT(D4CDLCD_EN);
  //#define D4CDLCD_ASSERT_EN RESET(D4CDLCD_EN);
  //#define D4CDLCD_DEASSERT_EN SET(D4CDLCD_EN);
  
  //#define D4CDLCD_INIT_RS OUTPUT(D4CDLCD_RS);
  //#define D4CDLCD_ASSERT_RS RESET(D4CDLCD_RS);
  //#define D4CDLCD_DEASSERT_RS SET(D4CDLCD_RS);
  
  //#define D4CDLCD_INIT_RW OUTPUT(D4CDLCD_RW);
  //#define D4CDLCD_ASSERT_RW RESET(D4CDLCD_RW);
  //#define D4CDLCD_DEASSERT_RW SET(D4CDLCD_RW);
  
  //#define D4CDLCD_INIT_BACKLIGHT OUTPUT(D4CDLCD_BACKLIGHT);
  //#define D4CDLCD_ASSERT_BACKLIGHT RESET(D4CDLCD_BACKLIGHT);
  //#define D4CDLCD_DEASSERT_BACKLIGHT SET(D4CDLCD_BACKLIGHT);
  
  //#define D4CDLCD_INIT_DATA OUTPUT_ALL(D4CDLCD_DATA);
  //#define D4CDLCD_DATA_WRITE(x) D4CDLCD_DATA = x;
  //#define D4CDLCD_DATA_READ(x) x = D4CDLCD_DATA;
  //#define D4CDLCD_DATA_SET2READ INPUT_ALL(D4CDLCD_DATA);
  //#define D4CDLCD_DATA_SET2WRITE OUTPUT_ALL(D4CDLCD_DATA);

#endif /* __D4CDLCDHW_GPIO_4BIT_CFG_H */
