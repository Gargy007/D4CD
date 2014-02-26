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
* @file      d4cdlcdhw_gpio_8bit.h
*
* @author    Petr Gargulak
* 
* @version   0.0.1.0
* 
* @date      Jan-6-2011
* 
* @brief     D4CD driver - gpio_8bit hardware lcd driver function header file 
*
*******************************************************************************/

#ifndef __D4CDLCDHW_GPIO_8BIT_H
#define __D4CDLCDHW_GPIO_8BIT_H

  #if (D4CD_MK_STR(D4CD_LLD_LCD_HW) == d4cdlcdhw_gpio_8bit_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4cdlcdhw_gpio_8bit_cfg.h"
    
    /******************************************************************************
    * Constants
    ******************************************************************************/
   
    /******************************************************************************
    * Types
    ******************************************************************************/

    /******************************************************************************
    * Macros 
    ******************************************************************************/
    #define GPIO_8BIT_SET(x)           (x##_PORT) |= (1 << (x))
    #define GPIO_8BIT_RESET(x)         (x##_PORT) &= ~(1 << (x))
    #define GPIO_8BIT_SET_ALL(x)       (x##_PORT) |= 0xFF
    #define GPIO_8BIT_RESET_ALL(x)     (x##_PORT) &= ~0xFF 

    #define GPIO_8BIT_OUTPUT(x)        (x##_DDR) |= (1 << (x)) 
    #define GPIO_8BIT_INPUT(x)         (x##_DDR) &= ~(1 << (x))
    #define GPIO_8BIT_OUTPUT_ALL(x)    (x##_DDR) |= 0xFF 
    #define GPIO_8BIT_INPUT_ALL(x)     (x##_DDR) &= ~0xFF

    #ifndef D4CDLCD_INIT_EN
      #ifndef D4CDLCD_EN
      	#error "The Macro D4CDLCD_EN must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_INIT_EN GPIO_8BIT_OUTPUT(D4CDLCD_EN);
      #endif	
    #endif
    
    #ifndef D4CDLCD_ASSERT_EN
    	#ifndef D4CDLCD_EN
      	#error "The Macro D4CDLCD_EN must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_ASSERT_EN GPIO_8BIT_RESET(D4CDLCD_EN);
      #endif	
    #endif  
    
    #ifndef D4CDLCD_DEASSERT_EN
      #ifndef D4CDLCD_EN
      	#error "The Macro D4CDLCD_EN must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_DEASSERT_EN GPIO_8BIT_SET(D4CDLCD_EN);
      #endif	
    #endif
    
    #ifndef D4CDLCD_INIT_RS
      #ifndef D4CDLCD_RS
      	#error "The Macro D4CDLCD_RS must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_INIT_RS GPIO_8BIT_OUTPUT(D4CDLCD_RS);
      #endif	
    #endif
    
    #ifndef D4CDLCD_ASSERT_RS
    	#ifndef D4CDLCD_RS
      	#error "The Macro D4CDLCD_RS must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_ASSERT_RS GPIO_8BIT_RESET(D4CDLCD_RS);
      #endif	
    #endif  
    
    #ifndef D4CDLCD_DEASSERT_RS
      #ifndef D4CDLCD_RS
      	#error "The Macro D4CDLCD_RS must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_DEASSERT_RS GPIO_8BIT_SET(D4CDLCD_RS);
      #endif	
    #endif

    #ifndef D4CDLCD_INIT_RW
      #ifndef D4CDLCD_RW
      	#error "The Macro D4CDLCD_RW must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_INIT_RW GPIO_8BIT_OUTPUT(D4CDLCD_RW);
      #endif	
    #endif
    
    #ifndef D4CDLCD_ASSERT_RW
    	#ifndef D4CDLCD_RW
      	#error "The Macro D4CDLCD_RW must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_ASSERT_RW GPIO_8BIT_RESET(D4CDLCD_RW);
      #endif	
    #endif  
    
    #ifndef D4CDLCD_DEASSERT_RW
      #ifndef D4CDLCD_RW
      	#error "The Macro D4CDLCD_RW must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_DEASSERT_RW GPIO_8BIT_SET(D4CDLCD_RW);
      #endif	
    #endif
    
    #ifdef D4CDLCD_BACKLIGHT

      #ifndef D4CDLCD_INIT_BACKLIGHT
      	#define D4CDLCD_INIT_BACKLIGHT GPIO_8BIT_OUTPUT(D4CDLCD_BACKLIGHT);
      #endif
      
      #ifndef D4CDLCD_ASSERT_BACKLIGHT
      	#define D4CDLCD_ASSERT_BACKLIGHT GPIO_8BIT_RESET(D4CDLCD_BACKLIGHT);
      #endif  
      
      #ifndef D4CDLCD_DEASSERT_BACKLIGHT
        #define D4CDLCD_DEASSERT_BACKLIGHT GPIO_8BIT_SET(D4CDLCD_BACKLIGHT);      
      #endif
      
    #endif

    #ifndef D4CDLCD_INIT_DATA
    #ifndef D4CDLCD_DATA
    	#error "The Macro D4CDLCD_DATA must be set in LCD low level driver configuration file." 
    #else    
    	#define D4CDLCD_INIT_DATA GPIO_8BIT_OUTPUT_ALL(D4CDLCD_DATA);
    #endif	
  #endif
  
  #ifndef D4CDLCD_DATA_WRITE
    #ifndef D4CDLCD_DATA
    	#error "The Macro D4CDLCD_DATA must be set in LCD low level driver configuration file." 
    #else    
    	#define D4CDLCD_DATA_WRITE(x) D4CDLCD_DATA = x;
    #endif	
  #endif
  
  #ifndef D4CDLCD_DATA_READ
    #ifndef D4CDLCD_DATA
    	#error "The Macro D4CDLCD_DATA must be set in LCD low level driver configuration file." 
    #else    
    	#define D4CDLCD_DATA_READ(x) x = D4CDLCD_DATA;
    #endif	
  #endif
  
  #ifndef D4CDLCD_DATA_SET2READ
    #ifndef D4CDLCD_DATA
    	#error "The Macro D4CDLCD_DATA must be set in LCD low level driver configuration file." 
    #else    
    	#define D4CDLCD_DATA_SET2READ GPIO_8BIT_INPUT_ALL(D4CDLCD_DATA);
    #endif	
  #endif
  
  #ifndef D4CDLCD_DATA_SET2WRITE
    #ifndef D4CDLCD_DATA
    	#error "The Macro D4CDLCD_DATA must be set in LCD low level driver configuration file." 
    #else    
    	#define D4CDLCD_DATA_SET2WRITE GPIO_8BIT_OUTPUT_ALL(D4CDLCD_DATA);
    #endif	
  #endif
  
    /******************************************************************************
    * Global variables
    ******************************************************************************/

    /******************************************************************************
    * Global functions
    ******************************************************************************/

  #endif
#endif /* __D4CDLCDHW_GPIO_8BIT_H */










