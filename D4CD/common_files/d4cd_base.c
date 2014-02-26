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
* @file      d4cd_base.c
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Jan-12-2011
* 
* @brief     D4CD driver core and base function c file 
*
******************************************************************************/

#include "d4cd.h"
#include "d4cd_private.h"

/**************************************************************//*!
*
* Global variables
*
******************************************************************/

Byte d4cd_scratchPad[D4CD_SCRATCHPAD_SIZE];

/**************************************************************//*!
*
* Local variables
*
******************************************************************/

// screen activation hisotry array & index
D4CD_SCREEN* d4cd_screenHistory[D4CD_SCREEN_HISTORY];
Byte d4cd_screenHistoryIndex;

// if not NULL, keys are captured by this object
D4CD_OBJECT* d4cd_pKeysCapturer = NULL;

// one and only message object being used to route information to objects
D4CD_MESSAGE d4cd_msg;

const D4CD_SIZE d4cd_size_zero = {0, 0}; // zero size structure for automatic function capability

// The D4CD system flags
D4CD_SYSTEM_FLAGS d4cd_systemFlags;

// last point remembered for MoveTo, LineTo etc.
static D4CD_POINT d4cd_curXY;

// LCD HW cursor 
D4CD_CURSOR d4cd_cursor;

// Blink data storage
D4CD_BLINK d4cd_blink;

// The buffer of input keys
static D4CD_KEYS_BUFFER d4cd_KeysBuff;

/**************************************************************//*!
*
* Initialize
*
******************************************************************/

D4CD_BOOL D4CD_Init(D4CD_SCREEN* pInitScreen)
{
  if(!D4CD_LCD_Init()) // Init hardware of LCD
    return D4CD_FALSE;
  
  d4cd_screenHistoryIndex = 0;
  
  d4cd_blink.cnt = 0;
  d4cd_blink.mod = D4CD_BLINK_MODULO;
  d4cd_blink.state.all = 0;
  
  D4CD_SetCursor((D4CD_POINT*) &d4cd_size_zero, 0, 0);
  
  D4CD_ClearKeysBuffer();
  if(pInitScreen != NULL)
    D4CD_ActivateScreen(pInitScreen, D4CD_FALSE);
    
  return D4CD_TRUE;  
}

/**************************************************************//*!
*
* The main poll call
*
******************************************************************/

void D4CD_Poll(void)
{
    // get active screen
    D4CD_SCREEN* pScreen;
    
    // handle keys (may change active screen)
    D4CD_HandleKeys();
  
    pScreen = D4CD_GetActiveScreen();
    if(pScreen == NULL)
      return;
    // call screen's main function
    if(pScreen->OnMain)
        pScreen->OnMain();    
        
    // redraw all objects on active screen
    pScreen = D4CD_GetActiveScreen();
    D4CD_RedrawScreen(pScreen);
}

/**************************************************************//*!
*
* Put actual information about keys
*
******************************************************************/

void D4CD_TimeTickPut(void)
{
  d4cd_systemFlags.bits.bTimeTick = 1;  
  
  if(++d4cd_blink.cnt > d4cd_blink.mod)
  {
    d4cd_blink.cnt = 0;
    d4cd_blink.state.bits.bBlink = ~d4cd_blink.state.bits.bBlink;
    d4cd_blink.state.bits.bChange = 1;
  }
  
}


/**************************************************************//*!
*
* Put actual information about keys
*
******************************************************************/

D4CD_BOOL D4CD_GetBlink(void)
{
  return d4cd_blink.state.bits.bBlink;  
}

/**************************************************************//*!
*
* Clear all rest information about pushed Keys in buffer
*
******************************************************************/
void D4CD_ClearKeysBuffer(void)
{
  d4cd_KeysBuff.readPos = 0;
  d4cd_KeysBuff.writePos = 0;    
}


/**************************************************************//*!
*
* Put actual information about keys
*
******************************************************************/

