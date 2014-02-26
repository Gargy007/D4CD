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
* @file      d4cd_menu.c
*
* @author    Petr Gargulak
* 
* @version   0.0.3.0
* 
* @date      Feb-14-2011
* 
* @brief     D4CD driver menu object c file 
*
******************************************************************************/

#include "d4cd.h"
#include "d4cd_private.h"

//Internal API
void D4CD_MenuOnMessage(D4CD_MESSAGE* pMsg);
D4CD_STRING* D4CD_MenuGetTextBuffer(D4CD_OBJECT* pThis);

const D4CD_OBJECT_SYS_FUNCTION d4cd_menuSysFunc = 
{
  D4CD_OBJECT_MENU,
  D4CD_MenuOnMessage,
  D4CD_MenuGetTextBuffer
};

// temporary structure for menu temporary value calculation
typedef struct 
{
    D4CD_POINT position;
    D4CD_COOR titleBar_y;
    D4CD_MENU_INDEX posCnt;
    char index_txt[8];// = {"000/000"};
    Byte index_txt_len;
} D4CD_MENU_TMP_VAL;

#define _calc (*((D4CD_MENU_TMP_VAL*)d4cd_scratchPad))




/*******************************************************
*
* MENU Helper computing routines
*
*******************************************************/

static D4CD_MENU_INDEX D4CD_GetPositionCount(D4CD_OBJECT* pThis)
{
 D4CD_MENU* pMenu = D4CD_GET_MENU(pThis);
  return (D4CD_MENU_INDEX)(pMenu->scrSize.cy - ((pMenu->title_text.pText || (pThis->initFlags & D4CD_MENU_F_INDEX))? 1 : 0));
}


static void D4CD_MenuValue2Coor(D4CD_OBJECT* pThis)
{
    D4CD_MENU* pMenu = D4CD_GET_MENU(pThis);
    _calc.position = D4CD_GetClientToScreenPoint(*pThis->pScreen, &pMenu->scrPos);
    _calc.titleBar_y = (D4CD_COOR)(((pMenu->title_text.pText || (pThis->initFlags & D4CD_MENU_F_INDEX))? 1 : 0));

    _calc.posCnt = D4CD_GetPositionCount(pThis);

    
#if  D4CD_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities  
    

#endif
    
    if(pThis->initFlags & D4CD_MENU_F_INDEX)
    {      
      // Draw index counter
      if(pMenu->pData->ix >= *(pMenu->pItemsCnt))
      {
        _calc.index_txt_len = 1;
        _calc.index_txt[0] = 'X';
      }else
        _calc.index_txt_len = D4CD_SprintDecU8((Byte)(pMenu->pData->ix + 1), _calc.index_txt, (char) 0);
      _calc.index_txt[_calc.index_txt_len++] = '/';
      _calc.index_txt_len += D4CD_SprintDecU8((Byte)(*(pMenu->pItemsCnt)), &_calc.index_txt[_calc.index_txt_len], (char) 0);
      _calc.index_txt[_calc.index_txt_len] = 0;

    }

  
}

/*******************************************************
*
* MENU Drawing routine
*
*******************************************************/

