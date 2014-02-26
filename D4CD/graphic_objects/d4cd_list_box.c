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
* @file      d4cd_list_box.c
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Feb-14-2011
* 
* @brief     D4CD driver listBox object c file 
*
******************************************************************************/

#include "d4cd.h"
#include "d4cd_private.h"

//Internal API
void D4CD_ListBoxOnMessage(D4CD_MESSAGE* pMsg);
D4CD_LIST_BOX_INDEX D4CD_ListBoxGetItemCount(D4CD_OBJECT_PTR pThis);
D4CD_COOR D4CD_ListBoxGetMaxItemLen(D4CD_OBJECT_PTR pThis);
static void D4CD_ListBoxOnKeyUp(D4CD_MESSAGE* pMsg);



const D4CD_OBJECT_SYS_FUNCTION d4cd_listBoxSysFunc = 
{
  D4CD_OBJECT_LIST_BOX,
  D4CD_ListBoxOnMessage,
  NULL
};

// temporary structure for listBox coordinate calculation
typedef struct 
{
    D4CD_POINT position;
    D4CD_SIZE size;
    D4CD_SIZE itemSize;
    D4CD_COOR textPos;
    D4CD_COOR itemPos;
    D4CD_COOR postTextPos;
} D4CD_LIST_BOX_TMP_VAL;
  
#define _calc (*((D4CD_LIST_BOX_TMP_VAL*)d4cd_scratchPad))
//D4CD_LIST_BOX_TMP_VAL _calc;
    
static void D4CD_ListBoxValue2Coor(D4CD_OBJECT* pThis)
{
 
  D4CD_LIST_BOX* pListBox = D4CD_GET_LIST_BOX(pThis);

  
  
  _calc.position = D4CD_GetClientToScreenPoint(*pThis->pScreen, &pListBox->scrPos);
  
  _calc.size = pListBox->scrSize;
  
  #if  D4CD_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities
    
    if(!_calc.size.cx)
    {
      _calc.size.cx = (D4CD_COOR)(D4CD_ListBoxGetMaxItemLen(pThis) + D4CD_GetTextLength(pListBox->preText) + D4CD_GetTextLength(pListBox->postText));
#if (D4CD_LISTBOX_SEL_CHAR == D4CD_TRUE)
      _calc.size.cx++;
#endif      
      
    }
    
    if(!_calc.size.cy)
    {
      _calc.size.cy = 1;
    }
#endif
  
  
  
  _calc.textPos = _calc.position.x;
  
#if (D4CD_LISTBOX_SEL_CHAR == D4CD_TRUE)
  _calc.textPos++;  
#endif
  
  _calc.itemPos = (D4CD_COOR)(_calc.textPos + D4CD_GetTextLength(pListBox->preText));
  
  _calc.postTextPos = (D4CD_COOR)(_calc.position.x + _calc.size.cx - D4CD_GetTextLength(pListBox->postText));
  
  _calc.itemSize.cx = (D4CD_COOR)(_calc.postTextPos - _calc.itemPos);
  _calc.itemSize.cy = 1;

 
}

/*******************************************************
*
* LIST_BOX Drawing routine
*
*******************************************************/

static void D4CD_ListBoxOnDraw(D4CD_MESSAGE* pMsg)
{
    D4CD_LIST_BOX* pListBox = D4CD_GET_LIST_BOX(pMsg->pObject);    
    D4CD_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4CD_STRING tmp_txtBuff;
    D4CD_TEXT_PROPERTIES tmp_text_prty;
    D4CD_ListBoxValue2Coor(pMsg->pObject);
    
  
    if(draw.bits.bComplete)  
    {
      D4CD_FillRect(&_calc.position, &_calc.size, ' ');
      
      if(pListBox->preText != NULL)
        D4CD_DrawSimpleTextXY(_calc.textPos, _calc.position.y, pListBox->preText);
      
      if(pListBox->postText != NULL)
        D4CD_DrawSimpleTextXY(_calc.postTextPos, _calc.position.y, pListBox->postText);      
    }
    
#if (D4CD_LISTBOX_SEL_CHAR == D4CD_TRUE)    
      if(draw.bits.bFocused)
        D4CD_PutChar(&_calc.position, D4CD_LISTBOX_SEL_FOCUS_CHAR);
      else
        D4CD_PutChar(&_calc.position, D4CD_LISTBOX_SEL_NORMAL_CHAR);
#endif  
            
    if(pListBox->pItems[pListBox->pData->index].pText != NULL)
    {
      _calc.position.x = _calc.itemPos;
      if((pListBox->pData->status.bits.bSet) && D4CD_GetBlink())
      {
        D4CD_FillRect(&_calc.position, &_calc.itemSize, ' ');
      }else
      {
        tmp_txtBuff.pText = (pListBox->pItems[pListBox->pData->index].pText);
        tmp_txtBuff.buffSize = _calc.itemSize.cx;
        tmp_text_prty.all = D4CD_LISTBOX_ITEM_TXT_PRTY_DEFAULT << D4CD_BITFIELD_SHIFT;
        tmp_txtBuff.text_properties = &tmp_text_prty;
        D4CD_DrawTextLine(&_calc.position, _calc.itemSize.cx, &tmp_txtBuff, ' ');
      }
    }
  
}