void D4CD_KeysChanged(D4CD_KEYS keys)
{
  static D4CD_KEYS keysLastState = 0;
  
  
  
 if(keys != keysLastState)
 {
    D4CD_KEYS kChange;
    D4CD_KEY_SCANCODE tmp_scanCode;
    
    kChange = (Byte)(keys ^ keysLastState); 
    
    if(kChange & D4CD_KEY_UP)
    {
      tmp_scanCode = D4CD_KEY_SCANCODE_UP;
      if((keys & D4CD_KEY_UP) == 0) // the key is released
        tmp_scanCode |= D4CD_KEY_SCANCODE_RELEASEMASK;
      
      D4CD_NewKeyEvent(tmp_scanCode);      
    }
    
    if(kChange & D4CD_KEY_DOWN)
    {
      tmp_scanCode = D4CD_KEY_SCANCODE_DOWN;
      if((keys & D4CD_KEY_DOWN) == 0) // the key is released
        tmp_scanCode |= D4CD_KEY_SCANCODE_RELEASEMASK;
      
      D4CD_NewKeyEvent(tmp_scanCode);      
    }
    
    if(kChange & D4CD_KEY_LEFT)
    {
      tmp_scanCode = D4CD_KEY_SCANCODE_LEFT;
      if((keys & D4CD_KEY_LEFT) == 0) // the key is released
        tmp_scanCode |= D4CD_KEY_SCANCODE_RELEASEMASK;
      
      D4CD_NewKeyEvent(tmp_scanCode);      
    }
    
    if(kChange & D4CD_KEY_RIGHT)
    {
      tmp_scanCode = D4CD_KEY_SCANCODE_RIGHT;
      if((keys & D4CD_KEY_RIGHT) == 0) // the key is released
        tmp_scanCode |= D4CD_KEY_SCANCODE_RELEASEMASK;
      
      D4CD_NewKeyEvent(tmp_scanCode);      
    }
    
    if(kChange & D4CD_KEY_ENTER)
    {
      tmp_scanCode = D4CD_KEY_SCANCODE_ENTER;
      if((keys & D4CD_KEY_ENTER) == 0) // the key is released
        tmp_scanCode |= D4CD_KEY_SCANCODE_RELEASEMASK;
      
      D4CD_NewKeyEvent(tmp_scanCode);      
    }
    
    if(kChange & D4CD_KEY_ESC)
    {
      tmp_scanCode = D4CD_KEY_SCANCODE_ESC;
      if((keys & D4CD_KEY_ESC) == 0) // the key is released
        tmp_scanCode |= D4CD_KEY_SCANCODE_RELEASEMASK;
      
      D4CD_NewKeyEvent(tmp_scanCode);      
    }
    
    keysLastState = keys;           
 }   
}

/**************************************************************//*!
*
* Put new event from keys
*
******************************************************************/

void D4CD_NewKeyEvent(D4CD_KEY_SCANCODE scanCode)
{

  // Is there place for new record?
  if((d4cd_KeysBuff.writePos + 1 == d4cd_KeysBuff.readPos) || ((d4cd_KeysBuff.readPos == 0) && (d4cd_KeysBuff.writePos + 1 == D4CD_KEYS_BUFF_LENGTH))) 
  {
    return;
  }
  
  // place new record to buffer
  d4cd_KeysBuff.buff[d4cd_KeysBuff.writePos++] = scanCode;
  if(d4cd_KeysBuff.writePos >= D4CD_KEYS_BUFF_LENGTH)
    d4cd_KeysBuff.writePos = 0;  
  
  
  
  #ifdef D4CD_INPUT_EVENT_CALLBACK
    D4CD_INPUT_EVENT_CALLBACK();
  #endif
  
}


/**************************************************************//*!
*
* helper to invoke OBJECT's message handler 
*
* all information need to be already prepared in d4cd_msg
*
******************************************************************/

void D4CD_SendMessage(D4CD_MESSAGE* pMsg)
{
    D4CD_OBJECT* pObject = pMsg->pObject;
    
    // screen gets the first chance
    if(pMsg->pScreen->OnObjectMsg)
    {
        // screen may discard the message by returning TRUE
        if(pMsg->pScreen->OnObjectMsg(pMsg))
            return ;
    }
    
    if(pObject)
    {      
      // now invoke the object's handler        
      if(pObject->OnUsrMessage)
      {
        if(pObject->OnUsrMessage(pMsg))
          return;   
      }
      
      if(pObject->pObjFunc->OnSysMessage)
          pObject->pObjFunc->OnSysMessage(pMsg); 
    }
}

/**************************************************************//*!
*
* Get the last state of keys
*
******************************************************************/

