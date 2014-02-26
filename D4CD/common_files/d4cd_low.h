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
* @file      d4cd_low.h
*
* @author    Petr Gargulak
* 
* @version   0.0.1.0
* 
* @date      Jan-6-2011
* 
* @brief     D4CD driver low level graphic function header file 
*
*******************************************************************************/

#ifndef __D4CD_LOW_H
#define __D4CD_LOW_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/

/******************************************************************************
* Macros 
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global D4CDrv low level functions
******************************************************************************/

D4CD_BOOL D4CD_LCD_Init(void); // Hardware init of LCD


void D4CD_LCD_Line (D4CD_COOR x1, D4CD_COOR y1, D4CD_COOR x2, D4CD_COOR y2, D4CD_CHAR ch);
void D4CD_LCD_Box(D4CD_COOR x1, D4CD_COOR y1, D4CD_COOR x2, D4CD_COOR y2, D4CD_CHAR ch);
void D4CD_LCD_PutChar(D4CD_COOR x, D4CD_COOR y, D4CD_CHAR ch);
void D4CD_LCD_Rect(D4CD_COOR x1, D4CD_COOR y1, D4CD_COOR x2, D4CD_COOR y2, D4CD_CHAR ch);

void D4CD_LCD_SetCursor(D4CD_CURSOR* cursor);
void D4CD_LCD_HideCursor(void);

void D4CD_LCD_LoadUserChar(Byte* charData, Byte ix);

//#define D4CD_LCD_SetCursor D4CD_LLD_LCD.D4CDLCD_SetCursor

#endif /* __D4CD_LOW_H */










