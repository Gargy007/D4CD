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
* @file      d4cd_object.h
*
* @author    Petr Gargulak
* 
* @version   0.0.1.0
* 
* @date      Jan-6-2011
* 
* @brief     D4CD Driver object functions header file
*
*******************************************************************************/

#ifndef __D4CD_OBJECT_H
#define __D4CD_OBJECT_H

/******************************************************************************
* Includes
******************************************************************************/


typedef union 
{
    D4CD_BIT_FIELD all;
    
    struct
    {
        unsigned bVisible     :1;     // visible at all 
        unsigned bEnabled     :1;     // Enabled / Disabled option 
        unsigned bTabStop     :1;     // can be focused 
        unsigned bCursorEna   :1;     // Object is enabled for touchscreen
        unsigned bCursorMod   :1;     // Only one touch to action
        unsigned bRedraw      :1;     // needs redraw
        unsigned bRedrawC     :1;     // use complete redraw
        unsigned bInitDone    :1;     // object initialization done         
    } bits;
    
    struct
    {
        unsigned bPossFocus   :3;  
    } mergedBits;
} D4CD_OBJECT_FLAGS;

typedef Word D4CD_OBJECT_INITFLAGS;

#if D4CD_BITFIELD_LSB_ALIGNMENT == D4CD_BITFIELD_LSB_ALLIGMENT_RIGHT
  #define D4CD_OBJECT_F_VISIBLE       0x01
  #define D4CD_OBJECT_F_ENABLED       0x02
  #define D4CD_OBJECT_F_TABSTOP       0x04
  #define D4CD_OBJECT_F_CURSOR_EN     0x08
  #define D4CD_OBJECT_F_CURSOR_MOD     0x10  
#else
  #define D4CD_OBJECT_F_VISIBLE       0x80
  #define D4CD_OBJECT_F_ENABLED       0x40
  #define D4CD_OBJECT_F_TABSTOP       0x20
  #define D4CD_OBJECT_F_CURSOR_EN     0x10
  #define D4CD_OBJECT_F_CURSOR_MOD     0x08  
#endif

  #define D4CD_OBJECT_F_SYSTEM        (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED | D4CD_OBJECT_F_TABSTOP | D4CD_OBJECT_F_CURSOR_EN | D4CD_OBJECT_F_CURSOR_MOD)
  #define D4CD_OBJECT_F_INIT          0xFF00 


typedef struct D4CD_OBJECT_SYS_FUNCTION_S
{
  D4CD_OBJECT_TYPE type;
  void (*OnSysMessage)(struct D4CD_MESSAGE_S* pMsg);
  struct D4CD_STRING_S* (*GetTextBuffer)(struct D4CD_OBJECT_S* pObj);            
}D4CD_OBJECT_SYS_FUNCTION;

struct D4CD_SCREEN_S;
struct D4CD_MESSAGE_S;


// this is allocated in ROM always
typedef struct D4CD_OBJECT_S
{
    const void* pParam;
    D4CD_OBJECT_SYS_FUNCTION* pObjFunc; 
    Byte (*OnUsrMessage)(struct D4CD_MESSAGE_S* pMsg);    
    void *userPointer;
    D4CD_OBJECT_INITFLAGS initFlags;
    D4CD_OBJECT_FLAGS*  flags;
    struct D4CD_SCREEN_S** pScreen;
} D4CD_OBJECT;

typedef const D4CD_OBJECT * D4CD_OBJECT_PTR;

/******************************************************************************
* Macros
******************************************************************************/
    
#define D4CD_EXTERN_OBJECT(name) \
    extern const D4CD_OBJECT name

/*********************************************************
*
* static declaration macros
*
*********************************************************/
#define D4CD_DECLARE_OBJECT(name, onusrmsg, sysFunc , param, initFlags, usrPointer) \
    D4CD_OBJECT_FLAGS name##_flags = { (D4CD_BIT_FIELD)(((D4CD_BIT_FIELD)(initFlags & D4CD_OBJECT_F_SYSTEM)) << D4CD_BITFIELD_SHIFT) }; \
    D4CD_SCREEN *name##_pScreen; \
    const D4CD_OBJECT name = \
    { \
        param, \
        (D4CD_OBJECT_SYS_FUNCTION*)sysFunc, \
        onusrmsg, \
        usrPointer, /* User Pointer */ \
        initFlags, \
        &(name##_flags), \
        &(name##_pScreen) \
    };    
    
#endif // __D4CD_OBJECT_H