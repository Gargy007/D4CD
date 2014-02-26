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
* @file      d4cd_font.h
*
* @author    Petr Gargulak
* 
* @version   0.0.1.0
* 
* @date      Jan-6-2011
* 
* @brief     D4CD Driver fonts header file
*
*******************************************************************************/

#ifndef __D4CD_FONT_H
#define __D4CD_FONT_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Types
******************************************************************************/


#define D4CD_TXT_PRTY_ALIGN_H_LEFT           (0x00)
#define D4CD_TXT_PRTY_ALIGN_H_RIGHT          (0x01)
#define D4CD_TXT_PRTY_ALIGN_H_CENTER         (0x02)
  
#define D4CD_TXT_PRTY_ALIGN_V_TOP            (0x00)
#define D4CD_TXT_PRTY_ALIGN_V_BOTTOM         (0x01)
#define D4CD_TXT_PRTY_ALIGN_V_CENTER         (0x02)

#if D4CD_BITFIELD_LSB_ALIGNMENT == D4CD_BITFIELD_LSB_ALLIGMENT_RIGHT
  #define D4CD_TXT_PRTY_ALIGN_H_MASK                (0x03)
  #define D4CD_TXT_PRTY_ALIGN_H_LEFT_MASK           (0x00)
  #define D4CD_TXT_PRTY_ALIGN_H_RIGHT_MASK          (0x01)
  #define D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK         (0x02)
  
  #define D4CD_TXT_PRTY_ALIGN_V_MASK                (0x0C)
  #define D4CD_TXT_PRTY_ALIGN_V_TOP_MASK            (0x00)
  #define D4CD_TXT_PRTY_ALIGN_V_BOTTOM_MASK         (0x04)
  #define D4CD_TXT_PRTY_ALIGN_V_CENTER_MASK         (0x08)
    
#else
  #define D4CD_TXT_PRTY_ALIGN_H_MASK                (0xc0)
  #define D4CD_TXT_PRTY_ALIGN_H_LEFT_MASK           (0x00)
  #define D4CD_TXT_PRTY_ALIGN_H_RIGHT_MASK          (0x40)
  #define D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK         (0x80)

  #define D4CD_TXT_PRTY_ALIGN_V_MASK                (0x30)
  #define D4CD_TXT_PRTY_ALIGN_V_TOP_MASK            (0x00)
  #define D4CD_TXT_PRTY_ALIGN_V_BOTTOM_MASK         (0x10)
  #define D4CD_TXT_PRTY_ALIGN_V_CENTER_MASK         (0x20)
  
#endif

  
typedef union 
{
    D4CD_BIT_FIELD all;
    
    struct
    {
        unsigned bAlignHoriz     :2;   // String horizontal alignment in text box
        unsigned bAlignVertic    :2;   // String vertical alignment in text box
        unsigned bReserverd     :4;   // Reserved bits        
    } bits;
} D4CD_TEXT_PROPERTIES;

typedef struct D4CD_STRING_S    // todo D4CD_STRING
{
  char *pText;
  Word buffSize;
  D4CD_TEXT_PROPERTIES *text_properties;
}D4CD_STRING;

typedef struct{
  D4CD_COOR x; 
  D4CD_COOR y; 
}D4CD_PRINT_CHAR;



/******************************************************************************
* Macros 
******************************************************************************/
#define D4CD_DECLARE_CHAR_TABLE_BEGIN(name)  const D4CD_CHAR_DATA* const name[] = {

#define D4CD_DECLARE_CHAR(pCharData) pCharData, // tab space in pixels

#define D4CD_DECLARE_CHAR_TABLE_END NULL };

#define D4CD_EXTERN_CHAR_TABLE(name) extern const D4CD_CHAR_DATA* const (name)[];


#define D4CD_PUT_UCHAR(ix) "\01##(ix)"

/******************************************************************************
* Global variables
******************************************************************************/




/******************************************************************************
* Global functions
******************************************************************************/
void D4CD_LCD_PrintStr(D4CD_COOR x, D4CD_COOR y, D4CD_CHAR* pText);
void D4CD_LoadUserCharTable(D4CD_CHAR_DATA** pCharTable);

#endif  //__D4CD_FONT_H


