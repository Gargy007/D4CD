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
* @file      d4cd_number.h
*
* @author    Petr Gargulak
* 
* @version   0.0.3.0
* 
* @date      Feb-14-2011
* 
* @brief     D4CD Driver number object header file
*
*******************************************************************************/

#ifndef __D4CD_NUMBER_H
#define __D4CD_NUMBER_H

extern const D4CD_OBJECT_SYS_FUNCTION d4cd_numberSysFunc;

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/

#ifndef D4CD_NUMBER_F_DEFAULT
  #define D4CD_NUMBER_F_DEFAULT  (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED | D4CD_OBJECT_F_TABSTOP | D4CD_OBJECT_F_CURSOR_EN )
#endif

#ifndef D4CD_NUMBER_TXT_PRTY_DEFAULT
  #define D4CD_NUMBER_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)
#endif

#ifndef D4CD_NUMBER_NUM_TXT_PRTY_DEFAULT
  #define D4CD_NUMBER_NUM_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)
#endif

#ifndef D4CD_NUMBER_SEL_FOCUS_CHAR
  #define D4CD_NUMBER_SEL_FOCUS_CHAR '>'
#endif

#ifndef D4CD_NUMBER_SEL_NORMAL_CHAR
  #define D4CD_NUMBER_SEL_NORMAL_CHAR ' '
#endif

#ifndef D4CD_NUMBER_DEC_POINT_CHAR
  #define D4CD_NUMBER_DEC_POINT_CHAR '.'
#endif

#ifndef D4CD_NUMBER_KEY_SCANCODE_PLUS
  #define D4CD_NUMBER_KEY_SCANCODE_PLUS D4CD_KEY_SCANCODE_UP
#endif

#ifndef D4CD_NUMBER_KEY_SCANCODE_MINUS
  #define D4CD_NUMBER_KEY_SCANCODE_MINUS D4CD_KEY_SCANCODE_DOWN
#endif

#ifndef D4CD_NUMBER_CHANGE_STEP
  #define D4CD_NUMBER_CHANGE_STEP 5
#endif



#if (D4CD_NUMBER_SEL_FOCUS_CHAR != 0) && (D4CD_NUMBER_SEL_NORMAL_CHAR != 0)
  #define  D4CD_NUMBER_SEL_CHAR D4CD_TRUE
#else
  #define  D4CD_NUMBER_SEL_CHAR D4CD_FALSE
#endif

 
typedef Byte D4CD_NUMBER_DECIMAL_POINT;
typedef sWord D4CD_NUMBER_VALUE;

typedef union
{
  D4CD_BIT_FIELD all;
  struct
  {
    unsigned bSet :1;
  }bits;
  
}D4CD_NUMBER_STATUS;




typedef struct
{
  Byte changeStep;
  D4CD_NUMBER_VALUE value_max;
  D4CD_NUMBER_VALUE value_min;
  D4CD_NUMBER_DECIMAL_POINT decimalPoint;
}D4CD_NUMBER_LIMITS;

typedef struct
{
  D4CD_NUMBER_VALUE value;      // current value
  D4CD_NUMBER_STATUS status;
  D4CD_NUMBER_LIMITS limits;  
} D4CD_NUMBER_DATA;

// NUMBER configuration (in ROM by default)

typedef struct
{
    D4CD_CHAR* preText;          // text printed before the list box
    D4CD_CHAR* postText;         // text printed behind the list box
    D4CD_POINT scrPos;           // position on the screen
    D4CD_COOR  numSize;          // size on the screen for focus rectangle (bitmap has its own size)
    void (*OnValueChanged)(D4CD_OBJECT_PTR pThis);
    D4CD_NUMBER_DATA* pData;    
} D4CD_NUMBER;

/******************************************************************************
* Macros 
******************************************************************************/

// getting the NUMBER structure from general OBJECT
#define D4CD_GET_NUMBER(pObj) ((D4CD_NUMBER*)((pObj)->pParam))

/*********************************************************
*
* static declaration macros
*
*********************************************************/
         
#define _D4CD_DECLARE_NUMBER(type, name, preText, postText, x, y, cx, initValue, flags, pUser, pOnValch, pOnUsrMsg) \
    static D4CD_NUMBER_DATA name##_data = {initValue, 0, { D4CD_NUMBER_CHANGE_STEP, 32000, -32000, 0} }; \
    static type D4CD_NUMBER name##_params = \
    { \
        preText,   /* perText */\
        postText,   /* postText */\
        { x, y },     /* scrPos     */ \
        cx,   /* scrSize    */ \
        pOnValch,      /* OnValueCanged */ \
        &(name##_data) /* RAM-based data */ \
    }; \
    \
    D4CD_DECLARE_OBJECT(name, pOnUsrMsg, &d4cd_numberSysFunc, &(name##_params), (flags), pUser)

#define D4CD_DECLARE_NUMBER(name, preText, postText, x, y, cx, initValue, flags, pUser, pOnValch, pOnUsrMsg) \
  _D4CD_DECLARE_NUMBER(D4CD_CONST, name, preText, postText, x, y, cx, initValue, flags, pUser, pOnValch, pOnUsrMsg)

#define D4CD_DECLARE_NUMBER_INRAM(name, preText, postText, x, y, cx, initValue, flags, pUser, pOnValch, pOnUsrMsg) \
  _D4CD_DECLARE_NUMBER(D4CD_NO_CONST, name, preText, postText, x, y, cx, initValue, flags, pUser, pOnValch, pOnUsrMsg)

#define D4CD_DECLARE_NUMBER_AUTOSIZE(name, preText, postText, x, y, initValue, flags, pUser, pOnValch, pOnUsrMsg) \
  D4CD_DECLARE_NUMBER(name, preText, postText, x, y, 0, initValue, flags, pUser, pOnValch, pOnUsrMsg)

#define D4CD_DECLARE_STD_NUMBER(name, preText, postText, x, y, cx,  pOnValch) \
  D4CD_DECLARE_NUMBER(name, preText, postText, x, y, cx, 0, (D4CD_NUMBER_F_DEFAULT), NULL, pOnValch, NULL)

#define D4CD_DECLARE_STD_NUMBER_AUTOSIZE(name, preText, postText, x, y, onvalch) \
  D4CD_DECLARE_STD_NUMBER(name, preText, postText, x, y, 0, onvalch)



/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* global functions
*
*********************************************************/
void D4CD_NumberSetValue(D4CD_OBJECT_PTR pThis, D4CD_NUMBER_VALUE value);
void D4CD_NumberChangeValue(D4CD_OBJECT_PTR pThis, sWord incr);
D4CD_NUMBER_VALUE D4CD_NumberGetValue(D4CD_OBJECT_PTR pThis);
void D4CD_NumberSetLimits(D4CD_OBJECT_PTR pThis, D4CD_NUMBER_LIMITS* pLimits);
void D4CD_NumberPrintNum(D4CD_OBJECT* pThis, D4CD_CHAR* pBuff);
// Obsolete functions, replaced by any general
#define D4CD_NumberSetText D4CD_SetText




#endif /* __D4CD_NUMBER_H */        


