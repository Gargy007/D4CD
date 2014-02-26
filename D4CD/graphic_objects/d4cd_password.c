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
* @file      d4cd_password.c
*
* @author    Petr Gargulak
* 
* @version   0.0.4.0
* 
* @date      Feb-14-2011
* 
* @brief     D4CD driver pswrd object c file 
*
******************************************************************************/

#include "d4cd.h"
#include "d4cd_private.h"

//Internal API
void D4CD_PswrdOnMessage(D4CD_MESSAGE* pMsg);
static void D4CD_PswrdOnKeyUp(D4CD_MESSAGE* pMsg);
static D4CD_BOOL D4CD_PswrdCompare(D4CD_OBJECT* pThis);

const D4CD_OBJECT_SYS_FUNCTION d4cd_pswrdSysFunc = 
{
  D4CD_OBJECT_PSWRD,
  D4CD_PswrdOnMessage,
  NULL
};

// temporary structure for pswrd coordinate calculation
typedef struct 
{
    D4CD_POINT position;
    D4CD_SIZE size;
    D4CD_COOR textPos;
    D4CD_COOR pswrdPos;
    D4CD_COOR postTextPos;
} D4CD_PSWRD_TMP_VAL;
  
//#define _calc (*((D4CD_PSWRD_TMP_VAL*)d4cd_scratchPad))
D4CD_PSWRD_TMP_VAL _calc;
    
static void D4CD_PswrdValue2Coor(D4CD_OBJECT* pThis)
{
 
  D4CD_PSWRD* pPswrd = D4CD_GET_PSWRD(pThis);
 
  _calc.position = D4CD_GetClientToScreenPoint(*pThis->pScreen, &pPswrd->scrPos);
  
  _calc.textPos = _calc.position.x;
  
#if (D4CD_PSWRD_SEL_CHAR == D4CD_TRUE)
  _calc.textPos++;  
#endif
  
  _calc.pswrdPos = (D4CD_COOR)(_calc.textPos + D4CD_GetTextLength(pPswrd->preText));
  
  _calc.postTextPos = (D4CD_COOR)(_calc.pswrdPos + pPswrd->pswrdSize);
  
  _calc.size.cx = (D4CD_COOR)(D4CD_GetTextLength(pPswrd->preText) + D4CD_GetTextLength(pPswrd->postText) + pPswrd->pswrdSize);
  
#if (D4CD_PSWRD_SEL_CHAR == D4CD_TRUE)
  _calc.size.cx++;
#endif
  
  _calc.size.cy = 1;
 
}

/*******************************************************
*
* PSWRD Drawing routine
*
*******************************************************/

static void D4CD_PswrdOnDraw(D4CD_MESSAGE* pMsg)
{
    D4CD_PSWRD* pPswrd = D4CD_GET_PSWRD(pMsg->pObject);    
    D4CD_PSWRD_DATA* pData = pPswrd->pData;
    D4CD_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4CD_STRING tmp_txtBuff;
    D4CD_TEXT_PROPERTIES tmp_text_prty; 
    D4CD_CHAR tmp_charBck;
    
    D4CD_PswrdValue2Coor(pMsg->pObject);
    
    if(draw.bits.bComplete)  
    {
      D4CD_FillRect(&_calc.position, &_calc.size, ' ');
      
      if(pPswrd->preText != NULL)
        D4CD_DrawSimpleTextXY(_calc.textPos, _calc.position.y, pPswrd->preText);
      
      if(pPswrd->postText != NULL)
        D4CD_DrawSimpleTextXY(_calc.postTextPos, _calc.position.y, pPswrd->postText);      
    }
    
#if (D4CD_CHECKBOX_SEL_CHAR == D4CD_TRUE) 
      if(draw.bits.bFocused)
        D4CD_PutChar(&_calc.position, D4CD_PSWRD_SEL_FOCUS_CHAR);
      else
        D4CD_PutChar(&_calc.position, D4CD_PSWRD_SEL_NORMAL_CHAR);
#endif
  
            
    
    _calc.position.x = _calc.pswrdPos;
    
    
    tmp_txtBuff.buffSize = (Word)(pPswrd->pswrdSize + 1);
    tmp_text_prty.all = D4CD_PSWRD_TXT_PRTY_DEFAULT << D4CD_BITFIELD_SHIFT;
    tmp_txtBuff.text_properties = &tmp_text_prty;    
    tmp_txtBuff.pText = (D4CD_CHAR*)(pPswrd->pData->value);
    
    if(D4CD_GetBlink() && (pData->status.bits.bSet))
    {
      tmp_charBck = pPswrd->pData->value[pData->status.bits.bSet-1];
      pPswrd->pData->value[pData->status.bits.bSet-1] = ' ';
    }
  
    D4CD_DrawTextLine(&_calc.position, pPswrd->pswrdSize, &tmp_txtBuff, ' ');
    
    if(D4CD_GetBlink() && (pData->status.bits.bSet))
      pPswrd->pData->value[pData->status.bits.bSet-1] = tmp_charBck;
}


