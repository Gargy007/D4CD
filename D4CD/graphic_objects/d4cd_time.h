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
* @file      d4cd_time.h
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Jan-12-2011
* 
* @brief     D4CD Driver time object header file
*
*******************************************************************************/

#ifndef __D4CD_TIME_H
#define __D4CD_TIME_H

extern const D4CD_OBJECT_SYS_FUNCTION d4cd_timeSysFunc;

/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Types
******************************************************************************/

// time configuration (goes to ROM by default)

#ifndef D4CD_TIME_F_DEFAULT
  #define D4CD_TIME_F_DEFAULT  (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED | D4CD_OBJECT_F_TABSTOP | D4CD_OBJECT_F_CURSOR_EN )
#endif

#ifndef D4CD_TIME_SEL_FOCUS_CHAR
  #define D4CD_TIME_SEL_FOCUS_CHAR '>'
#endif

#ifndef D4CD_TIME_SEL_NORMAL_CHAR
  #define D4CD_TIME_SEL_NORMAL_CHAR ' '
#endif

#ifndef D4CD_TIME_HOUR_MIN_CHAR
  #define D4CD_TIME_HOUR_MIN_CHAR ':'
#endif

#ifndef D4CD_TIME_MIN_SEC_CHAR
  #define D4CD_TIME_MIN_SEC_CHAR '.'
#endif

#ifndef D4CD_TIME_KEY_SCANCODE_PLUS
  #define D4CD_TIME_KEY_SCANCODE_PLUS D4CD_KEY_SCANCODE_UP
#endif

#ifndef D4CD_TIME_KEY_SCANCODE_MINUS
  #define D4CD_TIME_KEY_SCANCODE_MINUS D4CD_KEY_SCANCODE_DOWN
#endif


#if (D4CD_TIME_SEL_FOCUS_CHAR != 0) && (D4CD_TIME_SEL_NORMAL_CHAR != 0)
  #define  D4CD_TIME_SEL_CHAR D4CD_TRUE
#else
  #define  D4CD_TIME_SEL_CHAR D4CD_FALSE
#endif


typedef struct
{
  Byte seconds;
  Byte minutes;
  Byte hours;  
}D4CD_TIME_VALUE;

typedef union
{
  D4CD_BIT_FIELD all;
  struct
  {
    unsigned bVisibleSec  :1;
    unsigned bVisibleMin  :1;
    unsigned bVisibleHour :1;
    unsigned bMode12_24   :1;
    unsigned bSetSec  :1;
    unsigned bSetMin  :1;
    unsigned bSetHour :1;
    unsigned bReserved    :1;
  }bits;
  struct
  {
    unsigned bVisible   :3;
    unsigned bMode12_24 :1;
    unsigned bSet       :3;
    unsigned bReserved     :1;
  }mergedBits;
  
}D4CD_TIME_STATUS;


typedef struct
{
  D4CD_TIME_VALUE time;
  D4CD_TIME_VALUE new_time;
  D4CD_TIME_STATUS status;
}D4CD_TIME_DATA;


typedef struct 
{
    D4CD_POINT scrPos;        // position on the screen 
    D4CD_SIZE  scrSize;       // size on the screen (focus rect only, bitmaps have own size)
    D4CD_TIME_DATA*  pData;    
    void (*OnSet)(D4CD_OBJECT* pThis);
} D4CD_TIME;


/******************************************************************************
* Macros 
******************************************************************************/

// getting the TIME structure from general OBJECT
#define D4CD_GET_TIME(pObj) ((D4CD_TIME*)((pObj)->pParam))
#define D4CD_GET_TIME_DATA(pObj) ((D4CD_TIME_DATA*)(D4CD_GET_TIME(pObj)->pData))


/*********************************************************
*
* static declaration macros
*
*********************************************************/

#define D4CD_DECLARE_TIME(name, x, y, cx, flags, pUser, OnSet, pOnUsrMsg) \
    static D4CD_TIME_DATA  name##_data = {{ 0, 0, 0}, { 0, 0, 0}, { 0x07 }}; \
    static const D4CD_TIME name##_params = \
    { \
        { x, y },   /* scrPos */ \
        { cx, 0 }, /* scrSize */ \
        &(name##_data), /* Status clear */ \
        OnSet     /* Change event */ \
    }; \
    \
    D4CD_DECLARE_OBJECT(name, pOnUsrMsg,  &d4cd_timeSysFunc, &(name##_params), flags, pUser)

#define D4CD_DECLARE_STD_TIME(name, x, y, cx, OnSet) \
    D4CD_DECLARE_TIME(name, x, y, cx, (D4CD_TIME_F_DEFAULT), NULL, OnSet, NULL)

#define D4CD_DECLARE_STD_TIME_AUTOSIZE(name, x, y, OnSet) \
    D4CD_DECLARE_STD_TIME(name, x, y, 0, OnSet)

/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* Object API
*
*********************************************************/
void D4CD_TimeAddSecond(D4CD_OBJECT_PTR pThis);
void D4CD_TimeSetValue(D4CD_OBJECT_PTR pThis, Byte hours, Byte minutes, Byte seconds);
D4CD_TIME_VALUE* D4CD_TimeGetValue(D4CD_OBJECT_PTR pThis);
void D4CD_TimeSetVisibility(D4CD_OBJECT_PTR pThis, D4CD_BOOL hours, D4CD_BOOL minutes, D4CD_BOOL seconds);
void D4CD_TimeSetMode(D4CD_OBJECT_PTR pThis, D4CD_BOOL enable24mode);

#endif /*__D4CD_TIME_H*/












