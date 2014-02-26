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
* @file      d4cd_label.h
*
* @author    Petr Gargulak
* 
* @version   0.0.3.0
* 
* @date      Jan-18-2011
* 
* @brief     D4CD Driver label object header file
*
*******************************************************************************/

#ifndef __D4CD_LABEL_H
#define __D4CD_LABEL_H

extern const D4CD_OBJECT_SYS_FUNCTION d4cd_labelSysFunc;

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/




  

#ifndef D4CD_LBL_F_DEFAULT
  #define D4CD_LBL_F_DEFAULT  (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED)
#endif  

#ifndef D4CD_LBL_TXT_PRTY_DEFAULT
  #define D4CD_LBL_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)
#endif


// label configuration (goes to ROM by default)

typedef struct
{
    D4CD_STRING* textBuff;    // label text
    D4CD_CHAR* preText;          // text printed before the list box
    D4CD_CHAR* postText;         // text printed behind the list box
    D4CD_POINT scrPos;        // position on the screen 
    D4CD_SIZE  scrSize;       // size on the screen (focus rect only, bitmaps have own size)    
} D4CD_LABEL;


/******************************************************************************
* Macros 
******************************************************************************/

// getting the LABEL structure from general OBJECT
#define D4CD_GET_LABEL(pObj) ((D4CD_LABEL*)((pObj)->pParam))

/*********************************************************
*
* static declaration macros
*
*********************************************************/

#define D4CD_DECLARE_LABEL(name, preText, text, postText, x, y, cx, flags, pUser, pOnUsrMsg) \
    static D4CD_TEXT_PROPERTIES name##_textPrties = { D4CD_LBL_TXT_PRTY_DEFAULT << D4CD_BITFIELD_SHIFT}; \
    static D4CD_STRING name##_txtbuff = { text, sizeof(text), &name##_textPrties}; \
    static const D4CD_LABEL name##_params = \
    { \
        &name##_txtbuff, /* textBuff */ \
        preText,   /* perText */\
        postText,   /* postText */\
        { x, y },   /* scrPos */ \
        { cx, 0 }, /* scrSize */ \
    }; \
    \
    D4CD_DECLARE_OBJECT(name, pOnUsrMsg, &d4cd_labelSysFunc, &(name##_params), flags, pUser)
 
#define D4CD_DECLARE_STD_LABEL(name, preText, text, postText, x, y, cx) \
    D4CD_DECLARE_LABEL(name, preText, text, postText, x, y, cx, (D4CD_LBL_F_DEFAULT), NULL, NULL)

#define D4CD_DECLARE_LABEL_AUTOSIZE(name, preText, text, postText, x, y, flags, pUser, pOnUsrMsg) \
    D4CD_DECLARE_LABEL(name, preText, text, postText, x, y, 0, flags, pUser, pOnUsrMsg)
    
#define D4CD_DECLARE_STD_LABEL_AUTOSIZE(name, preText, text, postText, x, y) \
    D4CD_DECLARE_STD_LABEL(name, preText, text, postText, x, y, 0)

/******************************************************************************
* Global variables
******************************************************************************/
   

/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* Object API
*
*********************************************************/



// Obsolete functions, replaced by any general
#define D4CD_LabelSetText D4CD_SetText
void D4CD_LabelSetTextBuffer(D4CD_OBJECT* pThis, D4CD_STRING* pTxtBuff);
D4CD_STRING* D4CD_LabelGetTextBuffer(D4CD_OBJECT* pThis);



#endif //__D4CD_LABEL_H