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
* @file      d4cd_types.h
*
* @author    Petr Gargulak
* 
* @version   0.0.1.0
* 
* @date      Jan-6-2011
* 
* @brief     D4CD Driver global types header file
*
*******************************************************************************/


#ifndef __D4CD_TYPES_H
#define __D4CD_TYPES_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/

#define D4CD_FALSE 0
#define D4CD_TRUE  1


#define  D4CD_HC08      1
#define  D4CD_HCS08     2
#define  D4CD_HC12      10
#define  D4CD_HCS12     11
#define  D4CD_HCS12X    12
#define  D4CD_MCF51     50
#define  D4CD_MCF52     51
#define  D4CD_MCF53     52
#define  D4CD_MCF54     53
#define  D4CD_KINETIS   100

#ifndef NULL
  #define NULL (void*)0
#endif

#ifndef D4CD_USE_STANDARD_TYPES
  #define D4CD_USE_STANDARD_TYPES D4CD_FALSE
  #warning "The D4CDrv is using own standard types Byte, Word etc. , because the D4CD_USE_STANDARD_TYPES is not defined!"
#endif  

#if (D4CD_USE_STANDARD_TYPES != D4CD_TRUE)
  #ifndef Byte
    typedef  unsigned char      Byte;
  #endif
  #ifndef sByte
  typedef    signed char      sByte;
  #endif
  #ifndef Word
  typedef  unsigned short     Word;
  #endif
  #ifndef sWord
  typedef    signed short     sWord;
  #endif
  #ifndef LWord
  typedef  unsigned long       LWord;
  #endif
  #ifndef sLWord
  typedef    signed long       sLWord;
  #endif
  #ifndef DLWord
  typedef  unsigned long long     DLWord;
  #endif
  #ifndef sDLWord
  typedef    signed long long     sDLWord;
  #endif
#endif


#if (D4CD_MCU_TYPE == D4CD_HC08) || (D4CD_MCU_TYPE == D4CD_HCS08)
  typedef Byte D4CD_BIT_FIELD; 
  typedef Byte D4CD_BOOL;
#elif (D4CD_MCU_TYPE == D4CD_HC12) || (D4CD_MCU_TYPE == D4CD_HCS12) || (D4CD_MCU_TYPE == D4CD_HCS12X)
  typedef Word D4CD_BIT_FIELD;
  typedef Word D4CD_BOOL;  
#elif (D4CD_MCU_TYPE == D4CD_MCF51) || (D4CD_MCU_TYPE == D4CD_MCF52) || (D4CD_MCU_TYPE == D4CD_KINETIS)
  typedef LWord D4CD_BIT_FIELD;
  typedef LWord D4CD_BOOL;
#else
  #error "Unsupported MCU type for D4CD_BIT_FIELD/D4CD_BOOL type!"
#endif 


#ifdef D4CD_COOR_TYPE
  typedef D4CD_COOR_TYPE D4CD_COOR;
#else
  typedef Byte D4CD_COOR;  
#endif

typedef struct 
{
    D4CD_COOR x;
    D4CD_COOR y;
} D4CD_POINT;

typedef struct 
{
    D4CD_COOR cx;
    D4CD_COOR cy;
} D4CD_SIZE;

// Char type definition
typedef char D4CD_CHAR;

typedef Byte D4CD_CHAR_DATA;

typedef Word D4CD_INDEX;

typedef Byte D4CD_KEYS;  

typedef Byte D4CD_KEY_SCANCODE;


typedef enum
{
  D4CD_OBJECT_BUTTON,
  D4CD_OBJECT_CHECKBOX,
  D4CD_OBJECT_GRAPH,
  D4CD_OBJECT_LABEL,
  D4CD_OBJECT_MENU,
  D4CD_OBJECT_SLIDER,
  D4CD_OBJECT_TIME,
  D4CD_OBJECT_DATE,
  D4CD_OBJECT_LIST_BOX,
  D4CD_OBJECT_NUMBER,
  D4CD_OBJECT_PSWRD,
  D4CD_OBJECT_UNKNOWN  
}D4CD_OBJECT_TYPE;

/******************************************************************************
* Macros 
******************************************************************************/



#endif /* __D4CD_TYPES_H */
