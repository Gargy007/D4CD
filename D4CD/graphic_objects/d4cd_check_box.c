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
* @file      d4cd_check_box.c
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Feb-14-2011
* 
* @brief     D4CD driver check box object c file 
*
******************************************************************************/

#include "d4cd.h"
#include "d4cd_private.h"

//Internal API
void D4CD_CheckBoxOnMessage(D4CD_MESSAGE* pMsg);
D4CD_STRING* D4CD_CheckBoxGetTextBuffer(D4CD_OBJECT* pThis);

const D4CD_OBJECT_SYS_FUNCTION d4cd_checkBoxSysFunc = 
{
  D4CD_OBJECT_CHECKBOX,
  D4CD_CheckBoxOnMessage,
  D4CD_CheckBoxGetTextBuffer
};

typedef struct 
{
    D4CD_POINT Position;
    D4CD_SIZE Size;
    D4CD_COOR TxtPos_x;
} D4CD_CHECKBOX_TMP_VAL;

#define _calc (*((D4CD_CHECKBOX_TMP_VAL*)d4cd_scratchPad))

    
static void D4CD_CheckBoxValue2Coor(D4CD_OBJECT* pThis)
{
  D4CD_CHECKBOX* pCheckB = D4CD_GET_CHECKBOX(pThis);
  _calc.Size = pCheckB->scrSize;
  _calc.Position = D4CD_GetClientToScreenPoint(*pThis->pScreen, &pCheckB->scrPos);
 
  
  
#if  D4CD_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities
        
    //Check Size of button in axis X   
    if(!_calc.Size.cx)
    {
      _calc.Size.cx = (D4CD_COOR)((pCheckB->textBuff.buffSize - 1) + D4CD_CHECKBOX_TEXT_OFFSET + 1); // 1 for  checked char
      
      #if (D4CD_CHECKBOX_SEL_CHAR == D4CD_TRUE)
        _calc.Size.cx++;
      #endif  
    }
    
    //Check Size of button in axis Y
    
    if(!_calc.Size.cy)
    {
      _calc.Size.cy = 1;
    }
#else
    

#endif

    if(pCheckB->textBuff.pText)
    {
      _calc.TxtPos_x = (D4CD_COOR) (_calc.Position.x +  D4CD_CHECKBOX_TEXT_OFFSET + 1);      
      #if (D4CD_CHECKBOX_SEL_CHAR == D4CD_TRUE)      
        _calc.TxtPos_x++;
      #endif  
    }
    
  

}


/*******************************************************
*
* CHECK BOX Drawing routine
*
*******************************************************/

static void D4CD_CheckBoxOnDraw(D4CD_MESSAGE* pMsg)
{
    D4CD_CHECKBOX* pCheckB = D4CD_GET_CHECKBOX(pMsg->pObject);
    D4CD_CHECKBOX_STATUS* pStatus = D4CD_GET_CHECKBOX_STATUS(pMsg->pObject);    
    D4CD_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;    
    D4CD_COOR tmp_size;
    
    D4CD_CheckBoxValue2Coor(pMsg->pObject);


    if(draw.bits.bComplete)
    {
      D4CD_FillRect(&_calc.Position, &_calc.Size, ' ');  
    }
    
    tmp_size = (D4CD_COOR)(_calc.TxtPos_x - _calc.Position.x);
    
    
#if (D4CD_CHECKBOX_SEL_CHAR == D4CD_TRUE)     
    if(pMsg->pObject->flags->bits.bTabStop)
    {
      if(draw.bits.bFocused)
        D4CD_PutChar(&_calc.Position, D4CD_CHECKBOX_FOCUS_CHAR);
      else
        D4CD_PutChar(&_calc.Position, D4CD_CHECKBOX_UNFOCUS_CHAR);    
      _calc.Position.x++;
    }
#endif    
    
 
    // normal bitmap (also may be NULL if simple rect button is needed  
    if(pStatus->bits.bChecked)
      D4CD_PutChar(&_calc.Position, D4CD_CHECKBOX_CHECKED_CHAR);
    else
      D4CD_PutChar(&_calc.Position, D4CD_CHECKBOX_UNCHECKED_CHAR);    
    // draw the text
    if(pCheckB->textBuff.pText)
    {        
      _calc.Size.cx -= tmp_size;      
      _calc.Position.x = _calc.TxtPos_x;
      D4CD_DrawTextLine(&_calc.Position, _calc.Size.cx, &pCheckB->textBuff, ' ');
    }
    
}

/*******************************************************
*
* CHECK BOX key handling routine
*
*******************************************************/

static void D4CD_CheckBoxOnKeyDown(D4CD_MESSAGE* pMsg)    
{
    D4CD_CHECKBOX_STATUS* pStatus = D4CD_GET_CHECKBOX_STATUS(pMsg->pObject);
    
    // invoke "Click" event when enter key is pressed
    if(pMsg->prm.key == D4CD_KEY_SCANCODE_ENTER)
    {      
      pStatus->bits.bPressed = 1;
      pMsg->pObject->flags->bits.bRedraw = 1;
    }
}

