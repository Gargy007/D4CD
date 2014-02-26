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
* @file      d4cd_label.c
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Feb-14-2011
* 
* @brief     D4CD driver label object c file 
*
******************************************************************************/

#include "d4cd.h"
#include "d4cd_private.h"


// Internal API
void D4CD_LabelOnMessage(D4CD_MESSAGE* pMsg);
D4CD_STRING* D4CD_LabelGetTextBuffer(D4CD_OBJECT* pThis);

const D4CD_OBJECT_SYS_FUNCTION d4cd_labelSysFunc = 
{
  D4CD_OBJECT_LABEL,
  D4CD_LabelOnMessage,
  D4CD_LabelGetTextBuffer
};

typedef struct 
{
  D4CD_POINT position;
  D4CD_SIZE size;
  D4CD_COOR textLen;
  D4CD_COOR textPos;
  D4CD_COOR postTextPos;
} D4CD_LBL_TMP_VAL;

#define _calc (*((D4CD_LBL_TMP_VAL*)d4cd_scratchPad))

//static D4CD_LBL_TMP_VAL _calc;
    
static void D4CD_LblValue2Coor(D4CD_OBJECT* pThis)
{
  D4CD_LABEL* pLbl = D4CD_GET_LABEL(pThis);

  _calc.position = D4CD_GetClientToScreenPoint(*pThis->pScreen, &pLbl->scrPos);
  _calc.size = pLbl->scrSize;

  #if  D4CD_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities
      
  if(!_calc.size.cx)
  {
    // compute size of label object automaticcaly
    _calc.size.cx = (D4CD_COOR) ((pLbl->textBuff->buffSize - 1) + D4CD_GetTextLength(pLbl->preText) + D4CD_GetTextLength(pLbl->postText));         
  }
  
  if(!_calc.size.cy)
  {
    // compute size of label object automaticcaly
    _calc.size.cy = 1;        
  }
  
 #endif
   
  _calc.textPos = (D4CD_COOR) (D4CD_GetTextLength(pLbl->preText) + _calc.position.x); 
  
  _calc.postTextPos =  (D4CD_COOR) (_calc.position.x + _calc.size.cx - D4CD_GetTextLength(pLbl->postText));
  
  _calc.textLen = (D4CD_COOR) (_calc.postTextPos - _calc.textPos);

}
 


/*******************************************************
*
* LABEL Drawing routine
*
*******************************************************/

static void D4CD_LabelOnDraw(D4CD_MESSAGE* pMsg)
{
    D4CD_LABEL* pLbl = D4CD_GET_LABEL(pMsg->pObject);
    D4CD_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;

    
    D4CD_LblValue2Coor(pMsg->pObject);
    // draw the text
    if(draw.bits.bComplete)  
    {
      D4CD_FillRect(&_calc.position, &_calc.size, ' ');
      
      if(pLbl->preText != NULL)
        D4CD_DrawSimpleText(&_calc.position, pLbl->preText);
      
      if(pLbl->postText != NULL)
        D4CD_DrawSimpleTextXY(_calc.postTextPos, _calc.position.y, pLbl->postText);      
    }
    
    if(pLbl->textBuff->pText)
    {
      _calc.position.x = _calc.textPos;
      D4CD_DrawTextLine(&_calc.position, _calc.textLen, pLbl->textBuff, ' ');
    }
    
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
* The LABEL message handler 
*
*******************************************************/

void D4CD_LabelOnMessage(D4CD_MESSAGE* pMsg)
{
    switch(pMsg->nMsgId)
    {
    case D4CD_MSG_DRAW:
        D4CD_LabelOnDraw(pMsg);
        break;
    default:
        // call the default behavior of all objects
        D4CD_ObjOnMessage(pMsg);
    }
}


/**************************************************************//*!
*
* Get object text buffer
*
******************************************************************/
D4CD_STRING* D4CD_LabelGetTextBuffer(D4CD_OBJECT* pThis)
{
  return (D4CD_GET_LABEL(pThis)->textBuff);  
}

/**************************************************************//*!
*
* Get object text buffer
*
******************************************************************/
void D4CD_LabelSetTextBuffer(D4CD_OBJECT* pThis, D4CD_STRING* pTxtBuff)
{
  *(D4CD_GET_LABEL(pThis)->textBuff) = *pTxtBuff;  
}
