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
* @file      d4cdlcd_hd44780.h
*
* @author    Petr Gargulak
* 
* @version   0.0.1.0
* 
* @date      Jan-6-2011
* 
* @brief     D4CD driver - hd44780 lcd driver function header file 
*
*******************************************************************************/

#ifndef __D4CDLCD_HD44780_H
#define __D4CDLCD_HD44780_H

  #if (D4CD_MK_STR(D4CD_LLD_LCD) == d4cdlcd_hd44780_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4cdlcd_hd44780_cfg.h"
    
    /******************************************************************************
    * Macros 
    ******************************************************************************/
     
    #ifndef D4CD_DISPLAY_LINE_CNT
      #define D4CD_DISPLAY_LINE_CNT 2
      #warning The display line count is set to default: D4CD_DISPLAY_LINE_CNT = 2.
    #endif
    
    #ifndef D4CD_DISPLAY_INTERFACE_MODE
      #define D4CD_DISPLAY_INTERFACE_MODE 8
      #warning The display interface mode is set to default: D4CD_DISPLAY_INTERFACE_MODE = 8.
    #endif  
    
    /******************************************************************************
    * Constants
    ******************************************************************************/
   
    #define D4CDLCD_CMD_DISPLAYOFF 0x08 // display off
    #define D4CDLCD_CMD_DISPLAYON 0x0c // display on
    #define D4CDLCD_CMD_CLS 0x01 // CLS
    #define D4CDLCD_CMD_CURSORMODE 0x06 // increment mode, cursor moves
    #define D4CDLCD_CMD_SHIFT 0x10 // can shift cursor or display in both direction 

    #if (D4CD_DISPLAY_LINE_CNT == 1)
      #define D4CDLCD_CMD_RESETMODE 0x30  // RESET, 8 bit bus, 1 line display
      #define D4CDLCD_CMD_MODE4BIT 0x28 // 4 bit mode, 1 line!
      #define D4CDLCD_CMD_MODE8BIT 0x38 // 8 bit mode, 1 lines!
      
    #elif (D4CD_DISPLAY_LINE_CNT == 2) | (D4CD_DISPLAY_LINE_CNT == 4)
      #define D4CDLCD_CMD_RESETMODE 0x38  // RESET, 8 bit bus, 2 or 4 lines display
      #define D4CDLCD_CMD_MODE4BIT 0x28 // 4 bit mode, 2 lines!
      #define D4CDLCD_CMD_MODE8BIT 0x38 // 8 bit mode, 2 lines!
      
    #else
      #error Unsupported Dispaly line cnt in D4CD_DISPLAY_LINE_CNT
    #endif

    #define D4CDLCD_CMD_RETURNCURSOR 0x02 // cursor back (long execution)
    

    #define D4CDLCD_SHIFT_CUR_L 0x00
    #define D4CDLCD_SHIFT_CUR_R 0x04
    #define D4CDLCD_SHIFT_DIS_L 0x08
    #define D4CDLCD_SHIFT_DIS_R 0x0C
   
    #ifndef D4CDLCD_DDRAM
      #define D4CDLCD_DDRAM 0x80 // DDRAM address offset
    #endif
    #ifndef D4CDLCD_CGRAM 
      #define D4CDLCD_CGRAM 0x40 // CGRAM address offset
    #endif
    
    
    #ifndef D4CDLCD_CHAR_HEIGHT
      #define D4CDLCD_CHAR_HEIGHT 8
    #endif  
    
    #if (D4CD_DISPLAY_INTERFACE_MODE == 4)
      #define D4CDLCD_CMD_MODE D4CDLCD_CMD_MODE4BIT  // RESET, 8 bit bus, 1 line display
      
    #elif (D4CD_DISPLAY_INTERFACE_MODE == 8)
      #define D4CDLCD_CMD_MODE D4CDLCD_CMD_MODE8BIT  // RESET, 8 bit bus, 2 or 4 lines display
      
    #else
      #error Unsupported Display interface mode in D4CD_DISPLAY_INTERFACE_MODE
    #endif

   
    /******************************************************************************
    * Types
    ******************************************************************************/

     
      
    /******************************************************************************
    * Global variables
    ******************************************************************************/

    /******************************************************************************
    * Global functions
    ******************************************************************************/

  #endif
#endif /* __D4CDLCD_HD44780_H */










