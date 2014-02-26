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
* @file      d4cd_menu.h
*
* @author    Petr Gargulak
* 
* @version   0.0.3.0
* 
* @date      Feb-14-2011
* 
* @brief     D4CD Driver menu object header file
*
*******************************************************************************/

#ifndef __D4CD_MENU_H
#define __D4CD_MENU_H

extern const D4CD_OBJECT_SYS_FUNCTION d4cd_menuSysFunc;

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/

#define D4CD_MENU_F_INDEX  0x0100

#ifndef D4CD_MENU_F_DEFAULT
  #define D4CD_MENU_F_DEFAULT  (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED | D4CD_OBJECT_F_TABSTOP | D4CD_MENU_F_INDEX )
#endif

#ifndef D4CD_MENU_TXT_PRTY_DEFAULT
  #define D4CD_MENU_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)
#endif


#ifndef D4CD_MENU_IX_TXT_PRTY_DEFAULT
  #define D4CD_MENU_IX_TXT_PRTY_DEFAULT  ( D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)
#endif

  
#ifndef D4CD_MENU_ITEM_TXT_PRTY_DEFAULT
  #define D4CD_MENU_ITEM_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_LEFT_MASK)
#endif
    
#ifndef D4CD_MENU_ENDMENULINE_CHAR
  #define D4CD_MENU_ENDMENULINE_CHAR '-'
#endif

#ifndef D4CD_MENU_SIDEBAR_NORM_CHAR
  #define D4CD_MENU_SIDEBAR_NORM_CHAR '|'
#endif

#ifndef D4CD_MENU_SIDEBAR_FOCUS_CHAR
  #define D4CD_MENU_SIDEBAR_FOCUS_CHAR '<'
#endif


typedef sByte D4CD_MENU_INDEX;

// Menu configuration (goes to ROM by default)
typedef struct 
{
  D4CD_CHAR* pText;
  void* pUser;
}D4CD_MENU_ITEM;

typedef struct 
{
   D4CD_MENU_INDEX ix;
   D4CD_MENU_INDEX focus_pos;
   D4CD_MENU_INDEX item_select; 
} D4CD_MENU_DATA;

typedef struct
{
    D4CD_STRING title_text;    // menu title text
    D4CD_POINT scrPos;        // position on the screen 
    D4CD_SIZE  scrSize;       // size on the screen (focus rect only, bitmaps have own size)
    D4CD_MENU_DATA* pData;     // Menu volatile data struct
    const D4CD_MENU_ITEM*  pItems;
    const D4CD_MENU_INDEX* pItemsCnt;
    
    void (*OnClicked)(D4CD_OBJECT* pThis, D4CD_MENU_INDEX ix);
        
} D4CD_MENU;


/******************************************************************************
* Macros 
******************************************************************************/

#define MENU_BUTT_CNT(sizeY, font) (Byte)(((sizeY) - 16) / (D4CD_FONT_H(font) + 3))
#define MENU_BUTT_OFF(sizeY, buttCnt) (D4CD_COOR)((((sizeY) - 16) / (buttCnt))-1 )


// getting the MENU structure from general OBJECT
#define D4CD_GET_MENU(pObj) ((D4CD_MENU*)((pObj)->pParam))

/*********************************************************
*
* static declaration macros
*
*********************************************************/

    
#define D4CD_DECLARE_MENU_BEGIN(name, title_text, x, y, cx, cy, flags, pUser, pOnClick, pOnUsrMsg) \
    extern const D4CD_MENU_ITEM name##_items[];\
    extern const D4CD_MENU_INDEX name##_itemsCnt;\
    static D4CD_MENU_DATA name##_data = { 0, 0, -1};\
    static D4CD_TEXT_PROPERTIES name##_textPrties = {(D4CD_BIT_FIELD)(D4CD_MENU_TXT_PRTY_DEFAULT << D4CD_BITFIELD_SHIFT)}; \
    static const D4CD_MENU name##_params = \
    { \
        { title_text, sizeof(title_text), &name##_textPrties},  \
        { x, y },    \
        { cx, cy },  \
		    &(name##_data), \
		    &name##_items[0],  /* MENU ITEMS     */ \
		    &name##_itemsCnt, /* all items cnt */ \
		    pOnClick, \
    }; \
    \
    D4CD_DECLARE_OBJECT(name, pOnUsrMsg, &d4cd_menuSysFunc, &(name##_params), flags, pUser)\
    \
    const D4CD_MENU_ITEM name##_items[] = { \


    
//#define D4CD_DECLARE_MENU_ITEM(text) { {(text), sizeof(text), NULL}, (void*)NULL},

//#define D4CD_DECLARE_MENU_ITEM_FULL(text, pUser) { {(text), sizeof(text), NULL}, (void*)pUser},

#define D4CD_DECLARE_MENU_ITEM(text) { (text), (void*)NULL},

#define D4CD_DECLARE_MENU_ITEM_FULL(text, pUser) {(text), (void*)pUser},


//#define D4CD_DECLARE_MENU_END(name) { {NULL, 0, NULL}, (void*)NULL }}; \
//    const D4CD_MENU_INDEX name##_itemsCnt = ((const D4CD_MENU_INDEX)(sizeof(name##_items) / sizeof(D4CD_MENU_ITEM)) - 1);

#define D4CD_DECLARE_MENU_END(name) { NULL, (void*)NULL }}; \
    const D4CD_MENU_INDEX name##_itemsCnt = ((const D4CD_MENU_INDEX)(sizeof(name##_items) / sizeof(D4CD_MENU_ITEM)) - 1);

#define D4CD_DECLARE_STD_MENU_BEGIN(name, title_text, title_font, x, y, cx, cy, IndexFontId, ItemsFontId, posCnt, MenuItemsOff, pIcon, pOnClick) \
    D4CD_DECLARE_MENU_BEGIN(name, title_text, title_font, x, y, cx, cy, (D4CD_MENU_F_DEFAULT), NULL,\
    IndexFontId, ItemsFontId, posCnt, MenuItemsOff, pIcon, NULL, pOnClick, NULL)

#define D4CD_DECLARE_STD_MENU_AUTOSIZE_BEGIN(name, title_text, title_font, x, y, cx, cy, IndexFontId, ItemsFontId, pIcon, pOnClick) \
    D4CD_DECLARE_STD_MENU_BEGIN(name, title_text, title_font, x, y, cx, cy, IndexFontId, ItemsFontId, 0, 0, pIcon, pOnClick)

/******************************************************************************
* Global functions
******************************************************************************/
D4CD_MENU_INDEX D4CD_MenuGetIndex(D4CD_OBJECT* pThis);
void  D4CD_MenuSetIndex(D4CD_OBJECT* pThis, D4CD_MENU_INDEX ix);

D4CD_MENU_INDEX D4CD_MenuGetItemCount(D4CD_OBJECT_PTR pThis);
D4CD_MENU_INDEX D4CD_MenuFindUserDataItem(D4CD_OBJECT_PTR pThis, void* pUser);
void* D4CD_MenuGetItemUserData(D4CD_OBJECT_PTR pThis);
D4CD_CHAR* D4CD_MenuGetItemText(D4CD_OBJECT_PTR pThis);

#endif __D4CD_MENU_H 
 