D4CD_KEYS D4CD_GetKeys(void)
{
  //return d4cd_KeysBuff.d4cd_lastKeys;
  return (D4CD_KEYS)0;
}

/**************************************************************//*!
*
* Handle the keys (part of main poll call)
*
******************************************************************/
void D4CD_HandleKeys(void)
{
    // get active screen
    D4CD_SCREEN* pScreen = D4CD_GetActiveScreen();
    D4CD_OBJECT* pFocus;
    D4CD_KEY_SCANCODE scanCode;
    D4CD_BOOL tmp_release;
    
    // Check the buffer of input keys changes
    if(d4cd_KeysBuff.writePos == d4cd_KeysBuff.readPos)
      return;
    
    // Read the latest record in the buffer of input key event
    scanCode = d4cd_KeysBuff.buff[d4cd_KeysBuff.readPos++];
    
    // Check the overflow of the read pointer of readed input keys
    if(d4cd_KeysBuff.readPos >= D4CD_KEYS_BUFF_LENGTH)
        d4cd_KeysBuff.readPos = 0;
    
    tmp_release = scanCode & D4CD_KEY_SCANCODE_RELEASEMASK;
    scanCode &= ~D4CD_KEY_SCANCODE_RELEASEMASK;

    // do we handle the keys ourselves (to navigate across the screen) ?
    if(!d4cd_pKeysCapturer)
    {
      // escape the screen?
      if(scanCode == D4CD_KEY_SCANCODE_ESC)
      {            
        if(tmp_release == 0)
          D4CD_EscapeScreen();
        return;
      }
      // focus previous object on the screen?
      else if(scanCode ==  D4CD_KEY_FUNC_FOCUS_PREV)          
      {
        if(tmp_release == 0)
          D4CD_FocusPrevObject(pScreen);
        return;
      }          
      // focus next object on the screen?
      else if(scanCode ==  D4CD_KEY_FUNC_FOCUS_NEXT)
      {
        if(tmp_release == 0)
          D4CD_FocusNextObject(pScreen, D4CD_FALSE);
        return;
      }
    }
             
    // does the object get the key events?    
    // yes, invoke the key events to the active object
    pFocus = (D4CD_OBJECT*) (d4cd_pKeysCapturer ? d4cd_pKeysCapturer : D4CD_GetFocusedObject(pScreen));

    // prepare the message            
    d4cd_msg.pScreen = pScreen;
    d4cd_msg.pObject = pFocus;

    // if key was pressed down?    
    if(tmp_release == 0)
    {
      d4cd_msg.nMsgId = D4CD_MSG_KEYDOWN;
    }else
    // if key was released up?
    {
      d4cd_msg.nMsgId = D4CD_MSG_KEYUP;
    }
    d4cd_msg.prm.key = (D4CD_KEY_SCANCODE)(scanCode & D4CD_KEY_SCANCODE_KEYMASK);
    D4CD_SendMessage(&d4cd_msg);
    
}

/**************************************************************//*!
*
* convert decimal number to string
*
******************************************************************/

Byte D4CD_SprintDecU32(LWord val, char *pText, char fill) 
{
  LWord value;
  LWord divider;
  Byte cnt = 1;
  Byte fill_cnt = 0;
  Byte tmp_cnt;
  value = val;
  divider =  1000000000;
  
  for(tmp_cnt = 0; tmp_cnt < 9; tmp_cnt++)
  {
    if((value > divider - 1) || (cnt > 1))
    {    
      if(pText != NULL)
        *pText++ = (char)((value/divider)+'0');
      cnt++;
    } else if((pText != NULL) && (fill)) 
    {
      fill_cnt++;
      *pText++ = fill;
    }
    
    value %= divider;			
    divider /= 10;
  }
  
	if(pText != NULL)
    *pText++ = (char)((value)+'0');
  
  return (Byte)(cnt + fill_cnt);
}

/**************************************************************//*!
*
* convert signed decimal number to string
*
******************************************************************/

Byte D4CD_SprintDecS32(sLWord val, char *pText, char fill) 
{
  Byte cnt = 0;
  if(val < 0)
  {
    *pText++ = '-';
    cnt++;
    val *= -1;
  }
  return (Byte)(cnt + D4CD_SprintDecU32((LWord)val, pText, fill));
}

