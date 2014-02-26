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
* @file      d4cd_lldapi.h
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Feb-14-2011
* 
* @brief     D4CD driver - resistive touch screen driver function header file 
*
*******************************************************************************/

#ifndef __D4CD_LLD_API_H
#define __D4CD_LLD_API_H


// LCD low level driver API functions

typedef enum 
{
  D4CDHW_PIN_PULL_UP_ON,
  D4CDHW_PIN_PULL_UP_OFF,
  D4CDHW_PIN_OUT,
  D4CDHW_PIN_IN,  
  D4CDHW_PIN_SET_0,
  D4CDHW_PIN_SET_1,
  D4CDHW_PIN_GET,
  D4CDHW_PIN_ADC_ON,
  D4CDHW_PIN_ADC_OFF
}D4CDHW_PIN_STATE;

typedef enum 
{
  D4CDLCD_RESET_PIN,
  D4CDLCD_BACKLIGHT_PIN  
}D4CDLCDHW_PINS;



typedef struct D4CDLCD_FUNCTIONS_S
{
  unsigned char (*D4CDLCD_Init)(void);
  unsigned char (*D4CDLCD_GoTo)(unsigned char x, unsigned char y);
  unsigned char (*D4CDLCD_Send_Char)(unsigned char ch);
  unsigned char (*D4CDLCD_Read_Char)(void);
  unsigned char (*D4CDLCD_WriteUserChar)(unsigned char *pData, unsigned char ix);
  unsigned char (*D4CDLCD_SetCursor)(unsigned char enable, unsigned char blink);
  void (*D4CDLCD_Delay_ms)(unsigned short period);
  unsigned char (*D4CDLCD_DeInit)(void);
}D4CDLCD_FUNCTIONS;


typedef struct D4CDLCDHW_FUNCTIONS_S
{
  unsigned char (*D4CDLCDHW_Init)(void);
  unsigned char (*D4CDLCDHW_SendData)(unsigned char value);
  unsigned char (*D4CDLCDHW_SendCmd)(unsigned char Cmd);  
  unsigned char (*D4CDLCDHW_SendNibbleCmd)(unsigned char Cmd);  
  unsigned char (*D4CDLCDHW_ReadData)(void);
  unsigned char (*D4CDLCDHW_ReadCmd)(void);  
  unsigned char (*D4CDLCDHW_PinCtl)(D4CDLCDHW_PINS pinId, D4CDHW_PIN_STATE setState);
  unsigned char (*D4CDLCDHW_DeInit)(void);
}D4CDLCDHW_FUNCTIONS;


// Include the right low level driver for LCD/TFT

#ifndef D4CD_LLD_LCD
  #error "You must #define symbol D4CD_LLD_LCD in d4cd_user_cfg.h header file"
#endif

#ifndef D4CD_LLD_LCD_HW
  #error "You must #define symbol D4CD_LLD_LCD_HW in d4cd_user_cfg.h header file"
#endif

extern const D4CDLCD_FUNCTIONS D4CD_LLD_LCD;
extern const D4CDLCDHW_FUNCTIONS D4CD_LLD_LCD_HW;

#endif /* __D4CD_LLD_API_H */










