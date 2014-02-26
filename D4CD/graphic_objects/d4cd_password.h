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
****************************************************************************//*!
*
* @file      d4cd_password.h
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Jan-18-2011
* 
* @brief     D4CD Driver pswrd object header file
*
*******************************************************************************/

#ifndef __D4CD_PSWRD_H
#define __D4CD_PSWRD_H

extern const D4CD_OBJECT_SYS_FUNCTION d4cd_pswrdSysFunc;

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/

#ifndef D4CD_PSWRD_F_DEFAULT
  #define D4CD_PSWRD_F_DEFAULT  (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED | D4CD_OBJECT_F_TABSTOP | D4CD_OBJECT_F_CURSOR_EN)
#endif

#ifndef D4CD_PSWRD_TXT_PRTY_DEFAULT
  #define D4CD_PSWRD_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)
#endif

#ifndef D4CD_PSWRD_SEL_FOCUS_CHAR
  #define D4CD_PSWRD_SEL_FOCUS_CHAR '>'
#endif

#ifndef D4CD_PSWRD_SEL_NORMAL_CHAR
  #define D4CD_PSWRD_SEL_NORMAL_CHAR ' '
#endif

#ifndef D4CD_PSWRD_HIDEPSWRD_CHAR
  #define D4CD_PSWRD_HIDEPSWRD_CHAR '*'
#endif

#ifndef D4CD_PSWRD_KEY_SCANCODE_PLUS
  #define D4CD_PSWRD_KEY_SCANCODE_PLUS D4CD_KEY_SCANCODE_UP
#endif

#ifndef D4CD_PSWRD_KEY_SCANCODE_MINUS
  #define D4CD_PSWRD_KEY_SCANCODE_MINUS D4CD_KEY_SCANCODE_DOWN
#endif


#if (D4CD_PSWRD_SEL_FOCUS_CHAR != 0) && (D4CD_PSWRD_SEL_NORMAL_CHAR != 0)
  #define  D4CD_PSWRD_SEL_CHAR D4CD_TRUE
#else
  #define  D4CD_PSWRD_SEL_CHAR D4CD_FALSE
#endif

 
typedef Byte D4CD_PSWRD_VALUE;

typedef union
{
  D4CD_BIT_FIELD all;
  struct
  {
    unsigned bSet :4;    
    unsigned bNumEnable :1;
    unsigned bCharsEnable :1;
    unsigned bSpecialEnable :1;
  }bits;
  
}D4CD_PSWRD_STATUS;

typedef struct
{
  D4CD_PSWRD_VALUE* value;      // current value
  D4CD_PSWRD_STATUS status;
} D4CD_PSWRD_DATA;

// PSWRD configuration (in ROM by default)

typedef struct
{
    D4CD_CHAR* preText;          // text printed before the list box
    D4CD_CHAR* postText;         // text printed behind the list box
    D4CD_POINT scrPos;           // position on the screen
    D4CD_COOR  pswrdSize;          // size on the screen for focus rectangle (bitmap has its own size)
    D4CD_PSWRD_VALUE* pPswrd;           // PSWRD value
    void (*OnPswrdEngter)(D4CD_OBJECT* pThis, D4CD_BOOL valid);
    D4CD_PSWRD_DATA* pData;    
}D4CD_PSWRD;

/******************************************************************************
* Macros 
******************************************************************************/

// getting the PSWRD structure from general OBJECT
#define D4CD_GET_PSWRD(pObj) ((D4CD_PSWRD*)((pObj)->pParam))

/*********************************************************
*
* static declaration macros
*
*********************************************************/
          
#define _D4CD_DECLARE_PSWRD(type, name, preText, postText, x, y, pswdLen, flags, pUser, pOnEnter, pOnUsrMsg) \
    static D4CD_PSWRD_VALUE name##_pswrdVal[pswdLen+1]; \
    static D4CD_PSWRD_VALUE name##_pswrd[pswdLen+1];  \
    static D4CD_PSWRD_DATA name##_data = {name##_pswrdVal, 0xe0}; \
    static type D4CD_PSWRD name##_params = \
    { \
        preText,   /* perText */\
        postText,   /* postText */\
        { x, y },     /* scrPos     */ \
        pswdLen,   /* pswLen    */ \
        name##_pswrd, /* password */ \
        pOnEnter,      /* OnValueCanged */ \
        &(name##_data) /* RAM-based data */ \
    }; \
    \
    D4CD_DECLARE_OBJECT(name, pOnUsrMsg, &d4cd_pswrdSysFunc, &(name##_params), (flags), pUser)

#define D4CD_DECLARE_PSWRD(name, preText, postText, x, y, pswdLen, flags, pUser, pOnEnter, pOnUsrMsg) \
  _D4CD_DECLARE_PSWRD(D4CD_CONST, name, preText, postText, x, y, pswdLen, flags, pUser, pOnEnter, pOnUsrMsg)

#define D4CD_DECLARE_PSWRD_INRAM(name, preText, postText, x, y, pswdLen, flags, pUser, pOnEnter, pOnUsrMsg) \
  _D4CD_DECLARE_PSWRD(D4CD_NO_CONST, name, preText, postText, x, y, pswdLen, flags, pUser, pOnEnter, pOnUsrMsg)

#define D4CD_DECLARE_STD_PSWRD(name, preText, postText, x, y, pswdLen, pOnEnter) \
  D4CD_DECLARE_PSWRD(name, preText, postText, x, y, pswdLen, (D4CD_PSWRD_F_DEFAULT), NULL, pOnEnter, NULL)




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
void D4CD_PswrdSet(D4CD_OBJECT_PTR pThis, D4CD_PSWRD_VALUE* pValue);
D4CD_PSWRD_VALUE* D4CD_PswrdGetValue(D4CD_OBJECT_PTR pThis);
void D4CD_PswrdSetRange(D4CD_OBJECT_PTR pThis, D4CD_BOOL num, D4CD_BOOL chars, D4CD_BOOL special);
void D4CD_ShowDummyPswrd(D4CD_OBJECT_PTR pThis);
#endif /* __D4CD_PSWRD_H */        