static D4CD_BOOL D4CD_PswrdCompare(D4CD_OBJECT* pThis)
{
  D4CD_PSWRD* pPswrd = D4CD_GET_PSWRD(pThis);
  D4CD_PSWRD_DATA* pData = pPswrd->pData;
  Byte i;
  
  for(i=0;i < pPswrd->pswrdSize;i++)
    if(pPswrd->pPswrd[i] != pData->value[i])
      return D4CD_FALSE;
    
  return D4CD_TRUE;  
   
}

static void D4CD_PswrdOnKeyUp(D4CD_MESSAGE* pMsg)    
{
  D4CD_PSWRD* pPswrd = D4CD_GET_PSWRD(pMsg->pObject);
  D4CD_PSWRD_DATA* pData = pPswrd->pData;
  D4CD_OBJECT* pObject = pMsg->pObject;
  
  // invoke Change time procedure when enter key is pressed
  if(pMsg->prm.key == D4CD_KEY_SCANCODE_ENTER)
  {
    D4CD_POINT tmp_point;
    
    tmp_point = D4CD_GetClientToScreenPoint(pMsg->pScreen, &(D4CD_GET_PSWRD(pMsg->pObject)->scrPos));
    tmp_point.x += D4CD_GetTextLength(pPswrd->preText);
    
    if(pData->status.bits.bSet == 0)
    {
      pData->status.bits.bSet = 1;
      D4CD_CaptureKeys(pObject);
    }
    else
    {
      pData->status.bits.bSet++;
      if(pData->status.bits.bSet > pPswrd->pswrdSize)
      {
        pData->status.bits.bSet = 0;
        D4CD_CaptureKeys(NULL);
        // notify user that the value has changed        
        if(pPswrd->OnPswrdEngter)
          pPswrd->OnPswrdEngter(pObject, D4CD_PswrdCompare(pObject));
        
        D4CD_ShowDummyPswrd(pObject);
      }
    }
    
    if(pData->status.bits.bSet)
    {
      #if (D4CD_PSWRD_SEL_CHAR == D4CD_TRUE)
        tmp_point.x += pData->status.bits.bSet;
      #else
        tmp_point.x += (pData->status.bits.bSet - 1);
      #endif
        
      D4CD_SetCursor(&tmp_point, pMsg->pObject->flags->bits.bCursorEna, pMsg->pObject->flags->bits.bCursorMod);
    }
      
    pMsg->pObject->flags->bits.bRedraw = 1;        
  }
  
  if(pData->status.bits.bSet)     
  {
    D4CD_CHAR ch = pPswrd->pData->value[pData->status.bits.bSet-1];
    D4CD_CHAR ch_bck = ch;
    if(pMsg->prm.key == D4CD_PSWRD_KEY_SCANCODE_PLUS)
    {
      while(++ch != ch_bck)
      {
        if((D4CD_IsSpecial(ch) && pPswrd->pData->status.bits.bSpecialEnable) || (D4CD_IsNum(ch) && pPswrd->pData->status.bits.bNumEnable) || (D4CD_IsChar(ch) && pPswrd->pData->status.bits.bCharsEnable))
          break;
      }
    }
    
    if(pMsg->prm.key == D4CD_PSWRD_KEY_SCANCODE_MINUS)
    {
      while(--ch != ch_bck)
      {
        if((D4CD_IsSpecial(ch) && pPswrd->pData->status.bits.bSpecialEnable) || (D4CD_IsNum(ch) && pPswrd->pData->status.bits.bNumEnable) || (D4CD_IsChar(ch) && pPswrd->pData->status.bits.bCharsEnable))
          break;
      }  
    }
    
    pPswrd->pData->value[pData->status.bits.bSet-1] = ch;
    pMsg->pObject->flags->bits.bRedraw = 1;
  }
  
  if(pMsg->prm.key == D4CD_KEY_SCANCODE_ESC)
  {    
    pPswrd->pData->status.bits.bSet = 0;
    D4CD_CaptureKeys(NULL);  
    D4CD_ShowDummyPswrd(pObject);
  }
  
  
}

