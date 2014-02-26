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
* @file      d4cd_date.h
*
* @author    Petr Gargulak
* 
* @version   0.0.4.0
* 
* @date      Feb-14-2011
* 
* @brief     D4CD Driver date object header file
*
*******************************************************************************/

#ifndef __D4CD_DATE_H
#define __D4CD_DATE_H

extern const D4CD_OBJECT_SYS_FUNCTION d4cd_dateSysFunc;

/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Types
******************************************************************************/

// date configuration (goes to ROM by default)

#ifndef D4CD_DATE_F_DEFAULT
  #define D4CD_DATE_F_DEFAULT  (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED | D4CD_OBJECT_F_TABSTOP | D4CD_OBJECT_F_CURSOR_EN )
#endif

#ifndef D4CD_DATE_SEL_FOCUS_CHAR
  #define D4CD_DATE_SEL_FOCUS_CHAR '>'
#endif

#ifndef D4CD_DATE_SEL_NORMAL_CHAR
  #define D4CD_DATE_SEL_NORMAL_CHAR ' '
#endif

#ifndef D4CD_DATE_DIVIDER_CHAR
  #define D4CD_DATE_DIVIDER_CHAR '/'
#endif

#ifndef D4CD_DATE_KEY_SCANCODE_PLUS
  #define D4CD_DATE_KEY_SCANCODE_PLUS D4CD_KEY_SCANCODE_UP
#endif

#ifndef D4CD_DATE_KEY_SCANCODE_MINUS
  #define D4CD_DATE_KEY_SCANCODE_MINUS D4CD_KEY_SCANCODE_DOWN
#endif

#ifndef D4CD_DATE_YEAR_MIN
  #define D4CD_DATE_YEAR_MIN 2000
#endif

#ifndef D4CD_DATE_YEAR_MAX
  #define D4CD_DATE_YEAR_MAX 2099
#endif


#if (D4CD_DATE_SEL_FOCUS_CHAR != 0) && (D4CD_DATE_SEL_NORMAL_CHAR != 0)
  #define  D4CD_DATE_SEL_CHAR D4CD_TRUE
#else
  #define  D4CD_DATE_SEL_CHAR D4CD_FALSE
#endif


typedef enum
{
  Monday = 0,
  Tuesday = 1,
  Wednesday = 2,
  Thursday = 3,
  Friday = 4,
  Saturday = 5,
  Sunday = 6  
}D4CD_DATE_DAY;


typedef struct
{
  Byte maxLength;
  Byte lines;
  D4CD_CHAR* pStartTable;
}D4CD_DATE_MULTITEXT;

typedef struct
{
  D4CD_DATE_DAY day;
  Byte date;
  Byte month;
  Word year;
}D4CD_DATE_VALUE;

typedef union
{
  D4CD_BIT_FIELD all;
  struct
  {
    unsigned bVisibleDay  :1;
    unsigned bVisibleDate  :1;
    unsigned bVisibleMonth  :1;
    unsigned bVisibleYear :1;    
    unsigned bSetDate  :1;
    unsigned bSetMonth  :1;
    unsigned bSetYear :1;
    unsigned bBlink    :1;
  }bits;
  struct
  {
    unsigned bVisible   :4;
    unsigned bSet       :3;
    unsigned bReserved     :1;
  }mergedBits;
}D4CD_DATE_STATUS;


typedef struct
{
  D4CD_DATE_VALUE date;
  D4CD_DATE_VALUE new_date;
  D4CD_DATE_STATUS status;
}D4CD_DATE_DATA;



typedef struct 
{
    D4CD_POINT scrPos;        // position on the screen 
    D4CD_SIZE  scrSize;       // size on the screen (focus rect only, bitmaps have own size)
    D4CD_DATE_MULTITEXT* pDayNamesTable;
    D4CD_DATE_MULTITEXT* pMonthNamesTable;
    D4CD_DATE_DATA*  pData;    
    void (*OnSet)(D4CD_OBJECT* pThis);
} D4CD_DATE;


/******************************************************************************
* Macros 
******************************************************************************/

// getting the DATE structure from general OBJECT
#define D4CD_GET_DATE(pObj) ((D4CD_DATE*)((pObj)->pParam))
#define D4CD_GET_DATE_DATA(pObj) ((D4CD_DATE_DATA*)(D4CD_GET_DATE(pObj)->pData))


/*********************************************************
*
* static declaration macros
*
*********************************************************/

#define D4CD_DECLARE_DATE(name, x, y, cx, pDayNames, pMonthNames, flags, pUser, onSet, pOnUsrMsg) \
    static D4CD_DATE_DATA  name##_data = {{ Monday, 0, 0, D4CD_DATE_YEAR_MIN}, { Monday, 0, 0, D4CD_DATE_YEAR_MIN}, { 0x0E }}; \
    static const D4CD_DATE name##_params = \
    { \
        { x, y },   /* scrPos */ \
        { cx, 0 }, /* scrSize */ \
        (D4CD_DATE_MULTITEXT*)pDayNames, /* pDayNamesTable */ \
        (D4CD_DATE_MULTITEXT*)pMonthNames, /* pMonthNamesTable */ \
        &(name##_data), /* Status clear */ \
        onSet     /* Change event */ \
    }; \
    \
    D4CD_DECLARE_OBJECT(name, pOnUsrMsg,  &d4cd_dateSysFunc, &(name##_params), flags, pUser)

#define D4CD_DECLARE_STD_DATE(name, x, y, cx, pDayNames, pMonthNames, onSet) \
    D4CD_DECLARE_DATE(name, x, y, cx, pDayNames, pMonthNames, (D4CD_DATE_F_DEFAULT), NULL, onSet, NULL)

#define D4CD_DECLARE_STD_DATE_AUTOSIZE(name, x, y, pDayNames, pMonthNames, onSet) \
    D4CD_DECLARE_STD_DATE(name, x, y, 0, pDayNames, pMonthNames, onSet)

/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* Object API
*
*********************************************************/

void D4CD_DateSetValue(D4CD_OBJECT_PTR pThis, Byte day, Byte date, Byte month, Word year);
D4CD_DATE_VALUE* D4CD_DateGetValue(D4CD_OBJECT_PTR pThis);
void D4CD_DateSetVisibility(D4CD_OBJECT_PTR pThis, D4CD_BOOL day, D4CD_BOOL date, D4CD_BOOL month, D4CD_BOOL year);
void D4CD_SprintDec9(Byte value, char *pText);
void D4CD_SprintDec99(Byte value, char *pText);

#endif /*__D4CD_DATE_H*/












