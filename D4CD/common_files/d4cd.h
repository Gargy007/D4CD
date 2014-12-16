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
* @file      d4cd.h
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Jan-12-2011
* 
* @brief     D4CD Driver main header file
*
*******************************************************************************/

#ifndef __D4CD_H
#define __D4CD_H

/******************************************************************************
* Includes
******************************************************************************/

/**************************************************************//*!
*
* Types used by driver (standard HCxx: Byte, Word, LWord, ...)
*
******************************************************************/


/**************************************************************//*!
*
* Implementation header files
*
******************************************************************/
#include "d4cd_user_cfg.h"
#include "d4cd_types.h"
#include "d4cd_base.h"
#include "d4cd_object.h"
#include "d4cd_low.h"
#include "d4cd_font.h"
#include "d4cd_math.h"
#include "d4cd_screen.h"
#include "d4cd_button.h"
#include "d4cd_check_box.h"
#include "d4cd_label.h"
#include "d4cd_list_box.h"
#include "d4cd_number.h"
#include "d4cd_menu.h"
#include "d4cd_time.h"
#include "d4cd_date.h"
#include "d4cd_password.h"


/******************************************************************************
* Constants
******************************************************************************/

/**************************************************************//*!
*
* Basic driver configuration
*
******************************************************************/ 


/******************************************************************************
* Types
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

/**************************************************************//*!
*
* Main driver API Functions
*
******************************************************************/

/* General driver API */
D4CD_BOOL D4CD_Init(D4CD_SCREEN* pInitScreen);
void D4CD_Poll(void);
void D4CD_KeysChanged(D4CD_KEYS keys);
void D4CD_NewKeyEvent(D4CD_KEY_SCANCODE scanCode);
void D4CD_TimeTickPut(void);
D4CD_BOOL D4CD_GetBlink(void);

/* screen API */


/* object API */
void D4CD_EnableTabStop(D4CD_OBJECT_PTR pObj, Byte bEnable);
void D4CD_EnableObject(D4CD_OBJECT_PTR pObj, Byte bEnable);
void D4CD_ShowObject(D4CD_OBJECT_PTR pObject, D4CD_BOOL bShow);    
D4CD_OBJECT_PTR D4CD_GetFocusedObject(D4CD_SCREEN* pScreen);
void D4CD_FocusSet(D4CD_SCREEN* pScreen, D4CD_OBJECT_PTR pObject);
void D4CD_FocusNextObject(D4CD_SCREEN* pScreen, Byte bInitialSearch);
void D4CD_FocusPrevObject(D4CD_SCREEN* pScreen);
void D4CD_InvalidateObject(D4CD_OBJECT_PTR pObject, Byte bComplete);
D4CD_BOOL D4CD_IsShowedObject(D4CD_OBJECT_PTR pObject);
void* D4CD_GetUserPointer(D4CD_OBJECT *pThis);
void D4CD_CaptureKeys(D4CD_OBJECT_PTR pObj);
D4CD_OBJECT* D4CD_GetCapturedObject(void);

                             
/* general helpers */
D4CD_KEYS D4CD_GetKeys(void);
void D4CD_ClearKeysBuffer(void);
void D4CD_ClearScreen(D4CD_CHAR ch);
void D4CD_MoveTo(D4CD_POINT* ppt);
void D4CD_MoveToXY(D4CD_COOR x, D4CD_COOR y);
void D4CD_SetCursor(D4CD_POINT* ppt, D4CD_BOOL visible, D4CD_BOOL blink);