/**************************************************************//*!
*
* convert decimal number to string
*
******************************************************************/

Byte D4CD_SprintDecU16(Word val, char *pText, char fill) 
{
  Word value;
  Word divider;
  Byte cnt = 1;
  Byte fill_cnt = 0;
  Byte tmp_cnt;
  value = val;
  divider =  10000;
  
  for(tmp_cnt = 0; tmp_cnt < 4; tmp_cnt++)
  {
    if((value > divider - 1) || (cnt > 1))
    {    
      if(pText != NULL)
        *pText++ = (char)((value/divider)+'0');
      cnt++;
    }else if((pText != NULL) && (fill)) 
    {
      fill_cnt++;
      *pText++ = fill;
    }
        
    value %= divider;			
    divider /= 10;
  }

	if(pText != NULL)
    *pText++ = (char)((value)+'0');
  
  return (Byte)(cnt + fill_cnt);
}

/**************************************************************//*!
*
* convert signed decimal number to string
*
******************************************************************/

Byte D4CD_SprintDecS16(sWord val, char *pText, char fill) 
{
  Byte cnt = 0;
  if(val < 0)
  {
    *pText++ = '-';
    cnt++;
    val *= -1;
  }
  return (Byte)(cnt + D4CD_SprintDecU16((Word)val, pText, fill));
}

/**************************************************************//*!
*
* convert decimal number to string
*
******************************************************************/

Byte D4CD_SprintDecU8(Byte val, char *pText, char fill) 
{
  Byte value;
  Byte cnt = 1;
  Byte fill_cnt = 0;  
  value = val;
  
  if(value > 99)
  {    
    cnt++;
    if(pText != NULL)
      *pText++ = (char)((value/100)+'0');
    value %= 100;
    
  }else if((pText != NULL) && (fill))
  {
    fill_cnt++;
    *pText++ = fill;
  }
  
  if((value > 9) || (cnt > 1))
  {    
    cnt++;
    if(pText != NULL)
      *pText++ = (char)((value/10)+'0');
    value %= 10;
    
  }else if((pText != NULL) && (fill))
  {
    fill_cnt++;
    *pText++ = fill;
  }
      
  			
	
	if(pText != NULL)
    *pText++ = (char)((value)+'0');
  
  return (Byte)(cnt + fill_cnt);
}

/**************************************************************//*!
*
* convert signed decimal number to string
*
******************************************************************/

Byte D4CD_SprintDecS8(sByte val, char *pText, char fill) 
{
  Byte cnt = 0;
  if(val < 0)
  {
    *pText++ = '-';
    cnt++;
    val *= -1;
  }
  return (Byte)(cnt + D4CD_SprintDecU8((Byte)val, pText, fill));
}


/**************************************************************//*!
*
* Is ASCII char Number?
*
******************************************************************/
D4CD_BOOL D4CD_IsNum(D4CD_CHAR ch)
{
  if((ch >= '0') && (ch <= '9'))
    return D4CD_TRUE;
  else
    return D4CD_FALSE;
}

