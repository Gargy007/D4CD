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
* @file      d4cd_button.h
*
* @author    Petr Gargulak
* 
* @version   0.0.3.0
* 
* @date      Jan-18-2011
* 
* @brief     D4CD Driver button object header file
*
*******************************************************************************/

#ifndef __D4CD_BUTTON_H
#define __D4CD_BUTTON_H

extern const D4CD_OBJECT_SYS_FUNCTION d4cd_btnSysFunc;

/******************************************************************************
* Constants
******************************************************************************/

#ifndef D4CD_BTN_BORDER_OFFSET
  #define D4CD_BTN_BORDER_OFFSET 3
#endif

/******************************************************************************
* Types
******************************************************************************/

// button configuration (goes to ROM by default)

#ifndef D4CD_BTN_F_DEFAULT
  #define D4CD_BTN_F_DEFAULT  (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED | D4CD_OBJECT_F_TABSTOP | D4CD_OBJECT_F_CURSOR_EN )
#endif

#ifndef D4CD_BTN_TXT_PRTY_DEFAULT
  #define D4CD_BTN_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)
#endif


#ifndef D4CD_BTN_SEL_NORM_CHAR_LEFT
  #define D4CD_BTN_SEL_NORM_CHAR_LEFT '('
#endif

#ifndef D4CD_BTN_SEL_NORM_CHAR_RIGHT
  #define D4CD_BTN_SEL_NORM_CHAR_RIGHT ')'
#endif

#ifndef D4CD_BTN_SEL_FOCUS_CHAR_LEFT
  #define D4CD_BTN_SEL_FOCUS_CHAR_LEFT '>'
#endif

#ifndef D4CD_BTN_SEL_FOCUS_CHAR_RIGHT
  #define D4CD_BTN_SEL_FOCUS_CHAR_RIGHT '<'
#endif


#if (D4CD_BTN_SEL_FOCUS_CHAR_LEFT != 0) && (D4CD_BTN_SEL_NORM_CHAR_LEFT != 0)
  #define  D4CD_BTN_SEL_CHAR_LEFT D4CD_TRUE
#else
  #define  D4CD_BTN_SEL_CHAR_LEFT D4CD_FALSE
#endif

#if (D4CD_BTN_SEL_FOCUS_CHAR_RIGHT != 0) && (D4CD_BTN_SEL_NORM_CHAR_RIGHT != 0)
  #define  D4CD_BTN_SEL_CHAR_RIGHT D4CD_TRUE
#else
  #define  D4CD_BTN_SEL_CHAR_RIGHT D4CD_FALSE
#endif
  

typedef union
{
  D4CD_BIT_FIELD all;
  struct
  {
    unsigned bPressed   :1;      
  }bits;
  
}D4CD_BUTTON_STATUS;


typedef struct 
{
    D4CD_STRING textBuff;    // button text
    D4CD_POINT scrPos;        // position on the screen 
    D4CD_SIZE  scrSize;       // size on the screen (focus rect only, bitmaps have own size)
    D4CD_BUTTON_STATUS*  pStatus;    
    void (*OnClicked)(D4CD_OBJECT* pThis);
} D4CD_BUTTON;


/******************************************************************************
* Macros 
******************************************************************************/

// getting the BUTTON structure from general OBJECT
#define D4CD_GET_BUTTON(pObj) ((D4CD_BUTTON*)((pObj)->pParam))
#define D4CD_GET_BUTTON_STATUS(pObj) ((D4CD_BUTTON_STATUS*)(D4CD_GET_BUTTON(pObj)->pStatus))


/*********************************************************
*
* static declaration macros
*
*********************************************************/

#define D4CD_DECLARE_BUTTON(name, text, x, y, cx, flags, pUser, onclick, pOnUsrMsg) \
    static D4CD_BUTTON_STATUS  name##_status = { 0 }; \
    static D4CD_TEXT_PROPERTIES name##_strPrties = { D4CD_BTN_TXT_PRTY_DEFAULT << D4CD_BITFIELD_SHIFT}; \
    static const D4CD_BUTTON name##_params = \
    { \
        { text, sizeof(text), &name##_strPrties}, /* textBuff */ \
        { x, y },   /* scrPos */ \
        { cx, 0 }, /* scrSize */ \
        &(name##_status), /* Status clear */ \
        onclick     /* Click event */ \
    }; \
    \
    D4CD_DECLARE_OBJECT(name, pOnUsrMsg,  &d4cd_btnSysFunc, &(name##_params), flags, pUser)

#define D4CD_DECLARE_STD_BUTTON(name, text, x, y, cx, onclick) \
    D4CD_DECLARE_BUTTON(name, text, x, y, cx, (D4CD_BTN_F_DEFAULT), NULL, onclick, NULL)

#define D4CD_DECLARE_STD_BUTTON_AUTOSIZE(name, text, x, y, onclick) \
    D4CD_DECLARE_STD_BUTTON(name, text, x, y, 0, onclick)

/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* Object API
*
*********************************************************/

// Obsolete functions, replaced by any general
#define  D4CD_BtnSetText D4CD_SetText

#endif /*__D4CD_BUTTON_H*/