static void D4CD_ListBoxOnKeyUp(D4CD_MESSAGE* pMsg)    
{
  D4CD_LIST_BOX* pListBox = D4CD_GET_LIST_BOX(pMsg->pObject);
  // invoke Change time procedure when enter key is pressed
  if(pMsg->prm.key == D4CD_KEY_SCANCODE_ENTER)
  {
    if(pListBox->pData->status.bits.bSet == 0)
    {
      pListBox->pData->status.bits.bSet = 1;
      D4CD_CaptureKeys(pMsg->pObject);
    }
    else
    {
      pListBox->pData->status.bits.bSet = 0;
      D4CD_CaptureKeys(NULL);
      if(pListBox->OnValueChanged)
        pListBox->OnValueChanged((D4CD_OBJECT*)pMsg->pObject, pListBox->pItems[pListBox->pData->index].pUser);
    }
      
    pMsg->pObject->flags->bits.bRedraw = 1;        
  }
  
  if(pMsg->prm.key == D4CD_LISTBOX_KEY_SCANCODE_PLUS)
  {
    D4CD_ListBoxChangeIndex(pMsg->pObject, 1);  
  }
  
  if(pMsg->prm.key == D4CD_LISTBOX_KEY_SCANCODE_MINUS)
  {
    D4CD_ListBoxChangeIndex(pMsg->pObject, -1);
  }
  
  if(pMsg->prm.key == D4CD_KEY_SCANCODE_ESC)
  {    
    pListBox->pData->status.bits.bSet = 0;
    D4CD_CaptureKeys(NULL);  
  }
  
  
}

/*******************************************************
*
* Get LIST_BOX count
*
*******************************************************/

D4CD_LIST_BOX_INDEX D4CD_ListBoxGetItemCount(D4CD_OBJECT_PTR pThis)
{
   D4CD_LIST_BOX* pListBox = D4CD_GET_LIST_BOX(pThis);
   D4CD_LIST_BOX_INDEX  ix = 0;
   
   
   while(pListBox->pItems[ix].pText != NULL)
   {
     ix++;  
   }
   
   return ix;
}

/*******************************************************
*
* Get LIST_BOX max item lenght
*
*******************************************************/

D4CD_COOR D4CD_ListBoxGetMaxItemLen(D4CD_OBJECT_PTR pThis)
{
   D4CD_LIST_BOX* pListBox = D4CD_GET_LIST_BOX(pThis);
   D4CD_LIST_BOX_INDEX  ix = 0;
   D4CD_COOR len, tmp_len;
   
   len = 0;
   
   while(pListBox->pItems[ix].pText != NULL)
   {
     
     tmp_len = (D4CD_COOR)D4CD_GetTextLength(pListBox->pItems[ix].pText);  
     if(len < tmp_len)
      len = tmp_len; 
     
     ix++;
   }
   
   return len;
}

/*******************************************************
*
* Set LIST_BOX value
*
*******************************************************/

void D4CD_ListBoxSetIndex(D4CD_OBJECT_PTR pThis, D4CD_LIST_BOX_INDEX index)
{
    D4CD_LIST_BOX* pListBox = D4CD_GET_LIST_BOX(pThis);
    D4CD_LIST_BOX_DATA* pData = pListBox->pData;

    if(pData->index == index)
        return ;
    
    if(D4CD_ListBoxGetItemCount(pThis) == 0)  // There is no loaded bitmaps - go out
        return;
    
    if(index >= D4CD_ListBoxGetItemCount(pThis))  // If value is bigger then count of BMPs
        return;
    
    // remember new value
    pData->index = index;
    
    D4CD_InvalidateObject(pThis, D4CD_TRUE);

    // notify user that the value has changed        
    if(pData->status.bits.bSet == 0)
    {
      if(pListBox->OnValueChanged)
        pListBox->OnValueChanged((D4CD_OBJECT*)pThis, pListBox->pItems[pListBox->pData->index].pUser);
    }
}