void D4CD_ShowDummyPswrd(D4CD_OBJECT_PTR pThis)
{
  D4CD_PSWRD* pPswrd = D4CD_GET_PSWRD(pThis);
  Byte i;
  
  // remember new value
  for(i=0;i < pPswrd->pswrdSize;i++)
    pPswrd->pData->value[i] = D4CD_PSWRD_HIDEPSWRD_CHAR;
    
  pPswrd->pData->value[pPswrd->pswrdSize] = 0;  
}

/*******************************************************
*
* Set PSWRD value
*
*******************************************************/

void D4CD_PswrdSet(D4CD_OBJECT_PTR pThis, D4CD_PSWRD_VALUE* pValue)
{
    D4CD_PSWRD* pPswrd = D4CD_GET_PSWRD(pThis);
    Byte i;
    
    if(pThis == NULL)
      return;
    
    if(pValue == NULL)
      return;
    
    // remember new value
    for(i=0;i<= pPswrd->pswrdSize;i++)
      pPswrd->pPswrd[i] = pValue[i];
}

D4CD_PSWRD_VALUE* D4CD_PswrdGetValue(D4CD_OBJECT_PTR pThis)
{
  D4CD_PSWRD* pPswrd = D4CD_GET_PSWRD(pThis);
  if(pThis == NULL)
    return 0;
    
  return pPswrd->pPswrd;
}

void D4CD_PswrdSetRange(D4CD_OBJECT_PTR pThis, D4CD_BOOL num, D4CD_BOOL chars, D4CD_BOOL special)
{
  D4CD_PSWRD* pPswrd = D4CD_GET_PSWRD(pThis);
  D4CD_PSWRD_DATA* pData = pPswrd->pData;

  if(pThis == NULL)
    return;

  pData->status.bits.bNumEnable = num;
  pData->status.bits.bCharsEnable = chars;
  pData->status.bits.bSpecialEnable = special;
  
  D4CD_ShowDummyPswrd((D4CD_OBJECT*)pThis);
  
}

/**************************************************************//*!
*
*
*
*     THE INTERNAL API FUNCTIONS
*
*
*
******************************************************************/

/*******************************************************
*
* The main PSWRD message handler 
*
*******************************************************/

void D4CD_PswrdOnMessage(D4CD_MESSAGE* pMsg)
{
  D4CD_POINT tmp_point;      
    
    switch(pMsg->nMsgId)
    {
    case D4CD_MSG_DRAW:
        D4CD_PswrdOnDraw(pMsg);
        break;
    
    case D4CD_MSG_KEYUP:
        if(pMsg->pObject->flags->bits.bEnabled)
          D4CD_PswrdOnKeyUp(pMsg);
        break;
        
    case D4CD_MSG_TIMETICK:
      if(pMsg->prm.blink.bits.bChange)
        if((D4CD_GET_PSWRD(pMsg->pObject))->pData->status.bits.bSet)
          pMsg->pObject->flags->bits.bRedraw = 1;
      break;              

    case D4CD_MSG_KILLFOCUS:  
      D4CD_SetCursor((D4CD_POINT*) &d4cd_size_zero, D4CD_FALSE, D4CD_FALSE);
    case D4CD_MSG_ONINIT:
      D4CD_ShowDummyPswrd(pMsg->pObject);
      break;  


    case D4CD_MSG_KILLCAPTURE:
      D4CD_GET_PSWRD(pMsg->pObject)->pData->status.bits.bSet = 0;
      pMsg->pObject->flags->bits.bRedraw = 1;
    
    case D4CD_MSG_SETFOCUS:
      tmp_point = D4CD_GetClientToScreenPoint(pMsg->pScreen, &(D4CD_GET_PSWRD(pMsg->pObject)->scrPos));
      #if (D4CD_PSWRD_SEL_CHAR == D4CD_TRUE)
        tmp_point.x++;
      #endif  
      D4CD_SetCursor(&tmp_point, pMsg->pObject->flags->bits.bCursorEna, pMsg->pObject->flags->bits.bCursorMod);
      break;



    default:
        // call the default behavior of all objects
        D4CD_ObjOnMessage(pMsg);
    }
}

