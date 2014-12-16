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
* @file      d4cd_number.c
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Feb-14-2011
* 
* @brief     D4CD driver number object c file 
*
******************************************************************************/

#include "d4cd.h"
#include "d4cd_private.h"

//Internal API
void D4CD_NumberOnMessage(D4CD_MESSAGE* pMsg);
static void D4CD_NumberOnKeyUp(D4CD_MESSAGE* pMsg);



const D4CD_OBJECT_SYS_FUNCTION d4cd_numberSysFunc = 
{
  D4CD_OBJECT_NUMBER,
  D4CD_NumberOnMessage,
  NULL
};

// temporary structure for number coordinate calculation
typedef struct 
{
    D4CD_POINT position;
    D4CD_SIZE size;
    D4CD_SIZE numSize;
    D4CD_COOR textPos;
    D4CD_COOR numPos;
    D4CD_COOR postTextPos;
} D4CD_NUMBER_TMP_VAL;
  
#define _calc (*((D4CD_NUMBER_TMP_VAL*)d4cd_scratchPad))
//D4CD_NUMBER_TMP_VAL _calc;
    
static void D4CD_NumberValue2Coor(D4CD_OBJECT* pThis)
{
 
  D4CD_NUMBER* pNumber = D4CD_GET_NUMBER(pThis);
 
  _calc.position = D4CD_GetClientToScreenPoint(*pThis->pScreen, &pNumber->scrPos);
  
  _calc.textPos = _calc.position.x;
  
#if (D4CD_NUMBER_SEL_CHAR == D4CD_TRUE)
    _calc.textPos++;  
#endif
  
  _calc.numPos = (D4CD_COOR)(_calc.textPos + D4CD_GetTextLength(pNumber->preText));
  
  _calc.numSize.cx = pNumber->numSize;
  _calc.numSize.cy = 1;
  
  #if  D4CD_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities
    
    if(!_calc.numSize.cx)
    {
      _calc.numSize.cx = 6;
    }    
  #endif
  
  if(pNumber->pData->limits.decimalPoint)
    _calc.numSize.cx++;  

  
  _calc.postTextPos = (D4CD_COOR)(_calc.numPos + _calc.numSize.cx);
  
  
  _calc.size.cx = (D4CD_COOR)(D4CD_GetTextLength(pNumber->preText) + D4CD_GetTextLength(pNumber->postText) + _calc.numSize.cx);

#if (D4CD_NUMBER_SEL_CHAR == D4CD_TRUE)
   _calc.size.cx++;
#endif    
    
  _calc.size.cy = 1;

 
}


void D4CD_NumberPrintNum(D4CD_OBJECT* pThis, D4CD_CHAR* pBuff)
{
  D4CD_NUMBER* pNumber = D4CD_GET_NUMBER(pThis);    
  D4CD_NUMBER_DATA* pData = pNumber->pData;   
  
  
  D4CD_PrintNum(pData->value, pNumber->pData->limits.decimalPoint, pBuff, D4CD_NUMBER_DEC_POINT_CHAR);  
}

/*******************************************************
*
* NUMBER Drawing routine
*
*******************************************************/

static void D4CD_NumberOnDraw(D4CD_MESSAGE* pMsg)
{
    D4CD_NUMBER* pNumber = D4CD_GET_NUMBER(pMsg->pObject);    
    D4CD_NUMBER_DATA* pData = pNumber->pData;
    D4CD_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4CD_STRING tmp_txtBuff;
    D4CD_TEXT_PROPERTIES tmp_text_prty; 
    D4CD_CHAR tmp_text[12];
    
    
    D4CD_NumberValue2Coor(pMsg->pObject);
    
    if(draw.bits.bComplete)  
    {
      D4CD_FillRect(&_calc.position, &_calc.size, ' ');
      
      if(pNumber->preText != NULL)
        D4CD_DrawSimpleTextXY(_calc.textPos, _calc.position.y, pNumber->preText);
      
      if(pNumber->postText != NULL)
        D4CD_DrawSimpleTextXY(_calc.postTextPos, _calc.position.y, pNumber->postText);      
    }
    
#if (D4CD_NUMBER_SEL_CHAR == D4CD_TRUE)
      if(draw.bits.bFocused)
        D4CD_PutChar(&_calc.position, D4CD_NUMBER_SEL_FOCUS_CHAR);
      else
        D4CD_PutChar(&_calc.position, D4CD_NUMBER_SEL_NORMAL_CHAR);
#endif  
            
    
    _calc.position.x = _calc.numPos;
    
    if(D4CD_GetBlink() && pData->status.bits.bSet)
    {
      D4CD_FillRect(&_calc.position, &_calc.numSize, ' ');
    }else
    {
      D4CD_NumberPrintNum(pMsg->pObject, tmp_text);
      tmp_txtBuff.pText = tmp_text;
      tmp_txtBuff.buffSize = _calc.numSize.cx;
      tmp_text_prty.all = D4CD_NUMBER_NUM_TXT_PRTY_DEFAULT << D4CD_BITFIELD_SHIFT;
      tmp_txtBuff.text_properties = &tmp_text_prty;
  
      
      D4CD_DrawTextLine(&_calc.position, _calc.numSize.cx, &tmp_txtBuff, ' ');
    }
  
  
}