/*******************************************************
*
* Change LIST_BOX value
*
*******************************************************/

void D4CD_ListBoxChangeIndex(D4CD_OBJECT_PTR pThis, sByte incr)
{
  sByte tmp_ix = (sByte)(D4CD_GET_LIST_BOX(pThis)->pData->index + incr);
  
  
  if(tmp_ix >= D4CD_ListBoxGetItemCount(pThis))
    tmp_ix = 0;
  
  if(tmp_ix < 0)
    tmp_ix = (sByte)((D4CD_ListBoxGetItemCount(pThis)-1));
  
  D4CD_ListBoxSetIndex(pThis, (D4CD_LIST_BOX_INDEX)tmp_ix);
}


D4CD_LIST_BOX_INDEX D4CD_ListBoxGetIndex(D4CD_OBJECT_PTR pThis)
{
    D4CD_LIST_BOX* pListBox = D4CD_GET_LIST_BOX(pThis);
    return pListBox->pData->index;
}

D4CD_LIST_BOX_INDEX D4CD_ListBoxFindUserDataItem(D4CD_OBJECT_PTR pThis, void* pUser)
{
    D4CD_LIST_BOX* pListBox = D4CD_GET_LIST_BOX(pThis);
    Byte i_max = D4CD_ListBoxGetItemCount(pThis);
    Byte i;
    
    for(i=0;i< i_max;i++)
    {
      if(pListBox->pItems[i].pUser == pUser)
        return i;
    }
    
    return 0xff;     
}

void* D4CD_ListBoxGetItemUserData(D4CD_OBJECT_PTR pThis)
{
  D4CD_LIST_BOX* pListBox = D4CD_GET_LIST_BOX(pThis);
  return pListBox->pItems[pListBox->pData->index].pUser;
}

D4CD_CHAR* D4CD_ListBoxGetItemText(D4CD_OBJECT_PTR pThis)
{
  D4CD_LIST_BOX* pListBox = D4CD_GET_LIST_BOX(pThis);
  return pListBox->pItems[pListBox->pData->index].pText;
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
* The main LIST_BOX message handler 
*
*******************************************************/

void D4CD_ListBoxOnMessage(D4CD_MESSAGE* pMsg)
{
    D4CD_POINT tmp_point;
    
    switch(pMsg->nMsgId)
    {
    case D4CD_MSG_DRAW:
        D4CD_ListBoxOnDraw(pMsg);
        break;
    
    case D4CD_MSG_KEYUP:
        if(pMsg->pObject->flags->bits.bEnabled)
          D4CD_ListBoxOnKeyUp(pMsg);
        break;
        
    case D4CD_MSG_TIMETICK:
      if(pMsg->prm.blink.bits.bChange)
        if((D4CD_GET_LIST_BOX(pMsg->pObject))->pData->status.bits.bSet)
          pMsg->pObject->flags->bits.bRedraw = 1;
      break;              

    

    case D4CD_MSG_KILLFOCUS:
      D4CD_SetCursor((D4CD_POINT*) &d4cd_size_zero, D4CD_FALSE, D4CD_FALSE);
      break;

    case D4CD_MSG_SETCAPTURE:
      tmp_point = D4CD_GetClientToScreenPoint(pMsg->pScreen, &(D4CD_GET_LIST_BOX(pMsg->pObject)->scrPos));
      #if (D4CD_LISTBOX_SEL_CHAR == D4CD_TRUE)
        tmp_point.x++;
      #endif
      
      tmp_point.x += D4CD_GetTextLength(D4CD_GET_LIST_BOX(pMsg->pObject)->preText);
      
      D4CD_SetCursor(&tmp_point, pMsg->pObject->flags->bits.bCursorEna, pMsg->pObject->flags->bits.bCursorMod);
      break;
    
    case D4CD_MSG_KILLCAPTURE:
      D4CD_GET_LIST_BOX(pMsg->pObject)->pData->status.bits.bSet = 0;
      pMsg->pObject->flags->bits.bRedraw = 1;
    
    case D4CD_MSG_SETFOCUS:
      tmp_point = D4CD_GetClientToScreenPoint(pMsg->pScreen, &(D4CD_GET_LIST_BOX(pMsg->pObject)->scrPos));
      #if (D4CD_LISTBOX_SEL_CHAR == D4CD_TRUE)
        tmp_point.x++;
      #endif  
      D4CD_SetCursor(&tmp_point, pMsg->pObject->flags->bits.bCursorEna, pMsg->pObject->flags->bits.bCursorMod);
      break;

    default:
        // call the default behavior of all objects
        D4CD_ObjOnMessage(pMsg);
    }
}

