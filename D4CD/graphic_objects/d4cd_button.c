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
* @file      d4cd_button.c
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Feb-14-2011
* 
* @brief     D4CD driver button object c file 
*
******************************************************************************/

#include "d4cd.h"
#include "d4cd_private.h"

//Internal API
void D4CD_BtnOnMessage(D4CD_MESSAGE* pMsg);
D4CD_STRING* D4CD_BtnGetTextBuffer(D4CD_OBJECT* pThis);

const D4CD_OBJECT_SYS_FUNCTION d4cd_btnSysFunc = 
{
  D4CD_OBJECT_BUTTON,
  D4CD_BtnOnMessage,
  D4CD_BtnGetTextBuffer
};

typedef struct 
{
    D4CD_POINT Position;
    D4CD_SIZE Size;
    D4CD_COOR TextLen;
} D4CD_BTN_TMP_VAL;

#define _calc (*((D4CD_BTN_TMP_VAL*)d4cd_scratchPad))

    
static void D4CD_BtnValue2Coor(D4CD_OBJECT* pThis)
{
  D4CD_BUTTON* pBtn = D4CD_GET_BUTTON(pThis);
  Byte tmp_off = 0; 
  _calc.Size = pBtn->scrSize;
  _calc.Position = D4CD_GetClientToScreenPoint(*pThis->pScreen, &pBtn->scrPos);
  
#if  D4CD_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities
        
    //Check Size of button in axis X   
    if(!_calc.Size.cx)
    {
      #if (D4CD_BTN_SEL_CHAR_LEFT == D4CD_TRUE)
        tmp_off++;
      #endif  
      
      #if(D4CD_BTN_SEL_CHAR_RIGHT == D4CD_TRUE)
        tmp_off++;
      #endif
      
      _calc.Size.cx = (D4CD_COOR)(pBtn->textBuff.buffSize - 1 + tmp_off);
    }
    
    _calc.TextLen = (D4CD_COOR)(_calc.Size.cx - tmp_off);
    
    //Check Size of button in axis Y
    
    if(!_calc.Size.cy)
    {
      _calc.Size.cy++;
    }
    
#endif
  
}


/*******************************************************
*
* BUTTON Drawing routine
*
*******************************************************/

static void D4CD_BtnOnDraw(D4CD_MESSAGE* pMsg)
{
    D4CD_BUTTON* pBtn = D4CD_GET_BUTTON(pMsg->pObject);
    D4CD_BUTTON_STATUS* pStatus = D4CD_GET_BUTTON_STATUS(pMsg->pObject);    
    D4CD_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;

#if (D4CD_BTN_SEL_CHAR_LEFT == D4CD_TRUE) || (D4CD_BTN_SEL_CHAR_RIGHT == D4CD_TRUE)    
    D4CD_CHAR tmp_char;
#endif
    
    D4CD_BtnValue2Coor(pMsg->pObject);
    
#if (D4CD_BTN_SEL_CHAR_LEFT == D4CD_TRUE)
    if(draw.bits.bFocused)
      tmp_char = D4CD_BTN_SEL_FOCUS_CHAR_LEFT;  
    else
      tmp_char = D4CD_BTN_SEL_NORM_CHAR_LEFT;  
      
    D4CD_PutChar( &_calc.Position, tmp_char);
        
    _calc.Position.x++;
#endif

    // draw the text
    if(pBtn->textBuff.pText)
      D4CD_DrawTextLine(&_calc.Position, _calc.TextLen, &pBtn->textBuff, ' ');
    else
      D4CD_PutChar( &_calc.Position, ' ');  
  
#if (D4CD_BTN_SEL_CHAR_RIGHT == D4CD_TRUE)    
    if(draw.bits.bFocused)
      tmp_char = D4CD_BTN_SEL_FOCUS_CHAR_RIGHT;  
    else
      tmp_char = D4CD_BTN_SEL_NORM_CHAR_RIGHT;  
    
    _calc.Position.x += (Byte)(_calc.Size.cx - 1);
    
#if (D4CD_BTN_SEL_CHAR_LEFT == D4CD_TRUE)
    _calc.Position.x--;
#endif
    
    
    D4CD_PutChar( &_calc.Position, tmp_char);
#endif    
    
}