/**************************************************************//*!
*
* Is ASCII char normal Char?
*
******************************************************************/
D4CD_BOOL D4CD_IsChar(D4CD_CHAR ch)
{
  if(((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')))
    return D4CD_TRUE;
  else
    return D4CD_FALSE;
}

/**************************************************************//*!
*
* Is ASCII char special Char?
*
******************************************************************/
D4CD_BOOL D4CD_IsSpecial(D4CD_CHAR ch)
{
  if((ch >= ' ') && (ch <= '/'))
    return D4CD_TRUE;
  else
    return D4CD_FALSE;
}

/**************************************************************//*!
*
* Get curent text length
*
******************************************************************/

Word D4CD_GetTextLength(char *pText) 
{
  Word cnt = 0;
  
  if(pText == NULL)
    return 0;
  
  while(*(pText++) != 0)
    cnt++;

  return cnt;
}


/**************************************************************//*!
*
* change text in the buffer
*
******************************************************************/

void D4CD_ChangeText(D4CD_STRING* pBuff, char* pNewText, char fillChar)
{
    Word n = pBuff->buffSize;
    char* pDest = pBuff->pText;
    
    // preserve one byte in the buffer
    if(pDest != pNewText)
    {
      while(n > 1 && *pNewText)
      {
        *pDest = *pNewText;
        pNewText++;
        pDest++;
        n--;        
      }
    }else
    {
      Word i;
      i = D4CD_GetTextLength(pDest);
      n -= i;
      pDest += i;
    }
    
    while(n > 1 && fillChar)
    {
      *(pDest++) = fillChar;
      n--;        
    }
    
    // terminate string always
    *pDest = 0;
}

/**************************************************************//*!
*
* Set Object text 
*
******************************************************************/

void D4CD_SetText(D4CD_OBJECT_PTR pObject, char* pText) 
{
  D4CD_STRING* p_TextBuff = NULL;
  
  if(pObject->pObjFunc->GetTextBuffer)
    p_TextBuff = pObject->pObjFunc->GetTextBuffer((D4CD_OBJECT*)pObject);
  
  if(p_TextBuff)
  {
    D4CD_ChangeText(p_TextBuff, pText, 0);  
    D4CD_InvalidateObject(pObject, D4CD_FALSE);
  }

}

/**************************************************************//*!
*
* Set Object text properties
*
******************************************************************/

void D4CD_SetTextProperties(D4CD_OBJECT_PTR pObject, D4CD_TEXT_PROPERTIES property) 
{
  
  D4CD_STRING* p_TextBuff = NULL;
  
  if(pObject->pObjFunc->GetTextBuffer)
    p_TextBuff = pObject->pObjFunc->GetTextBuffer((D4CD_OBJECT*)pObject);
 
  if(p_TextBuff)
  {
    if(p_TextBuff->text_properties->all == property.all)
      return;     // There is no change needed
    
    p_TextBuff->text_properties->all = property.all;
    D4CD_InvalidateObject(pObject, D4CD_FALSE);
  }

}

/**************************************************************//*!
*
* Basic drawing
*
******************************************************************/

void D4CD_MoveTo(D4CD_POINT* ppt)
{
    d4cd_curXY = *ppt;    
}

void D4CD_MoveToXY(D4CD_COOR x, D4CD_COOR y)
{
  d4cd_curXY.x = x;
  d4cd_curXY.y = y;  
}


void D4CD_PutCharXY(D4CD_COOR x, D4CD_COOR y, D4CD_CHAR ch)
{
  D4CD_LCD_PutChar(x, y, ch);
  d4cd_curXY.x = x;
  d4cd_curXY.y = y;  
}

void D4CD_PutChar(D4CD_POINT *ppt, D4CD_CHAR ch)
{
  d4cd_curXY = *ppt;
  D4CD_LCD_PutChar(d4cd_curXY.x, d4cd_curXY.y, ch);
    
}


void D4CD_LineTo(D4CD_POINT* ppt, D4CD_CHAR ch)
{
    D4CD_LCD_Line(d4cd_curXY.x, d4cd_curXY.y, ppt->x, ppt->y, ch);
    
    d4cd_curXY = *ppt;    
}

void D4CD_LineToXY(D4CD_COOR x, D4CD_COOR y, D4CD_CHAR ch)
{
   D4CD_LCD_Line(d4cd_curXY.x, d4cd_curXY.y, x, y, ch);
   
   d4cd_curXY.x = x;    
    d4cd_curXY.y = y;
}

void D4CD_Rect(D4CD_POINT* ppt, D4CD_SIZE* psz, D4CD_CHAR ch)
{
    D4CD_LCD_Rect((D4CD_COOR)ppt->x, (D4CD_COOR)ppt->y, (D4CD_COOR)(ppt->x+psz->cx), (D4CD_COOR)(ppt->y+psz->cy), ch);
    
    d4cd_curXY = *ppt;    
}

void D4CD_RectXY(D4CD_COOR x1, D4CD_COOR y1, D4CD_COOR x2, D4CD_COOR y2, D4CD_CHAR ch)
{
    D4CD_LCD_Rect(x1, y1, x2, y2, ch);
    
    d4cd_curXY.x = x1;    
    d4cd_curXY.y = y1;    
}

void D4CD_RectTo(D4CD_POINT* ppt, D4CD_CHAR ch)
{
    D4CD_LCD_Rect(d4cd_curXY.x, d4cd_curXY.y, ppt->x, ppt->y, ch);
    
    d4cd_curXY = *ppt;    
}


void D4CD_RectToXY(D4CD_COOR x, D4CD_COOR y, D4CD_CHAR ch)
{
    D4CD_LCD_Rect(d4cd_curXY.x, d4cd_curXY.y, x, y, ch);
    
    d4cd_curXY.x = x;    
    d4cd_curXY.y = y;    
}


void D4CD_FillRect(D4CD_POINT* ppt, D4CD_SIZE* psz, D4CD_CHAR ch)
{
    D4CD_LCD_Box((D4CD_COOR)ppt->x, (D4CD_COOR)ppt->y, (D4CD_COOR)(ppt->x+psz->cx - 1), (D4CD_COOR)(ppt->y+psz->cy - 1), ch);
    
    d4cd_curXY = *ppt;
}

void D4CD_FillRectXY(D4CD_COOR x1, D4CD_COOR y1, D4CD_COOR x2, D4CD_COOR y2, D4CD_CHAR ch)
{
    D4CD_LCD_Box(x1, y1, x2, y2, ch);
    
    d4cd_curXY.x = x1;    
    d4cd_curXY.y = y1;
}

void D4CD_FillRectTo(D4CD_POINT* ppt, D4CD_CHAR ch)
{
    D4CD_LCD_Box(d4cd_curXY.x, d4cd_curXY.y, ppt->x, ppt->y, ch);
    
    d4cd_curXY = *ppt;
}

void D4CD_FillRectToXY(D4CD_COOR x, D4CD_COOR y, D4CD_CHAR ch)
{
    D4CD_LCD_Box(d4cd_curXY.x, d4cd_curXY.y, x, y, ch);
    
    d4cd_curXY.x = x;    
    d4cd_curXY.y = y;
}


/**************************************************************//*!
*
* Draw text on XY coordination
*
******************************************************************/
void D4CD_DrawSimpleText(D4CD_POINT* ppt, D4CD_CHAR* pText)
{
    D4CD_LCD_PrintStr(ppt->x, ppt->y, pText);
}

/**************************************************************//*!
*
* Draw text on XY coordination
*
******************************************************************/
void D4CD_DrawSimpleTextXY(D4CD_COOR x, D4CD_COOR y, D4CD_CHAR* pText)
{
    D4CD_LCD_PrintStr(x, y, pText);
}


/**************************************************************//*!
*
* Draw text on XY coordination
*
******************************************************************/
void D4CD_DrawText(D4CD_POINT* ppt, D4CD_STRING* buffText)
{
    D4CD_LCD_PrintStr(ppt->x, ppt->y, buffText->pText);
}

/**************************************************************//*!
*
* Draw text on XY coordination
*
******************************************************************/
void D4CD_DrawTextXY(D4CD_COOR x, D4CD_COOR y, D4CD_STRING* buffText)
{
    D4CD_LCD_PrintStr(x, y, buffText->pText);
}


/**************************************************************//*!
*
* Draw text on XY coordination in logic rectangle
*
******************************************************************/
void D4CD_DrawTextRect(D4CD_POINT* ppt, D4CD_SIZE* psz, D4CD_STRING* buffText, D4CD_CHAR bckg)
{
    D4CD_SIZE tmp_size = *psz;
    D4CD_COOR x,y;
    D4CD_TEXT_PROPERTIES tmp_txtProp;
    D4CD_INDEX tmp_txtWidth = D4CD_GetTextLength(buffText->pText);


     
    if(!tmp_size.cx)
    {
      tmp_size.cx = (D4CD_COOR)(buffText->buffSize);
    }
    
    if(!tmp_size.cy)
    {
      tmp_size.cy = 1;
    }
    
    tmp_txtProp.all = buffText->text_properties->all;
    
     
    // Compute the text position in rectangle in Y axis
    switch(tmp_txtProp.bits.bAlignVertic)
    {
      case D4CD_TXT_PRTY_ALIGN_V_BOTTOM:
        y = (D4CD_COOR)(ppt->y + (tmp_size.cy - 1));
        break;
      
      case D4CD_TXT_PRTY_ALIGN_V_CENTER:
        y = (D4CD_COOR)(ppt->y + ((tmp_size.cy - 1) / 2));
        break;
      
      default:
      case D4CD_TXT_PRTY_ALIGN_V_TOP:
        y = ppt->y;
        break;        
    }
    
    // Compute the text position in rectangle in X axis
    switch(tmp_txtProp.bits.bAlignHoriz)
    {
      case D4CD_TXT_PRTY_ALIGN_H_RIGHT:
        x = (D4CD_COOR)(ppt->x + (tmp_size.cx - tmp_txtWidth));
        break;
      
      case D4CD_TXT_PRTY_ALIGN_H_CENTER:
        x = (D4CD_COOR)(ppt->x + ((tmp_size.cx - tmp_txtWidth) / 2));
        break;
      
      default:
      case D4CD_TXT_PRTY_ALIGN_H_LEFT:
        x = ppt->x;
        break;        
    }
    
    // Redraw the rest of rectangle where isn't text for normal (non trasparent) texts

    if(tmp_txtProp.bits.bAlignHoriz != D4CD_TXT_PRTY_ALIGN_H_LEFT)
    {
      // Fill the part of rect in front of the text in full height of rect
      D4CD_LCD_Box(ppt->x, ppt->y, x, (D4CD_COOR)(ppt->y + tmp_size.cy - 1), bckg);
    }
    
    if(tmp_txtProp.bits.bAlignHoriz != D4CD_TXT_PRTY_ALIGN_H_RIGHT)
    {
      // Fill the part of rect behind the text in full height of rect
      D4CD_LCD_Box((D4CD_COOR)(x + tmp_txtWidth - 1), ppt->y, (D4CD_COOR)(ppt->x + tmp_size.cx - 1), (D4CD_COOR)(ppt->y + tmp_size.cy - 1), bckg);
    }
    
    if((psz->cy) && (tmp_size.cy > 1))
    {
      // No auto height is used
      
      if(tmp_txtProp.bits.bAlignVertic != D4CD_TXT_PRTY_ALIGN_V_TOP)
      {
        // Fill the part of rect above the text in lenght of text
        D4CD_LCD_Box(x, ppt->y, (D4CD_COOR)(x + tmp_txtWidth - 1), (D4CD_COOR)(y - 0), bckg);
      }
      
      if(tmp_txtProp.bits.bAlignVertic != D4CD_TXT_PRTY_ALIGN_V_BOTTOM)
      {
        // Fill the part of rect under the text in lenght of text
        D4CD_LCD_Box(x, (D4CD_COOR)(y + 1), (D4CD_COOR)(x + tmp_txtWidth - 1), (D4CD_COOR)(ppt->y + tmp_size.cy - 1), bckg);
      }
      
    }
    
    
    D4CD_LCD_PrintStr(x, y, buffText->pText);
}



/**************************************************************//*!
*
* Draw text on XY coordination in logic rectangle
*
******************************************************************/
void D4CD_DrawTextRectTo(D4CD_POINT* ppt, D4CD_STRING* buffText, D4CD_CHAR bckg)
{
  D4CD_SIZE tmp_size;
  
  tmp_size.cx = (D4CD_COOR)(ppt->x - d4cd_curXY.x);
  tmp_size.cy = (D4CD_COOR)(ppt->y - d4cd_curXY.y);
  
  D4CD_DrawTextRect( &d4cd_curXY, &tmp_size, buffText, bckg);
}

/**************************************************************//*!
*
* Draw text on XY coordination in logic rectangle
*
******************************************************************/
void D4CD_DrawTextRectXY(D4CD_COOR x1, D4CD_COOR y1, D4CD_COOR x2, D4CD_COOR y2, D4CD_STRING* buffText, D4CD_CHAR bckg)
{
  D4CD_POINT tmp_point;
  D4CD_SIZE tmp_size;
  
  tmp_point.x = x1;
  tmp_point.y = y1;
  
  tmp_size.cx = (D4CD_COOR)(x2 - x1);
  tmp_size.cy = (D4CD_COOR)(y2 - y1);
  
  
  D4CD_DrawTextRect( &tmp_point, &tmp_size, buffText, bckg);
}

/**************************************************************//*!
*
* Draw text on XY coordination in logic rectangle
*
******************************************************************/
void D4CD_DrawTextRectToXY(D4CD_COOR x, D4CD_COOR y, D4CD_STRING* buffText, D4CD_CHAR bckg)
{
  D4CD_SIZE tmp_size;
  
  tmp_size.cx = (D4CD_COOR)(x - d4cd_curXY.x);
  tmp_size.cy = (D4CD_COOR)(y - d4cd_curXY.y);
  
  
  D4CD_DrawTextRect( &d4cd_curXY, &tmp_size, buffText, bckg);
}


/**************************************************************//*!
*
* Draw text on XY coordination in logic rectangle
*
******************************************************************/
void D4CD_DrawTextLine(D4CD_POINT* ppt, D4CD_COOR len, D4CD_STRING* buffText, D4CD_CHAR bckg)
{
    D4CD_COOR x;
    D4CD_TEXT_PROPERTIES tmp_txtProp;
    D4CD_INDEX tmp_txtWidth = D4CD_GetTextLength(buffText->pText);


     
    if(!len)
      len = (D4CD_COOR)(buffText->buffSize);
    
    tmp_txtProp.all = buffText->text_properties->all;
    
     
    // Compute the text position in rectangle in X axis
    switch(tmp_txtProp.bits.bAlignHoriz)
    {
      case D4CD_TXT_PRTY_ALIGN_H_RIGHT:
        x = (D4CD_COOR)(ppt->x + (len - tmp_txtWidth));
        break;
      
      case D4CD_TXT_PRTY_ALIGN_H_CENTER:
        x = (D4CD_COOR)(ppt->x + ((len - tmp_txtWidth) / 2));
        break;
      
      default:
      case D4CD_TXT_PRTY_ALIGN_H_LEFT:
        x = ppt->x;
        break;        
    }
    
    // Redraw the rest of rectangle where isn't text for normal (non trasparent) texts

    if(tmp_txtProp.bits.bAlignHoriz != D4CD_TXT_PRTY_ALIGN_H_LEFT)
    {
      // Fill the part of rect in front of the text in full height of rect
      D4CD_LCD_Box(ppt->x, ppt->y, x, ppt->y, bckg);
    }
    
    if(tmp_txtProp.bits.bAlignHoriz != D4CD_TXT_PRTY_ALIGN_H_RIGHT)
    {
      // Fill the part of rect behind the text in full height of rect
      D4CD_LCD_Box((D4CD_COOR)(x + tmp_txtWidth - 1), ppt->y, (D4CD_COOR)(ppt->x + len - 1), ppt->y, bckg);
    }
    
    D4CD_LCD_PrintStr(x, ppt->y, buffText->pText);
}



/**************************************************************//*!
*
* Draw text on XY coordination in logic rectangle
*
******************************************************************/
void D4CD_DrawTextLineTo(D4CD_POINT* ppt, D4CD_STRING* buffText, D4CD_CHAR bckg)
{
  D4CD_DrawTextLine( &d4cd_curXY, (D4CD_COOR)(ppt->y - d4cd_curXY.y), buffText, bckg);
}

/**************************************************************//*!
*
* Draw text on XY coordination in logic rectangle
*
******************************************************************/
void D4CD_DrawTextLineXY(D4CD_COOR x1, D4CD_COOR y1, D4CD_COOR x2, D4CD_STRING* buffText, D4CD_CHAR bckg)
{
  D4CD_POINT tmp_point;
  
  tmp_point.x = x1;
  tmp_point.y = y1;
  
  D4CD_DrawTextLine( &tmp_point, (D4CD_COOR)(x2 - x1), buffText, bckg);
}

/**************************************************************//*!
*
* Draw text on XY coordination in logic rectangle
*
******************************************************************/
void D4CD_DrawTextLineToXY(D4CD_COOR x, D4CD_STRING* buffText, D4CD_CHAR bckg)
{
  D4CD_DrawTextLine( &d4cd_curXY, (D4CD_COOR)(x - d4cd_curXY.x), buffText, bckg);
}

/**************************************************************//*!
*
* Set cursor behaviour and coordination
*
******************************************************************/
void D4CD_SetCursor(D4CD_POINT* ppt, D4CD_BOOL visible, D4CD_BOOL blink)
{
  d4cd_cursor.pos = *ppt;
  d4cd_cursor.state.bVisible = visible;
  d4cd_cursor.state.bBlink = blink;
  d4cd_cursor.state.bUpdate = 1;
  
}