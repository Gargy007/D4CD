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
* @file      d4cdlcdhw_gpio_4bit.h
*
* @author    Petr Gargulak
* 
* @version   0.0.1.0
* 
* @date      Jan-6-2011
* 
* @brief     D4CD driver - gpio_4bit hardware lcd driver function header file 
*
*******************************************************************************/

#ifndef __D4CDLCDHW_GPIO_4BIT_H
#define __D4CDLCDHW_GPIO_4BIT_H

  #if (D4CD_MK_STR(D4CD_LLD_LCD_HW) == d4cdlcdhw_gpio_4bit_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4cdlcdhw_gpio_4bit_cfg.h"
    
    /******************************************************************************
    * Constants
    ******************************************************************************/
    
    #define D4CDLCD_RS_DATA 1
    #define D4CDLCD_RS_CMD  0
    
    #define D4CDLCD_RW_WRITE 0
    #define D4CDLCD_RW_READ  1
    
    /******************************************************************************
    * Types
    ******************************************************************************/

    /******************************************************************************
    * Macros 
    ******************************************************************************/
    #define SET(x)           (x##_PORT) |= (1 << (x))
    #define RESET(x)         (x##_PORT) &= ~(1 << (x))
    #define SET_ALL(x)       (x##_PORT) |= 0xFF
    #define RESET_ALL(x)     (x##_PORT) &= ~0xFF 

    #define SET_MASK(x,m)       (x##_PORT) |= m
    #define RESET_MASK(x,m)     (x##_PORT) &= ~m 


    #define OUTPUT(x)        (x##_DDR) |= (1 << (x)) 
    #define INPUT(x)         (x##_DDR) &= ~(1 << (x))
    #define OUTPUT_ALL(x)    (x##_DDR) |= 0xFF 
    #define INPUT_ALL(x)     (x##_DDR) &= ~0xFF

    #define OUTPUT_MASK(x,m)    (x##_DDR) |= m 
    #define INPUT_MASK(x,m)     (x##_DDR) &= ~m

    

    #ifndef D4CDLCD_INIT_EN
      #ifndef D4CDLCD_EN
      	#error "The Macro D4CDLCD_EN must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_INIT_EN OUTPUT(D4CDLCD_EN);
      #endif	
    #endif
    
    #ifndef D4CDLCD_ASSERT_EN
    	#ifndef D4CDLCD_EN
      	#error "The Macro D4CDLCD_EN must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_ASSERT_EN RESET(D4CDLCD_EN);
      #endif	
    #endif  
    
    #ifndef D4CDLCD_DEASSERT_EN
      #ifndef D4CDLCD_EN
      	#error "The Macro D4CDLCD_EN must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_DEASSERT_EN SET(D4CDLCD_EN);
      #endif	
    #endif
    
    #ifndef D4CDLCD_INIT_RS
      #ifndef D4CDLCD_RS
      	#error "The Macro D4CDLCD_RS must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_INIT_RS OUTPUT(D4CDLCD_RS);
      #endif	
    #endif
    
    #ifndef D4CDLCD_ASSERT_RS
    	#ifndef D4CDLCD_RS
      	#error "The Macro D4CDLCD_RS must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_ASSERT_RS RESET(D4CDLCD_RS);
      #endif	
    #endif  
    
    #ifndef D4CDLCD_DEASSERT_RS
      #ifndef D4CDLCD_RS
      	#error "The Macro D4CDLCD_RS must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_DEASSERT_RS SET(D4CDLCD_RS);
      #endif	
    #endif

    #ifndef D4CDLCD_INIT_RW
      #ifndef D4CDLCD_RW
      	#error "The Macro D4CDLCD_RW must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_INIT_RW OUTPUT(D4CDLCD_RW);
      #endif	
    #endif
    
    #ifndef D4CDLCD_ASSERT_RW
    	#ifndef D4CDLCD_RW
      	#error "The Macro D4CDLCD_RW must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_ASSERT_RW RESET(D4CDLCD_RW);
      #endif	
    #endif  
    
    #ifndef D4CDLCD_DEASSERT_RW
      #ifndef D4CDLCD_RW
      	#error "The Macro D4CDLCD_RW must be set in LCD low level driver configuration file." 
      #else
      	#define D4CDLCD_DEASSERT_RW SET(D4CDLCD_RW);
      #endif	
    #endif
    
    #ifdef D4CDLCD_BACKLIGHT

      #ifndef D4CDLCD_INIT_BACKLIGHT
      	#define D4CDLCD_INIT_BACKLIGHT OUTPUT(D4CDLCD_BACKLIGHT);
      #endif
      
      #ifndef D4CDLCD_ASSERT_BACKLIGHT
      	#define D4CDLCD_ASSERT_BACKLIGHT RESET(D4CDLCD_BACKLIGHT);
      #endif  
      
      #ifndef D4CDLCD_DEASSERT_BACKLIGHT
        #define D4CDLCD_DEASSERT_BACKLIGHT SET(D4CDLCD_BACKLIGHT);      
      #endif
      
    #endif

    #ifndef D4CDLCD_INIT_DATA
    #if defined(D4CDLCD_DATA) & defined(D4CDLCD_DATA_MASK) 
    	#define D4CDLCD_INIT_DATA OUTPUT_MASK(D4CDLCD_DATA, D4CDLCD_DATA_MASK);    	
    #else    
    	#error "The Macro D4CDLCD_DATA and D4CDLCD_DATA_MASK must be set in LCD low level driver configuration file." 
    #endif	
  #endif
  
  #ifndef D4CDLCD_DATA_WRITE
    #if defined(D4CDLCD_DATA) & defined(D4CDLCD_DATA_MASK) & defined(D4CDLCD_DATA_SHIFT)
      #define D4CDLCD_DATA_WRITE(x) D4CDLCD_DATA &= ~D4CDLCD_DATA_MASK; D4CDLCD_DATA |= (((x) << D4CDLCD_DATA_SHIFT) & D4CDLCD_DATA_MASK);    	
    #else    
    	#error "The Macro D4CDLCD_DATA, D4CDLCD_DATA_SHIFT and D4CDLCD_DATA_MASK must be set in LCD low level driver configuration file." 
    #endif	
  #endif
  
  #ifndef D4CDLCD_DATA_READ
    #if defined(D4CDLCD_DATA) & defined(D4CDLCD_DATA_MASK) & defined(D4CDLCD_DATA_SHIFT)
      #define D4CDLCD_DATA_READ(x) x = ((D4CDLCD_DATA & D4CDLCD_DATA_MASK) >> D4CDLCD_DATA_SHIFT);    	
    #else    
    	#error "The Macro D4CDLCD_DATA, D4CDLCD_DATA_SHIFT and D4CDLCD_DATA_MASK must be set in LCD low level driver configuration file." 
    #endif 	
  #endif
  
  #ifndef D4CDLCD_DATA_SET2READ    
    #if defined(D4CDLCD_DATA) & defined(D4CDLCD_DATA_MASK) 
      #define D4CDLCD_DATA_SET2READ INPUT_MASK(D4CDLCD_DATA, D4CDLCD_DATA_MASK);    	
    #else    
    	#error "The Macro D4CDLCD_DATA and D4CDLCD_DATA_MASK must be set in LCD low level driver configuration file." 
    #endif    
  #endif
  
  #ifndef D4CDLCD_DATA_SET2WRITE
    #if defined(D4CDLCD_DATA) & defined(D4CDLCD_DATA_MASK) 
      #define D4CDLCD_DATA_SET2WRITE OUTPUT_MASK(D4CDLCD_DATA, D4CDLCD_DATA_MASK);    	
    #else    
    	#error "The Macro D4CDLCD_DATA and D4CDLCD_DATA_MASK must be set in LCD low level driver configuration file." 
    #endif    
  #endif
  
    /******************************************************************************
    * Global variables
    ******************************************************************************/

    /******************************************************************************
    * Global functions
    ******************************************************************************/

  #endif
#endif /* __D4CDLCDHW_GPIO_4BIT_H */