void D4CD_PutChar(D4CD_POINT *ppt, D4CD_CHAR ch);
void D4CD_PutCharXY(D4CD_COOR x, D4CD_COOR y, D4CD_CHAR ch);
void D4CD_LineTo(D4CD_POINT* ppt, D4CD_CHAR ch);
void D4CD_LineToXY(D4CD_COOR x, D4CD_COOR y, D4CD_CHAR ch);        
void D4CD_FillRect(D4CD_POINT* ppt, D4CD_SIZE* psz, D4CD_CHAR ch);
void D4CD_FillRectXY(D4CD_COOR x1, D4CD_COOR y1, D4CD_COOR x2, D4CD_COOR y2, D4CD_CHAR ch);
void D4CD_FillRectTo(D4CD_POINT* ppt, D4CD_CHAR ch);
void D4CD_FillRectToXY(D4CD_COOR x, D4CD_COOR y, D4CD_CHAR ch);    
void D4CD_Rect(D4CD_POINT* ppt, D4CD_SIZE* psz, D4CD_CHAR ch);
void D4CD_RectXY(D4CD_COOR x1, D4CD_COOR y1, D4CD_COOR x2, D4CD_COOR y2, D4CD_CHAR ch);
void D4CD_RectTo(D4CD_POINT* ppt, D4CD_CHAR ch);
void D4CD_RectToXY(D4CD_COOR x, D4CD_COOR y, D4CD_CHAR ch);

void D4CD_DrawSimpleText(D4CD_POINT* ppt, D4CD_CHAR* pText);
void D4CD_DrawSimpleTextXY(D4CD_COOR x, D4CD_COOR y, D4CD_CHAR* pText);

void D4CD_DrawText(D4CD_POINT* ppt, D4CD_STRING* buffText);
void D4CD_DrawTextXY(D4CD_COOR x, D4CD_COOR y, D4CD_STRING* buffText);

void D4CD_DrawTextRect(D4CD_POINT* ppt, D4CD_SIZE* psz, D4CD_STRING* buffText, D4CD_CHAR bckg);
void D4CD_DrawTextRectTo(D4CD_POINT* ppt, D4CD_STRING* buffText, D4CD_CHAR bckg);
void D4CD_DrawTextRectXY(D4CD_COOR x1, D4CD_COOR y1, D4CD_COOR x2, D4CD_COOR y2, D4CD_STRING* buffText, D4CD_CHAR bckg);
void D4CD_DrawTextRectToXY(D4CD_COOR x, D4CD_COOR y, D4CD_STRING* buffText, D4CD_CHAR bckg);

void D4CD_DrawTextLine(D4CD_POINT* ppt, D4CD_COOR len, D4CD_STRING* buffText, D4CD_CHAR bckg);
void D4CD_DrawTextLineTo(D4CD_POINT* ppt, D4CD_STRING* buffText, D4CD_CHAR bckg);
void D4CD_DrawTextLineXY(D4CD_COOR x1, D4CD_COOR y1, D4CD_COOR x2, D4CD_STRING* buffText, D4CD_CHAR bckg);
void D4CD_DrawTextLineToXY(D4CD_COOR x, D4CD_STRING* buffText, D4CD_CHAR bckg);


void D4CD_SetText(D4CD_OBJECT_PTR pObject, char* pText);
void D4CD_SetTextProperties(D4CD_OBJECT_PTR pObject, D4CD_TEXT_PROPERTIES property);
Word D4CD_GetTextLength(char *pi);


Byte D4CD_SprintDecU8(Byte val, char *pText, char fill);
Byte D4CD_SprintDecS8(sByte val, char *pText, char fill);
Byte D4CD_SprintDecU16(Word val, char *pText, char fill);
Byte D4CD_SprintDecS16(sWord val, char *pText, char fill);
Byte D4CD_SprintDecU32(LWord val, char *pText, char fill);
Byte D4CD_SprintDecS32(sLWord val, char *pText, char fill);
void D4CD_PrintNum(sWord value, Byte decimalPoint, D4CD_CHAR* pBuff, D4CD_CHAR decPointChar);
void D4CD_ChangeText(D4CD_STRING* pBuff, char* pNewText, char fillChar);
D4CD_BOOL D4CD_IsNum(D4CD_CHAR ch);
D4CD_BOOL D4CD_IsChar(D4CD_CHAR ch);
D4CD_BOOL D4CD_IsSpecial(D4CD_CHAR ch);



#endif /* __D4CD_H */