static void D4CD_MenuOnDraw(D4CD_MESSAGE* pMsg)
{
    D4CD_MENU* pMenu = D4CD_GET_MENU(pMsg->pObject);
    D4CD_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;   
    D4CD_STRING tmp_txtbuff;
    D4CD_TEXT_PROPERTIES tmp_text_prty;
    Byte tmpB;
    D4CD_CHAR tmpChar;
    D4CD_POINT tmp_point;
    D4CD_SIZE tmp_size;
    D4CD_COOR tmp_Len;
    
    tmp_txtbuff.text_properties = &tmp_text_prty;    
    
    // Get background simple color 
    
    // Compute all tempoarary values
    D4CD_MenuValue2Coor(pMsg->pObject);
  
    // draw just a color rectangle instead of bitmap
    if(draw.bits.bComplete)  
    {      
      if(pMenu->title_text.pText)
      {                         
        tmp_Len = pMenu->scrSize.cx;
        if(pMsg->pObject->initFlags & D4CD_MENU_F_INDEX)
        {
          tmp_Len -= (D4CD_COOR)(((*pMenu->pItemsCnt) < 10)? 4:6);
        }
        
        D4CD_DrawTextLine(&_calc.position , tmp_Len, &pMenu->title_text, ' ');        
      }
    }
    
    if(pMsg->pObject->initFlags & D4CD_MENU_F_INDEX)
    {      
      // Draw index counter
      
      tmp_Len = (D4CD_COOR)(((*pMenu->pItemsCnt) < 10)? 3:5);

      tmp_point.x = (D4CD_COOR)(_calc.position.x + pMenu->scrSize.cx - tmp_Len);
      tmp_point.y = (D4CD_COOR)(_calc.position.y);
      
      tmp_txtbuff.pText = _calc.index_txt;
      
      tmp_text_prty.all = D4CD_MENU_IX_TXT_PRTY_DEFAULT << D4CD_BITFIELD_SHIFT;
      D4CD_DrawTextLine(&tmp_point, tmp_Len, &tmp_txtbuff, ' ');
    }
    
    for(tmpB = 0; (tmpB < _calc.posCnt) && (tmpB < *(pMenu->pItemsCnt)); tmpB++)
    {
      tmp_point.x = _calc.position.x;
      tmp_point.y = (D4CD_COOR)(_calc.position.y + _calc.titleBar_y + tmpB );
      
      if(((pMenu->pData->ix - pMenu->pData->focus_pos) != tmpB) || (!pMsg->pObject->flags->bits.bEnabled))
      {        
        //normal text
        tmpChar = D4CD_MENU_SIDEBAR_NORM_CHAR;
      }
      else
      {        
        //selected text 
        tmpChar = D4CD_MENU_SIDEBAR_FOCUS_CHAR;
        
        if(draw.bits.bCapturing)
          D4CD_SetCursor(&tmp_point, pMsg->pObject->flags->bits.bCursorEna, pMsg->pObject->flags->bits.bCursorMod); 
      }
      
      tmp_point.x += (pMenu->scrSize.cx - 1);
      
      D4CD_PutChar( &tmp_point, tmpChar); 
      
      // Draw menu item text
      if((pMenu->pData->item_select == -2) || (draw.bits.bComplete))
      {        
        tmp_point.x = (D4CD_COOR)(_calc.position.x);
        tmp_point.y = (D4CD_COOR)(_calc.position.y + _calc.titleBar_y + tmpB);
        
        tmp_size.cx = (D4CD_COOR)(_calc.position.x + pMenu->scrSize.cx - 1);
        tmp_size.cy = 1; 
        
        
        if(pMenu->pItems[tmpB + pMenu->pData->focus_pos].pText != NULL)
        {
          tmp_txtbuff.pText = pMenu->pItems[tmpB + pMenu->pData->focus_pos].pText;

          tmp_text_prty.all = D4CD_MENU_ITEM_TXT_PRTY_DEFAULT << D4CD_BITFIELD_SHIFT;
          
          D4CD_DrawTextLine(&tmp_point, tmp_size.cx, &tmp_txtbuff, ' ');        
        }else if(tmpB + pMenu->pData->focus_pos >= *(pMenu->pItemsCnt))
        {
          D4CD_FillRect(&tmp_point, &tmp_size, D4CD_MENU_ENDMENULINE_CHAR);
        }
        
        
      }
    }  
}