/*******************************************************
*
* CHECK BOX key handling routine
*
*******************************************************/

static void D4CD_CheckBoxOnKeyUp(D4CD_MESSAGE* pMsg)    
{
  D4CD_CHECKBOX* pCheckB = D4CD_GET_CHECKBOX(pMsg->pObject);
  D4CD_CHECKBOX_STATUS* pStatus = D4CD_GET_CHECKBOX_STATUS(pMsg->pObject);
  
  // invoke "Click" event when enter key is pressed
  if(pMsg->prm.key == D4CD_KEY_SCANCODE_ENTER && pMsg->pObject->flags->bits.bEnabled)
  {
    if(pStatus->bits.bPressed)
    {
      pStatus->bits.bPressed = 0;      
        pStatus->bits.bChecked = !pStatus->bits.bChecked;  
        if(pCheckB->OnChange)
          pCheckB->OnChange(pMsg->pObject);            
    }
    pMsg->pObject->flags->bits.bRedraw = 1; 
  } 
}

/*******************************************************
*
* CHECK BOX focus handling routine
*
*******************************************************/

static void D4CD_CheckBoxKillFocus(D4CD_MESSAGE* pMsg)
{
  D4CD_CHECKBOX_STATUS* pStatus = D4CD_GET_CHECKBOX_STATUS(pMsg->pObject);
  
  if(pStatus->bits.bPressed)
  {    
    pStatus->bits.bPressed = 0;
    pMsg->pObject->flags->bits.bRedraw = 1;
  }
  
  D4CD_SetCursor((D4CD_POINT*) &d4cd_size_zero, D4CD_FALSE, D4CD_FALSE);
  
}



/*******************************************************
*
* Set CHECK BOX value
*
*******************************************************/

void D4CD_CheckBoxSetValue(D4CD_OBJECT_PTR pThis, D4CD_BOOL value)
{
  D4CD_CHECKBOX* pCheckB = D4CD_GET_CHECKBOX(pThis);
  D4CD_CHECKBOX_STATUS* pStatus = D4CD_GET_CHECKBOX_STATUS(pThis);
  
  if(!pThis->flags->bits.bEnabled)
    return ;
    
  if((pStatus->bits.bChecked && value) || (!pStatus->bits.bChecked && !value))
    return ;
  
  // remember new value
  pStatus->bits.bChecked = (value)? 1:0;
    
  D4CD_InvalidateObject(pThis, D4CD_TRUE);

  // notify user that the value has changed        
  if(pCheckB->OnChange)
    pCheckB->OnChange((D4CD_OBJECT*)pThis);
}

Byte D4CD_CheckBoxGetValue(D4CD_OBJECT_PTR pThis)
{
  D4CD_CHECKBOX_STATUS* pStatus = D4CD_GET_CHECKBOX_STATUS(pThis);
  
  return (Byte)pStatus->bits.bChecked;
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
* The main CHECK BOX message handler 
*
*******************************************************/

void D4CD_CheckBoxOnMessage(D4CD_MESSAGE* pMsg)
{
  D4CD_POINT tmp_point;
      
    switch(pMsg->nMsgId)
    {
    case D4CD_MSG_DRAW:
        D4CD_CheckBoxOnDraw(pMsg);
        break;
        
    case D4CD_MSG_KEYDOWN:
        if(pMsg->pObject->flags->bits.bEnabled)
          D4CD_CheckBoxOnKeyDown(pMsg);
        break;

    case D4CD_MSG_KEYUP:
        if(pMsg->pObject->flags->bits.bEnabled)
          D4CD_CheckBoxOnKeyUp(pMsg);
        break;
        
     case D4CD_MSG_SETFOCUS:
        
        tmp_point = D4CD_GetClientToScreenPoint(pMsg->pScreen, &(D4CD_GET_CHECKBOX(pMsg->pObject)->scrPos));
#if (D4CD_CHECKBOX_SEL_CHAR == D4CD_TRUE) 
          tmp_point.x++;
#endif        
        D4CD_SetCursor(&tmp_point, pMsg->pObject->flags->bits.bCursorEna, pMsg->pObject->flags->bits.bCursorMod);
        break;    
        
    case D4CD_MSG_KILLFOCUS:
        D4CD_CheckBoxKillFocus(pMsg);
        break;

    default:
        // call the default behavior
        D4CD_ObjOnMessage(pMsg);
    }
}


/**************************************************************//*!
*
* Get object text buffer
*
******************************************************************/
D4CD_STRING* D4CD_CheckBoxGetTextBuffer(D4CD_OBJECT* pThis)
{
  return &(D4CD_GET_CHECKBOX(pThis)->textBuff);  
}