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
* @file      d4cd_list_box.h
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Jan-12-2011
* 
* @brief     D4CD Driver listBox object header file
*
*******************************************************************************/

#ifndef __D4CD_LIST_BOX_H
#define __D4CD_LIST_BOX_H

extern const D4CD_OBJECT_SYS_FUNCTION d4cd_listBoxSysFunc;

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/

#ifndef D4CD_LIST_BOX_F_DEFAULT
  #define D4CD_LIST_BOX_F_DEFAULT  (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED | D4CD_OBJECT_F_TABSTOP | D4CD_OBJECT_F_CURSOR_EN)
#endif

#ifndef D4CD_LIST_BOX_TXT_PRTY_DEFAULT
  #define D4CD_LIST_BOX_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)
#endif

#ifndef D4CD_LISTBOX_ITEM_TXT_PRTY_DEFAULT
  #define D4CD_LISTBOX_ITEM_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)
#endif

#ifndef D4CD_LISTBOX_SEL_FOCUS_CHAR
  #define D4CD_LISTBOX_SEL_FOCUS_CHAR '>'
#endif

#ifndef D4CD_LISTBOX_SEL_NORMAL_CHAR
  #define D4CD_LISTBOX_SEL_NORMAL_CHAR ' '
#endif

#ifndef D4CD_LISTBOX_KEY_SCANCODE_PLUS
  #define D4CD_LISTBOX_KEY_SCANCODE_PLUS D4CD_KEY_SCANCODE_UP
#endif

#ifndef D4CD_LISTBOX_KEY_SCANCODE_MINUS
  #define D4CD_LISTBOX_KEY_SCANCODE_MINUS D4CD_KEY_SCANCODE_DOWN
#endif

#if (D4CD_LISTBOX_SEL_FOCUS_CHAR != 0) && (D4CD_LISTBOX_SEL_NORMAL_CHAR != 0)
  #define  D4CD_LISTBOX_SEL_CHAR D4CD_TRUE
#else
  #define  D4CD_LISTBOX_SEL_CHAR D4CD_FALSE
#endif

 
typedef Byte D4CD_LIST_BOX_INDEX;

typedef struct
{
  D4CD_CHAR* pText;
  void* pUser;
}D4CD_LIST_BOX_ITEMS;

typedef union
{
  D4CD_BIT_FIELD all;
  struct
  {
    unsigned bSet :1;
  }bits;
  
}D4CD_LISTBOX_STATUS;

typedef struct
{
  D4CD_LIST_BOX_INDEX index;      // current value
  D4CD_LISTBOX_STATUS status;
} D4CD_LIST_BOX_DATA;

// LIST_BOX configuration (in ROM by default)

typedef struct
{
    D4CD_CHAR* preText;          // text printed before the list box
    D4CD_CHAR* postText;         // text printed behind the list box
    D4CD_POINT scrPos;           // position on the screen
    D4CD_SIZE  scrSize;          // size on the screen for focus rectangle (bitmap has its own size)
    D4CD_LIST_BOX_ITEMS* pItems;           // LIST_BOX state X idication bitmaps
    void (*OnValueChanged)(D4CD_OBJECT* pThis, void* pUser);
    D4CD_LIST_BOX_DATA* pData;    
} D4CD_LIST_BOX;

/******************************************************************************
* Macros 
******************************************************************************/

// getting the LIST_BOX structure from general OBJECT
#define D4CD_GET_LIST_BOX(pObj) ((D4CD_LIST_BOX*)((pObj)->pParam))

/*********************************************************
*
* static declaration macros
*
*********************************************************/
          
#define _D4CD_DECLARE_LIST_BOX_BEGIN(type, name, preText, postText, x, y, cx, flags, pUser, pOnValch, pOnUsrMsg) \
    extern type D4CD_LIST_BOX_ITEMS name##_items[];\
    static D4CD_LIST_BOX_DATA name##_data; \
    static type D4CD_LIST_BOX name##_params = \
    { \
        preText,   /* perText */\
        postText,   /* postText */\
        { x, y },     /* scrPos     */ \
        { cx, 0 },   /* scrSize    */ \
        (D4CD_LIST_BOX_ITEMS*)&(name##_items[0]), /* pItems     */ \
        pOnValch,      /* OnValueCanged */ \
        &(name##_data) /* RAM-based data */ \
    }; \
    \
    D4CD_DECLARE_OBJECT(name, pOnUsrMsg, &d4cd_listBoxSysFunc, &(name##_params), (flags), pUser)\
    \
    type D4CD_LIST_BOX_ITEMS   name##_items[] = \
    {

#define D4CD_DECLARE_LIST_BOX_ITEM(pItemText, pItemUserPointer) { (pItemText), (void*)(pItemUserPointer)},

#define D4CD_DECLARE_LIST_BOX_END { NULL, NULL }};


#define D4CD_DECLARE_LIST_BOX_BEGIN(name, preText, postText, x, y, cx, flags, pUser, pOnValch, pOnUsrMsg) \
  _D4CD_DECLARE_LIST_BOX_BEGIN(D4CD_CONST, name, preText, postText, x, y, cx, flags, pUser, pOnValch, pOnUsrMsg)


#define D4CD_DECLARE_LIST_BOX_BEGIN_INRAM(name, preText, postText, x, y, cx, flags, pUser, pOnValch, pOnUsrMsg) \
  _D4CD_DECLARE_LIST_BOX_BEGIN(D4CD_NO_CONST, name, preText, postText, x, y, cx, flags, pUser, pOnValch, pOnUsrMsg)


#define D4CD_DECLARE_LIST_BOX_BEGIN_AUTOSIZE(name, preText, postText, x, y, flags, pUser, onvalch, pOnUsrMsg) \
  D4CD_DECLARE_LIST_BOX_BEGIN(name, preText, postText, x, y, 0, (flags), pUser, onvalch, pOnUsrMsg)

#define D4CD_DECLARE_STD_LIST_BOX_BEGIN(name, preText, postText, x, y, cx, onvalch) \
  D4CD_DECLARE_LIST_BOX_BEGIN(name, preText, postText, x, y, cx, (D4CD_LIST_BOX_F_DEFAULT), NULL, onvalch, NULL)

#define D4CD_DECLARE_STD_LIST_BOX_BEGIN_AUTOSIZE(name, preText, postText, x, y, onvalch)\
  D4CD_DECLARE_STD_LIST_BOX_BEGIN(name, preText, postText, x, y, 0, onvalch)



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
void D4CD_ListBoxSetIndex(D4CD_OBJECT_PTR pThis, D4CD_LIST_BOX_INDEX index);
void D4CD_ListBoxChangeIndex(D4CD_OBJECT_PTR pThis, sByte incr);
D4CD_LIST_BOX_INDEX D4CD_ListBoxGetIndex(D4CD_OBJECT_PTR pThis);


D4CD_LIST_BOX_INDEX D4CD_ListBoxGetItemCount(D4CD_OBJECT_PTR pThis);
D4CD_LIST_BOX_INDEX D4CD_ListBoxFindUserDataItem(D4CD_OBJECT_PTR pThis, void* pUser);
void* D4CD_ListBoxGetItemUserData(D4CD_OBJECT_PTR pThis);
D4CD_CHAR* D4CD_ListBoxGetItemText(D4CD_OBJECT_PTR pThis);
// Obsolete functions, replaced by any general
#define D4CD_ListBoxSetText D4CD_SetText




#endif /* __D4CD_LIST_BOX_H */        