/*******************************************************
*
* MENU object focus next item routine
*
*******************************************************/
static void D4CD_MenuFocusNextItem(D4CD_OBJECT* pThis)
{
  D4CD_MENU* pMenu = D4CD_GET_MENU(pThis);
  D4CD_MENU_INDEX tmpPosCnt = D4CD_GetPositionCount(pThis);
  
  pMenu->pData->item_select = -1;
  pMenu->pData->ix++;

  if(*(pMenu->pItemsCnt) > tmpPosCnt)
  {
    if(pMenu->pData->ix > *(pMenu->pItemsCnt))
    {
      pMenu->pData->ix = 0;
      pMenu->pData->focus_pos = 0;
      pMenu->pData->item_select = -2;
    }

    if(pMenu->pData->ix - pMenu->pData->focus_pos > (tmpPosCnt - 1))
    {
      pMenu->pData->focus_pos++;
      pMenu->pData->item_select = -2; 
    }
  } else
  {
    if(pMenu->pData->ix >= *(pMenu->pItemsCnt))
    {
      pMenu->pData->ix = 0;        
    }

  }  
}

/*******************************************************
*
* MENU object focus previous item routine
*
*******************************************************/
static void D4CD_MenuFocusPreviousItem(D4CD_OBJECT* pThis)
{
  D4CD_MENU* pMenu = D4CD_GET_MENU(pThis);
  D4CD_MENU_INDEX tmpPosCnt = D4CD_GetPositionCount(pThis);
  
  pMenu->pData->item_select = -1;
  pMenu->pData->ix--;
  
  if(*(pMenu->pItemsCnt) > tmpPosCnt)
  {
    if(pMenu->pData->ix < 0)
    {
      pMenu->pData->ix = (D4CD_MENU_INDEX)(*(pMenu->pItemsCnt) - 0);
      pMenu->pData->focus_pos = (D4CD_MENU_INDEX)(*(pMenu->pItemsCnt) - tmpPosCnt + 1);
      pMenu->pData->item_select = -2;
    }else if(pMenu->pData->ix - pMenu->pData->focus_pos < 0)
    {
      pMenu->pData->focus_pos--;
      pMenu->pData->item_select = -2;
    }
  } else
  {
    if(pMenu->pData->ix < 0)
    {          
      pMenu->pData->ix = (D4CD_MENU_INDEX)(*(pMenu->pItemsCnt) - 1);
    }
  }  
}


/*******************************************************
*
* MENU key handling routine
*
*******************************************************/

static void D4CD_MenuOnKeyDown(D4CD_MESSAGE* pMsg)    
{
    D4CD_MENU* pMenu = D4CD_GET_MENU(pMsg->pObject);   
    
    
    if(pMsg->prm.key == D4CD_KEY_SCANCODE_UP)
    {      
      D4CD_MenuFocusNextItem(pMsg->pObject);
      pMsg->pObject->flags->bits.bRedraw = 1;   
    
    
    } else if(pMsg->prm.key == D4CD_KEY_SCANCODE_DOWN)
    {      
      D4CD_MenuFocusPreviousItem(pMsg->pObject);
      pMsg->pObject->flags->bits.bRedraw = 1; 
        
    } else if(pMsg->prm.key == D4CD_KEY_SCANCODE_ENTER)
    {    
      if(pMsg->pObject != D4CD_GetCapturedObject())
        D4CD_CaptureKeys(pMsg->pObject);
      else
      {
        pMenu->pData->item_select = pMenu->pData->ix;
        pMsg->pObject->flags->bits.bRedraw = 1;
      }
    } 
    
    if(pMsg->prm.key == D4CD_KEY_SCANCODE_ESC)
      D4CD_CaptureKeys(NULL);
    
}

/*******************************************************
*
* MENU key handling routine
*
*******************************************************/

static void D4CD_MenuOnKeyUp(D4CD_MESSAGE* pMsg)    
{
  D4CD_MENU* pMenu = D4CD_GET_MENU(pMsg->pObject);

  if(pMsg->prm.key == D4CD_KEY_SCANCODE_ENTER)
    if(pMenu->pData->item_select >= 0)
    {
      if(pMenu->OnClicked)
        pMenu->OnClicked(pMsg->pObject, pMenu->pData->item_select);
      
      pMenu->pData->item_select = -1;
      pMsg->pObject->flags->bits.bRedraw = 1;
    }
    
        
}



/*******************************************************
*
* The MENU Get index function 
*
*******************************************************/

