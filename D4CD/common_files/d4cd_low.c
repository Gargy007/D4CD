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
* @file      d4cd_low.c
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Feb-14-2011
* 
* @brief     D4CD driver low level graphic function c file 
*
******************************************************************************/

/**************************************************************//*!
*
* Include headers
*
******************************************************************/

#include "d4cd.h"
#include "d4cd_private.h"
#include "d4cd_lldapi.h"


/**************************************************************//*!
*
* Global variables
*
******************************************************************/

/**************************************************************//*!
*
* Local variables
*
******************************************************************/


/**************************************************************//*!
*
* Initialize
*
******************************************************************/

D4CD_BOOL D4CD_LCD_Init(void)
{
  if(D4CD_LLD_LCD.D4CDLCD_Init == NULL)
    return D4CD_FALSE;
  if(D4CD_LLD_LCD.D4CDLCD_GoTo == NULL)
    return D4CD_FALSE;
  if(D4CD_LLD_LCD.D4CDLCD_Send_Char == NULL)
    return D4CD_FALSE;
  if(D4CD_LLD_LCD.D4CDLCD_Read_Char == NULL)
    return D4CD_FALSE;
  if(D4CD_LLD_LCD.D4CDLCD_WriteUserChar == NULL)
    return D4CD_FALSE;
  if(D4CD_LLD_LCD.D4CDLCD_Delay_ms == NULL)
    return D4CD_FALSE;
  if(D4CD_LLD_LCD.D4CDLCD_DeInit == NULL)
    return D4CD_FALSE;
  
  
  if(!D4CD_LLD_LCD.D4CDLCD_Init())
    return D4CD_FALSE; 
  
  return D4CD_TRUE;
}


void D4CD_ClearScreen(D4CD_CHAR ch) {
  D4CD_LCD_Box(0, 0, D4CD_SCREEN_CHAR_CNT, D4CD_SCREEN_LINE_CNT, ch);
}


static void D4CD_LCD_SwapCoor(D4CD_COOR* c1, D4CD_COOR* c2) {
  D4CD_COOR tmp;
  
  tmp = *c1;
  *c1 = *c2;
  *c2 = tmp;  
}


void D4CD_LCD_Box(D4CD_COOR x1, D4CD_COOR y1, D4CD_COOR x2, D4CD_COOR y2, D4CD_CHAR ch)
{
  D4CD_COOR x,y;

  //Check the coordination if are in right order
  if(x1 > x2)
    D4CD_LCD_SwapCoor(&x1, &x2);

  if(y1 > y2)
    D4CD_LCD_SwapCoor(&y1, &y2);
  
  // for all lines
  for (y = y1; y <= y2 ; y++) {
    (void)D4CD_LLD_LCD.D4CDLCD_GoTo(x1, y);
    for (x = x1; x <= x2 ; x++) {
      // and for all chars 
      // draw the specified char
      (void)D4CD_LLD_LCD.D4CDLCD_Send_Char(ch);
    }
  }
 
}

void D4CD_LCD_PutChar(D4CD_COOR x, D4CD_COOR y, D4CD_CHAR ch) {
  (void)D4CD_LLD_LCD.D4CDLCD_GoTo(x, y);
  (void)D4CD_LLD_LCD.D4CDLCD_Send_Char(ch);  
}



void D4CD_LCD_Rect(D4CD_COOR x1, D4CD_COOR y1, D4CD_COOR x2, D4CD_COOR y2, D4CD_CHAR ch)   //draw rectangle
{
  if(ch) {    
    D4CD_LCD_Line(x1, y1, x2, y1, ch);
    D4CD_LCD_Line(x2, y1, x2, y2, ch);
    D4CD_LCD_Line(x1, y2, x2, y2, ch);
    D4CD_LCD_Line(x1, y1, x1, y2, ch);
  }
}

void D4CD_LCD_Line (D4CD_COOR x1, D4CD_COOR y1, D4CD_COOR x2, D4CD_COOR y2, D4CD_CHAR ch)     //draw line - bresenham algorithm
{
   sByte delta_x, delta_y;
   sByte ix, iy;
   sByte error;

   if((x1 == x2) || (y1 == y2))  // if the line is vertical or horizontal then draw box function is faster than general bresenham algorithm
   {       
      D4CD_LCD_Box(x1 ,y1 ,x2 , y2, ch);
      
      return;
   }
   
   delta_x = (sByte)(D4CD_Abs8((sByte)(x2 - x1)) << 1);
   delta_y = (sByte)(D4CD_Abs8((sByte)(y2 - y1)) << 1);

    // if x1 == x2 or y1 == y2, then it does not matter what we set here
    ix = (sByte)((x2 > x1)?1:-1);
    iy = (sByte)((y2 > y1)?1:-1);

    D4CD_LCD_PutChar((D4CD_COOR) x1, (D4CD_COOR) y1, ch);

    if (delta_x >= delta_y)
    {
        // error may go below zero
        error = (sByte)(delta_y - (delta_x >> 1));

        while (x1 != x2)
        {
            if (error >= 0)
            {
                if (error || (ix > 0))
                {
                    y1 += iy;
                    error -= delta_x;
                }
                // else do nothing
            }
            // else do nothing

            x1 += ix;
            error += delta_y;

            D4CD_LCD_PutChar((D4CD_COOR) x1, (D4CD_COOR) y1, ch);
        }
    }
    else
    {
        // error may go below zero
        error = (sByte)(delta_x - (delta_y >> 1));

        while (y1 != y2)
        {
            if (error >= 0)
            {
                if (error || (iy > 0))
                {
                    x1 += ix;
                    error -= delta_y;
                }
                // else do nothing
            }
            // else do nothing

            y1 += iy;
            error += delta_x;

            D4CD_LCD_PutChar((D4CD_COOR) x1, (D4CD_COOR) y1, ch);
        }
    }
 }

void D4CD_LCD_SetCursor(D4CD_CURSOR* cursor)
{
  (void)D4CD_LLD_LCD.D4CDLCD_GoTo(cursor->pos.x, cursor->pos.y);
  (void)D4CD_LLD_LCD.D4CDLCD_SetCursor((unsigned char)(cursor->state.bVisible), (unsigned char)(cursor->state.bBlink));    
}

void D4CD_LCD_HideCursor(void)
{
  (void)D4CD_LLD_LCD.D4CDLCD_SetCursor(D4CD_FALSE, D4CD_FALSE);      
}


void D4CD_LCD_LoadUserChar(Byte* charData, Byte ix)
{
  if(ix > 0x0f)
    return;
  
  if(charData == NULL)
    return;
  
  (void)D4CD_LLD_LCD.D4CDLCD_WriteUserChar(charData, ix);
}