/*******************************************************
*
* BUTTON key handling routine
*
*******************************************************/

static void D4CD_BtnOnKeyDown(D4CD_MESSAGE* pMsg)    
{                
  // invoke "Click" event when enter key is pressed
  if(pMsg->prm.key == D4CD_KEY_SCANCODE_ENTER)
  {      
    D4CD_BUTTON_STATUS* pStatus = D4CD_GET_BUTTON_STATUS(pMsg->pObject);
    pStatus->bits.bPressed = 1;
    pMsg->pObject->flags->bits.bRedraw = 1;  
    
  }  
}

/*******************************************************
*
* BUTTON key handling routine
*
*******************************************************/

static void D4CD_BtnOnKeyUp(D4CD_MESSAGE* pMsg)    
{
                 
  // invoke "Click" event when enter key is pressed
  if(pMsg->prm.key == D4CD_KEY_SCANCODE_ENTER)
  {
    D4CD_BUTTON* pBtn = D4CD_GET_BUTTON(pMsg->pObject);
    D4CD_BUTTON_STATUS* pStatus = D4CD_GET_BUTTON_STATUS(pMsg->pObject);
    
    if(pStatus->bits.bPressed)
    {
      pStatus->bits.bPressed = 0;
      if(pBtn->OnClicked)
          pBtn->OnClicked(pMsg->pObject);
    }
    
    pMsg->pObject->flags->bits.bRedraw = 1;        
  }
}

/*******************************************************
*
* BUTTON focus handling routine
*
*******************************************************/

static void D4CD_BtnKillFocus(D4CD_MESSAGE* pMsg)
{
  D4CD_BUTTON_STATUS* pStatus = D4CD_GET_BUTTON_STATUS(pMsg->pObject);
  
  if(pStatus->bits.bPressed)
  {    
    pStatus->bits.bPressed = 0;
    pMsg->pObject->flags->bits.bRedraw = 1;
  }
  
  D4CD_SetCursor((D4CD_POINT*) &d4cd_size_zero, D4CD_FALSE, D4CD_FALSE);
  
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
* The main BUTTON message handler 
*
*******************************************************/

void D4CD_BtnOnMessage(D4CD_MESSAGE* pMsg)
{
    D4CD_POINT tmp_point;
   
    switch(pMsg->nMsgId)
    {
    case D4CD_MSG_DRAW:
        D4CD_BtnOnDraw(pMsg);
        break;
        
    case D4CD_MSG_KEYDOWN:
        if(pMsg->pObject->flags->bits.bEnabled)
          D4CD_BtnOnKeyDown(pMsg);
        break;

    case D4CD_MSG_KEYUP:
        if(pMsg->pObject->flags->bits.bEnabled)
          D4CD_BtnOnKeyUp(pMsg);
        break;
    
    case D4CD_MSG_SETFOCUS:
        
        tmp_point = D4CD_GetClientToScreenPoint(pMsg->pScreen, &(D4CD_GET_BUTTON(pMsg->pObject)->scrPos));
        #if (D4CD_BTN_SEL_CHAR_LEFT == D4CD_TRUE)
          tmp_point.x++;
        #endif  
        D4CD_SetCursor(&tmp_point, pMsg->pObject->flags->bits.bCursorEna, pMsg->pObject->flags->bits.bCursorMod);
        break;
        
    case D4CD_MSG_KILLFOCUS:
        D4CD_BtnKillFocus(pMsg);
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
D4CD_STRING* D4CD_BtnGetTextBuffer(D4CD_OBJECT* pThis)
{
  return &(D4CD_GET_BUTTON(pThis)->textBuff);  
}