D4CD_MENU_INDEX D4CD_MenuGetIndex(D4CD_OBJECT* pThis)
{
  D4CD_MENU* pMenu = D4CD_GET_MENU(pThis);
  
  if(pThis == NULL)
    return 0;
  
  return pMenu->pData->ix;
  
}

/*******************************************************
*
* The MENU Set index function 
*
*******************************************************/

void  D4CD_MenuSetIndex(D4CD_OBJECT* pThis, D4CD_MENU_INDEX ix)
{
  D4CD_MENU* pMenu = D4CD_GET_MENU(pThis);
  
  if(pThis == NULL)
    return;
  
  if(ix > (D4CD_MENU_INDEX)*pMenu->pItemsCnt)
    ix = (D4CD_MENU_INDEX)*pMenu->pItemsCnt;
  
  pMenu->pData->ix = ix;
  pMenu->pData->focus_pos = ix;
  pMenu->pData->item_select = -2;
  
}

/*******************************************************
*
* Get MENU item count
*
*******************************************************/

D4CD_MENU_INDEX D4CD_MenuGetItemCount(D4CD_OBJECT_PTR pThis)
{
   D4CD_MENU* pMenu = D4CD_GET_MENU(pThis);
   
   return *pMenu->pItemsCnt;
}

/*******************************************************
*
* The MENU find Item by user data function 
*
*******************************************************/
D4CD_MENU_INDEX D4CD_MenuFindUserDataItem(D4CD_OBJECT_PTR pThis, void* pUser)
{
    D4CD_MENU* pMenu = D4CD_GET_MENU(pThis);
    Byte i_max = D4CD_MenuGetItemCount(pThis);
    Byte i;
    
    for(i=0;i< i_max;i++)
    {
      if(pMenu->pItems[i].pUser == pUser)
        return i;
    }
    
    return (D4CD_MENU_INDEX)0xff;     
}

/*******************************************************
*
* The MENU get Item user data function 
*
*******************************************************/
void* D4CD_MenuGetItemUserData(D4CD_OBJECT_PTR pThis)
{
  D4CD_MENU* pMenu = D4CD_GET_MENU(pThis);
  return pMenu->pItems[pMenu->pData->ix].pUser;
}

/*******************************************************
*
* The MENU get Item Text function 
*
*******************************************************/
D4CD_CHAR* D4CD_MenuGetItemText(D4CD_OBJECT_PTR pThis)
{
  D4CD_MENU* pMenu = D4CD_GET_MENU(pThis);
  return pMenu->pItems[pMenu->pData->ix].pText;
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
* The MENU message handler 
*
*******************************************************/

void D4CD_MenuOnMessage(D4CD_MESSAGE* pMsg)
{
  D4CD_POINT tmp_point;
  
  switch(pMsg->nMsgId)
  {
    case D4CD_MSG_DRAW:
      D4CD_MenuOnDraw(pMsg);
      break;
      
    case D4CD_MSG_KEYDOWN:
      if(pMsg->pObject->flags->bits.bEnabled)
        D4CD_MenuOnKeyDown(pMsg);
      break;
    
    case D4CD_MSG_KEYUP:
      if(pMsg->pObject->flags->bits.bEnabled)
        D4CD_MenuOnKeyUp(pMsg);
      break;        
 
    case D4CD_MSG_SETFOCUS:
    case D4CD_MSG_KILLCAPTURE:
      tmp_point = D4CD_GetClientToScreenPoint(pMsg->pScreen, &(D4CD_GET_MENU(pMsg->pObject)->scrPos));
      D4CD_SetCursor(&tmp_point, pMsg->pObject->flags->bits.bCursorEna, pMsg->pObject->flags->bits.bCursorMod);
      break;
      
    case D4CD_MSG_KILLFOCUS:
      D4CD_SetCursor((D4CD_POINT*) &d4cd_size_zero, D4CD_FALSE, D4CD_FALSE);
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
D4CD_STRING* D4CD_MenuGetTextBuffer(D4CD_OBJECT* pThis)
{
  return &(D4CD_GET_MENU(pThis)->title_text);  
}