static void D4CD_NumberOnKeyUp(D4CD_MESSAGE* pMsg)    
{
  D4CD_NUMBER* pNumber = D4CD_GET_NUMBER(pMsg->pObject);
  D4CD_NUMBER_DATA* pData = pNumber->pData;
  // invoke Change time procedure when enter key is pressed
  if(pMsg->prm.key == D4CD_KEY_SCANCODE_ENTER)
  {
    if(pNumber->pData->status.bits.bSet == 0)
    {
      pNumber->pData->status.bits.bSet = 1;
      D4CD_CaptureKeys(pMsg->pObject);
    }
    else
    {
      pNumber->pData->status.bits.bSet = 0;
      D4CD_CaptureKeys(NULL);
      // notify user that the value has changed        
      if(pNumber->OnValueChanged)
        pNumber->OnValueChanged(pMsg->pObject);
    }
      
    pMsg->pObject->flags->bits.bRedraw = 1;        
  }
  
  if(pMsg->prm.key == D4CD_NUMBER_KEY_SCANCODE_PLUS)
  {
    D4CD_NumberChangeValue(pMsg->pObject, pData->limits.changeStep);  
  }
  
  if(pMsg->prm.key == D4CD_NUMBER_KEY_SCANCODE_MINUS)
  {
    D4CD_NumberChangeValue(pMsg->pObject, (sWord)(pData->limits.changeStep * -1));
  }
  
  if(pMsg->prm.key == D4CD_KEY_SCANCODE_ESC)
  {    
    pNumber->pData->status.bits.bSet = 0;
    D4CD_CaptureKeys(NULL);  
  }   
}

/*******************************************************
*
* Set NUMBER value
*
*******************************************************/

void D4CD_NumberSetValue(D4CD_OBJECT_PTR pThis, D4CD_NUMBER_VALUE value)
{
    D4CD_NUMBER* pNumber = D4CD_GET_NUMBER(pThis);
    D4CD_NUMBER_DATA* pData = pNumber->pData;

    if(value > pData->limits.value_max)
      value = pData->limits.value_max;
    
    if(value < pData->limits.value_min)
      value = pData->limits.value_min;
    
    if(pData->value == value)
        return ;
    
    // remember new value
    pData->value = value;
    
    D4CD_InvalidateObject(pThis, D4CD_TRUE);

    // notify user that the value has changed        
    if(pData->status.bits.bSet == 0)
    {
      if(pNumber->OnValueChanged)
        pNumber->OnValueChanged(pThis);
    }
}


/*******************************************************
*
* Change NUMBER value
*
*******************************************************/

void D4CD_NumberChangeValue(D4CD_OBJECT_PTR pThis, sWord incr)
{
  sWord tmp_val = (sWord)(D4CD_GET_NUMBER(pThis)->pData->value + incr);
  D4CD_NumberSetValue(pThis, (D4CD_NUMBER_VALUE)tmp_val);
}


D4CD_NUMBER_VALUE D4CD_NumberGetValue(D4CD_OBJECT_PTR pThis)
{
    D4CD_NUMBER* pNumber = D4CD_GET_NUMBER(pThis);
    return pNumber->pData->value;
}

void D4CD_NumberSetLimits(D4CD_OBJECT_PTR pThis, D4CD_NUMBER_LIMITS* pLimits)
{
  D4CD_NUMBER* pNumber = D4CD_GET_NUMBER(pThis);
  D4CD_NUMBER_DATA* pData = pNumber->pData;

  pData->limits = *pLimits;
  
  D4CD_NumberSetValue(pThis, pData->value);    
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
* The main NUMBER message handler 
*
*******************************************************/

void D4CD_NumberOnMessage(D4CD_MESSAGE* pMsg)
{
    D4CD_POINT tmp_point;
    
    switch(pMsg->nMsgId)
    {
    case D4CD_MSG_DRAW:
        D4CD_NumberOnDraw(pMsg);
        break;
    
    case D4CD_MSG_KEYUP:
        if(pMsg->pObject->flags->bits.bEnabled)
          D4CD_NumberOnKeyUp(pMsg);
        break;
        
    case D4CD_MSG_TIMETICK:
      if(pMsg->prm.blink.bits.bChange)
        if((D4CD_GET_NUMBER(pMsg->pObject))->pData->status.bits.bSet == 1)
          pMsg->pObject->flags->bits.bRedraw = 1;
      break;              
      
    case D4CD_MSG_KILLFOCUS:
      D4CD_SetCursor((D4CD_POINT*) &d4cd_size_zero, D4CD_FALSE, D4CD_FALSE);
      break;

    case D4CD_MSG_SETCAPTURE:
      tmp_point = D4CD_GetClientToScreenPoint(pMsg->pScreen, &(D4CD_GET_NUMBER(pMsg->pObject)->scrPos));
      #if (D4CD_NUMBER_SEL_CHAR == D4CD_TRUE)
        tmp_point.x++;
      #endif
      
      tmp_point.x += D4CD_GetTextLength(D4CD_GET_NUMBER(pMsg->pObject)->preText);
      
      D4CD_SetCursor(&tmp_point, pMsg->pObject->flags->bits.bCursorEna, pMsg->pObject->flags->bits.bCursorMod);
      break;
    
    case D4CD_MSG_KILLCAPTURE:
      D4CD_GET_NUMBER(pMsg->pObject)->pData->status.bits.bSet = 0;
      pMsg->pObject->flags->bits.bRedraw = 1;
    
    case D4CD_MSG_SETFOCUS:
      tmp_point = D4CD_GetClientToScreenPoint(pMsg->pScreen, &(D4CD_GET_NUMBER(pMsg->pObject)->scrPos));
      #if (D4CD_NUMBER_SEL_CHAR == D4CD_TRUE)
        tmp_point.x++;
      #endif  
      D4CD_SetCursor(&tmp_point, pMsg->pObject->flags->bits.bCursorEna, pMsg->pObject->flags->bits.bCursorMod);
      break;  
      

    default:
        // call the default behavior of all objects
        D4CD_ObjOnMessage(pMsg);
    }
}

