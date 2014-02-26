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
* @file      d4cdlcd_hd44780_cfg.h
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Jan-18-2011
* 
* @brief     D4CD driver - template lcd driver configuration header file 
*
*******************************************************************************/

#ifndef __D4CDLCD_TEMPLATE_CFG_H
#define __D4CDLCD_TEMPLATE_CFG_H


  /******************************************************************************
  * includes
  ******************************************************************************/
  // include here what the driver need for run for example "derivative.h"
  // #include "derivative.h"    /* include peripheral declarations and more for S08 and CV1 */ 
  
  /******************************************************************************
  * Constants
  ******************************************************************************/
  
  
  
  #define D4CD_DISPLAY_LINE_CNT 2 // Two line display
  
  #define D4CD_DISPLAY_INTERFACE_MODE 4 // 4bit interface mode
  
  //#define D4CDLCD_CGRAM 0x40 // CGRAM address offset
  //#define D4CDLCD_DDRAM 0x80 // DDRAM address offset

#endif /* __D4CDLCD_TEMPLATE_CFG_H */