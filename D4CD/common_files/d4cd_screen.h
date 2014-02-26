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
* @file      d4cd_screen.h
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Mar-11-2011
* 
* @brief     D4CD driver screen header file 
*
******************************************************************************/

#ifndef __D4CD_SCREEN_H
#define __D4CD_SCREEN_H

/******************************************************************************
* Constants
******************************************************************************/










#ifndef D4CD_SCR_OUTLINE_CORNER_CHAR
  #define D4CD_SCR_OUTLINE_CORNER_CHAR  '+'
#endif

#ifndef D4CD_SCR_OUTLINE_ROW_CHAR
  #define D4CD_SCR_OUTLINE_ROW_CHAR  '-'
#endif

#ifndef D4CD_SCR_OUTLINE_COLUMN_CHAR
  #define D4CD_SCR_OUTLINE_COLUMN_CHAR  '|'
#endif


#ifndef D4CD_SCR_BCKG_CHAR
  #define D4CD_SCR_BCKG_CHAR  ' '
#endif



typedef struct
{
    // other runtime data
    Byte focusedObjIx;          // index of currently focused object
    
    union 
    {
        D4CD_BIT_FIELD all;
    
        struct
        {
            unsigned bInitDone:1;       // screen initialization done
            unsigned bReserved0:1;    // Object is enabled for touchscreen
            unsigned bCompleteRedraw:1; // complete redraw pending
        } bits;                
    } flags;
} D4CD_SCREEN_DATA;

// screen configuration (goes to ROM by default)
#if D4CD_BITFIELD_LSB_ALIGNMENT == D4CD_BITFIELD_LSB_ALLIGMENT_RIGHT
  #define D4CD_SCR_F_OUTLINE       (0x01)
  #define D4CD_SCR_F_TITLEBAR      (0x02)
  #define D4CD_SCR_F_BCKG          (0x04)
#else
  #define D4CD_SCR_F_OUTLINE       (0x80)
  #define D4CD_SCR_F_TITLEBAR      (0x40)
  #define D4CD_SCR_F_BCKG          (0x20)
#endif  

#ifndef D4CD_SCR_F_DEFAULT
  #define D4CD_SCR_F_DEFAULT        (0)
#endif 

typedef union 
{
    D4CD_BIT_FIELD all;
    
    struct
    {
        unsigned bOutline     :1;     // show outline rectagle 
        unsigned bHeader      :1;     // Header box bit 
        unsigned bBackground  :1;     // draw window bakground 
        unsigned bReserved3   :1;     // Reserved bit                 
        unsigned bReserve4    :1;     // Reserved bit
        unsigned bReserve5    :1;     // Reserved bit
        unsigned bReserve6    :1;     // Reserved bit
        unsigned bReserve7    :1;     // Reserved bit
    } bits;
} D4CD_SCREEN_FLAGS;

struct D4CD_STRING_S;

typedef struct D4CD_SCREEN_S
{ 
    // object list
    const D4CD_OBJECT* const * pObjects; // NULL-terminated array of objects (may lay in ROM)
    
    // event handlers    
    void (*OnInit)(void);       // one-time initialization
    void (*OnMain)(void);       // main screen handler function
    void (*OnActivate)(void);   // called before screen activation
    void (*OnDeactivate)(void); // called before deactivating
    Byte (*OnObjectMsg)(struct D4CD_MESSAGE_S* pMsg); // called before object receives the message
    
    // Screen advanced properties
    D4CD_POINT position;
    D4CD_SIZE size;
    struct D4CD_STRING_S textBuff;
    D4CD_SCREEN_FLAGS flags;     
    // pointer to runtime data
    D4CD_SCREEN_DATA* pData;
    D4CD_CHAR_DATA** pUserTable;
} D4CD_SCREEN;


/*********************************************************
*
* static declaration macros
*
*********************************************************/


#define D4CD_DECLARE_SCREEN_BEGIN(name, funcPrefix, x ,y, cx, cy, text, userCharTable, flags) \
    extern const D4CD_OBJECT * const name##_objects[]; \
    D4CD_EXTERN_CHAR_TABLE(userCharTable) \
    static void funcPrefix##OnInit(); \
    static void funcPrefix##OnMain(); \
    static void funcPrefix##OnActivate(); \
    static void funcPrefix##OnDeactivate(); \
    static Byte funcPrefix##OnObjectMsg(D4CD_MESSAGE* pMsg); \
    static D4CD_SCREEN_DATA name##_data = { 0,  (((((D4CD_BIT_FIELD)flags) >> 8) & 0x00FF) << D4CD_BITFIELD_SHIFT)}; \
    static D4CD_TEXT_PROPERTIES name##_textPrties = { (D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK | D4CD_TXT_PRTY_ALIGN_V_CENTER_MASK)  << D4CD_BITFIELD_SHIFT}; \
    const  D4CD_SCREEN name = \
    { \
        name##_objects, \
        funcPrefix##OnInit, \
        funcPrefix##OnMain, \
        funcPrefix##OnActivate, \
        funcPrefix##OnDeactivate, \
        funcPrefix##OnObjectMsg, \
        {x, y}, \
        {cx, cy}, \
        { text, sizeof(text),  &name##_textPrties}, /* textBuff */ \
        ((D4CD_BIT_FIELD)(flags & 0x00FF) << D4CD_BITFIELD_SHIFT), \
        &(name##_data), \
        (D4CD_CHAR_DATA**)userCharTable \
    }; \
    const D4CD_OBJECT * const name##_objects[] = \
    {
     
     
#define D4CD_DECLARE_STD_SCREEN_BEGIN(name, funcPrefix) D4CD_DECLARE_SCREEN_BEGIN(name, funcPrefix, 0 ,0, \
          (D4CD_COOR)(D4CD_SCREEN_SIZE_X), (D4CD_COOR)(D4CD_SCREEN_SIZE_Y), NULL, D4CD_USER_CHAR_TABLE_DEFAULT, D4CD_SCR_F_DEFAULT)   
     
#define D4CD_DECLARE_SCREEN_OBJECT(name) &(name),

#define D4CD_DECLARE_SCREEN_END() NULL };

#define D4CD_EXTERN_SCREEN(name) \
    extern D4CD_SCREEN name
               

/**************************************************************//*!
*
* Main driver API Functions
*
******************************************************************/


D4CD_SCREEN* D4CD_GetActiveScreen(void);
void D4CD_ActivateScreen(D4CD_SCREEN* pNewScreen, Byte bReplaceCurrent);
void D4CD_EscapeScreen(void);
void D4CD_EscapeToBaseScreen(void);
void D4CD_ClearScreenHistory(void);
void D4CD_InvalidateScreen(D4CD_SCREEN* pScreen, Byte bComplete);
void D4CD_ResetObjectFocus(D4CD_SCREEN* pScreen);


D4CD_POINT D4CD_GetClientToScreenPoint(D4CD_SCREEN* pScreen, D4CD_POINT* nClientPoint);
D4CD_POINT D4CD_GetScreenToClientPoint(D4CD_SCREEN* pScreen, D4CD_POINT* nScreenPoint);


    
#endif /* __D4CD_SCREEN_H */   