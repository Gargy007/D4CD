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
* @file      d4cd_screen.c
*
* @author    Petr Gargulak
* 
* @version   0.0.4.0
* 
* @date      Mar-11-2011
* 
* @brief     D4CD driver screen functions c file 
*
******************************************************************************/

#include "d4cd.h"
#include "d4cd_private.h"



static void D4CD_DrawScreenNC(D4CD_SCREEN* pScreen, D4CD_BOOL active);
static void D4CD_ChangeScreen(D4CD_SCREEN* pNewScreen, D4CD_SCREEN* pOldScreen);
static Byte D4CD_GetObjectCount(D4CD_SCREEN* pScreen);
static Byte D4CD_FindObject(D4CD_SCREEN* pScreen, D4CD_OBJECT_PTR pObject);
/**************************************************************//*!
*
* Get active screen
*
******************************************************************/

D4CD_SCREEN* D4CD_GetActiveScreen(void)
{
    // no screen yet shown
    if(d4cd_screenHistoryIndex <= 0)
        return NULL;
    
    return d4cd_screenHistory[d4cd_screenHistoryIndex - 1];
}


/**************************************************************//*!
*
* Change the active screen
*
******************************************************************/
static void D4CD_ChangeScreen(D4CD_SCREEN* pNewScreen, D4CD_SCREEN* pOldScreen)
{    
    D4CD_SCREEN_DATA* pData;
    D4CD_MESSAGE tmp_msg;
    Byte ix;
    
    if(pNewScreen == NULL)
      return;
    
    if(pOldScreen == pNewScreen)
      return;
    
    tmp_msg.pScreen = pOldScreen;
    
    if(pOldScreen != NULL)
    {
      tmp_msg.nMsgId = D4CD_MSG_KILLFOCUS;
      tmp_msg.pObject = (D4CD_OBJECT*) pOldScreen->pObjects[pOldScreen->pData->focusedObjIx];
      D4CD_SendMessage(&tmp_msg);
              
      // Draw NC screen area as an inactivate
      D4CD_DrawScreenNC(pOldScreen, D4CD_FALSE);
      
      // call de-activate event
      if(pOldScreen->OnDeactivate != NULL)
          pOldScreen->OnDeactivate();       
    }

    // invalidate the new screen (global complete redraw, not individual objects)
    D4CD_InvalidateScreen(pNewScreen, D4CD_TRUE);
    
    // if this is the first time activating 
    pData = pNewScreen->pData;
    
    // load the screen user char Table
    D4CD_LoadUserCharTable(pNewScreen->pUserTable);
    
    // initialize the pointers on this screen in all screen objects
    ix = 0;
    tmp_msg.pScreen = pNewScreen;
    tmp_msg.nMsgId = D4CD_MSG_ONINIT;
    
    while(pNewScreen->pObjects[ix] != NULL) {
      
      *(pNewScreen->pObjects[ix]->pScreen) = pNewScreen;
      
      
      tmp_msg.pObject = (D4CD_OBJECT*)pNewScreen->pObjects[ix];
      
      if(!(tmp_msg.pObject->flags->bits.bInitDone))
      {            
          
          // send the ON INIT message
          D4CD_SendMessage(&tmp_msg);
          tmp_msg.pObject->flags->bits.bInitDone = 1;            
      }
      
      ix++;
      
    }
    
    
    
    if(!pData->flags.bits.bInitDone)
    {
        pData->flags.bits.bInitDone = 1;
        
        // user's screen initialization
        if(pNewScreen->OnInit)
            pNewScreen->OnInit();
        
        D4CD_FocusNextObject(pNewScreen, D4CD_TRUE);
    }else
    {
      tmp_msg.pScreen = pNewScreen;
      tmp_msg.nMsgId = D4CD_MSG_SETFOCUS;
      tmp_msg.pObject = (D4CD_OBJECT*) pNewScreen->pObjects[pData->focusedObjIx];
      D4CD_SendMessage(&tmp_msg);          
    }
    
    // inform the screen it has been activated
    if(pNewScreen->OnActivate)
        pNewScreen->OnActivate();
    
    D4CD_ClearKeysBuffer();
}


/**************************************************************//*!
*
* Enter new screen, remember the history if not only replacing
*
******************************************************************/
void D4CD_ActivateScreen(D4CD_SCREEN* pNewScreen, Byte bReplaceCurrent)
{    
  D4CD_SCREEN* pOldScreen;

  // can not activate screen (no space left in history array)
  if(d4cd_screenHistoryIndex >= D4CD_SCREEN_HISTORY && !bReplaceCurrent)
      return ;
  
  // cancel key capture
  D4CD_CaptureKeys(NULL);

  // deactivate current screen (if any)
  pOldScreen = D4CD_GetActiveScreen();

  if(pOldScreen != NULL)
  {
      // replacing this screen in history?
      if(bReplaceCurrent)
          d4cd_screenHistoryIndex--; // note that we are sure this is >0
  }

  // set the new screen as the active one
  d4cd_screenHistory[d4cd_screenHistoryIndex] = pNewScreen;
  d4cd_screenHistoryIndex++;

  D4CD_ChangeScreen(pNewScreen, pOldScreen);    
}

 

/**************************************************************//*!
*
* Return to previous screen recorded in the history
*
******************************************************************/
void D4CD_EscapeScreen(void)
{
    D4CD_SCREEN* pOldScreen;
    
    // can not escape current screen, it is the top one 
    if(d4cd_screenHistoryIndex <= 1)
      return;
    
    // cancel key capture
    D4CD_CaptureKeys(NULL);

    pOldScreen = D4CD_GetActiveScreen();
    
    // pop the history stack
    if(d4cd_screenHistoryIndex > 1)
      d4cd_screenHistoryIndex--;
    
    D4CD_ChangeScreen(D4CD_GetActiveScreen(), pOldScreen);

}


/**************************************************************//*!
*
* Return to previous screen recorded in the history
*
******************************************************************/
void D4CD_EscapeToBaseScreen(void)
{
    D4CD_SCREEN* pOldScreen;

    // can not escape current screen, it is the top one 
    if(d4cd_screenHistoryIndex == 0)
        return;
    
    // cancel key capture
    D4CD_CaptureKeys(NULL);

    
    // I can be sure this is not NULL
    pOldScreen = D4CD_GetActiveScreen();

    // pop the history stack
    d4cd_screenHistoryIndex = 1;    
    
    D4CD_ChangeScreen(D4CD_GetActiveScreen(), pOldScreen);
}

/**************************************************************//*!
*
* Return to previous screen recorded in the history
*
******************************************************************/
void D4CD_ClearScreenHistory(void)
{
  // clear the history stack
  d4cd_screenHistoryIndex = 0;  
}

/**************************************************************//*!
*
* Mark all object on screen as "redraw pennding"
*
******************************************************************/

void D4CD_InvalidateScreen(D4CD_SCREEN* pScreen, Byte bComplete)
{
    D4CD_OBJECT* pObj;
    Byte i;

    if(bComplete)
    {
        pScreen->pData->flags.bits.bCompleteRedraw = 1;
    }
    else
    {
        for(i=0; pScreen->pObjects[i] != NULL; i++)
        {
            pObj = (D4CD_OBJECT*) pScreen->pObjects[i];
            pObj->flags->bits.bRedraw = 1;
        }
    }
}

/**************************************************************//*!
*
* Gety the count of object in given screen
*
******************************************************************/

static Byte D4CD_GetObjectCount(D4CD_SCREEN* pScreen)
{
    Byte ix = 0;
    
    while(pScreen->pObjects[ix] != NULL)
        ix++;
    
    return ix;
}

/**************************************************************//*!
*
* Get focused object in a given screen
*
******************************************************************/

D4CD_OBJECT_PTR D4CD_GetFocusedObject(D4CD_SCREEN* pScreen)
{
    if(!pScreen)
        return NULL;
           
    // check if index is not out of range?                
    if(pScreen->pData->focusedObjIx > D4CD_GetObjectCount(pScreen))
      return NULL;

    // just return the object on a focus index
    return pScreen->pObjects[pScreen->pData->focusedObjIx];
}


/**************************************************************//*!
*
* Get focused object in a given screen
*
******************************************************************/

void D4CD_ResetObjectFocus(D4CD_SCREEN* pScreen)
{
    if(!pScreen)
        return;
    
    pScreen->pData->focusedObjIx = 0;
    D4CD_FocusNextObject(pScreen, D4CD_TRUE);
}



 /**************************************************************//*!
*
* Focus next object in given screen
*
******************************************************************/

void D4CD_FocusNextObject(D4CD_SCREEN* pScreen, Byte bInitialSearch)
{
    D4CD_SCREEN_DATA* pData = pScreen->pData;
    const D4CD_OBJECT* const* pObjects = pScreen->pObjects;
    D4CD_MESSAGE tmp_msg;
    Byte ix = pData->focusedObjIx;
    
    
    // sanity check of list of objects - contains Screen any object?
    if(pObjects[ix] == NULL)
      return;

    // currently focused object already has a tabstop    
    if(bInitialSearch)
    {
      if(pObjects[ix]->flags->mergedBits.bPossFocus == 0x07)
      {
        tmp_msg.pScreen = pScreen;
        tmp_msg.nMsgId = D4CD_MSG_SETFOCUS;
        tmp_msg.pObject = (D4CD_OBJECT*) pObjects[ix];
        D4CD_SendMessage(&tmp_msg);
        
        return;
      }
    }
    
    do
    {
      // get next object
      ix++;
      
      // wrap around in the array if end is reached
      if(pObjects[ix] == NULL)
          ix = 0;
      
      // visible enabled object with focus enabled?
      if(pObjects[ix]->flags->mergedBits.bPossFocus == 0x07)
          break;
      
      // avoid endless loop if no focused object can be found
    } while(ix != pData->focusedObjIx);
    
    if(ix != pData->focusedObjIx)
    {        
      // invalidate object which is loosing focus
      pObjects[pData->focusedObjIx]->flags->bits.bRedraw = 1;
      
      // prepare message KILLFOCUS
      tmp_msg.pScreen = pScreen;
      tmp_msg.nMsgId = D4CD_MSG_KILLFOCUS;
      tmp_msg.pObject = (D4CD_OBJECT*) pObjects[pData->focusedObjIx];
      D4CD_SendMessage(&tmp_msg);
      
      
      // invalidate object which is getting focus
      pObjects[ix]->flags->bits.bRedraw = 1;
      
      // move the focus
      pData->focusedObjIx = ix;
      
      // prepare message
      tmp_msg.pScreen = pScreen;
      tmp_msg.nMsgId = D4CD_MSG_SETFOCUS;
      tmp_msg.pObject = (D4CD_OBJECT*) pObjects[ix];
      D4CD_SendMessage(&tmp_msg);
    }
}

/**************************************************************//*!
*
* Focus previous object in given screen
*
******************************************************************/

void D4CD_FocusPrevObject(D4CD_SCREEN* pScreen)
{
    D4CD_SCREEN_DATA* pData = pScreen->pData;
    const D4CD_OBJECT* const* pObjects = pScreen->pObjects;
    D4CD_MESSAGE tmp_msg;
    Byte ix = pData->focusedObjIx;
    
    // sanity check of list of objects - contains Screen any object?
    if(pObjects[ix] == NULL)
      return;
    
    do
    {
        // wrap around in the array if begin is reached
        if(ix == 0)
        {           
            // assuming at least one object in the screen always
            ix = D4CD_GetObjectCount(pScreen);
            // but better be robust
            if(ix) ix--;
        }
        else
        {
            // just get previous object
            ix--;
        }
        
        // object with focus enabled?
        if(pObjects[ix]->flags->mergedBits.bPossFocus == 0x07)
            break;
        
      // avoid endless loop if no focused object can be found
    } while(ix != pData->focusedObjIx);
    
    if(ix != pData->focusedObjIx)
    {        
      // invalidate object which is loosing focus
      pObjects[pData->focusedObjIx]->flags->bits.bRedraw = 1;
      
      // prepare message KILLFOCUS
      tmp_msg.pScreen = pScreen;
      tmp_msg.nMsgId = D4CD_MSG_KILLFOCUS;
      tmp_msg.pObject = (D4CD_OBJECT*) pObjects[pData->focusedObjIx];
      D4CD_SendMessage(&tmp_msg);
      
      
      // invalidate object which is getting focus
      pObjects[ix]->flags->bits.bRedraw = 1;
      
      // move the focus
      pData->focusedObjIx = ix;
      
      // prepare message
      tmp_msg.pScreen = pScreen;
      tmp_msg.nMsgId = D4CD_MSG_SETFOCUS;
      tmp_msg.pObject = (D4CD_OBJECT*) pObjects[ix];
      D4CD_SendMessage(&tmp_msg);
    }
}

/**************************************************************//*!
*
* Find object's index
*
******************************************************************/

static Byte D4CD_FindObject(D4CD_SCREEN* pScreen, D4CD_OBJECT_PTR pObject)
{
    Byte ix;
    
    for(ix = 0; pScreen->pObjects[ix] != NULL; ix++)
        if(pScreen->pObjects[ix] == pObject)
            return ix;
        
    // not found
    return 0xff;
}

/**************************************************************//*!
*
* Focus selected object in given screen
*
******************************************************************/

void D4CD_FocusSet(D4CD_SCREEN* pScreen, D4CD_OBJECT_PTR pObject)
{
    D4CD_SCREEN_DATA* pData = pScreen->pData;
    const D4CD_OBJECT* const* pObjects = pScreen->pObjects;
    Byte ix = pData->focusedObjIx;
    Byte i;
    
    if(pScreen == NULL)
      return;
    
    // check if object is really item of the current screen
    i = D4CD_FindObject(pScreen, pObject);
    if(i == 0xff)
      return;
    
    if(i == ix) // is selected object same as focused?
      return;
  
    if(!pObject->flags->bits.bTabStop  || !pObject->flags->bits.bEnabled) // is this object selectable?
      return;
    
    // invalidate object which is loosing focus
    pObjects[ix]->flags->bits.bRedraw = 1;
    
    // prepare message
    d4cd_msg.pScreen = pScreen;
    d4cd_msg.nMsgId = D4CD_MSG_KILLFOCUS;
    d4cd_msg.pObject = (D4CD_OBJECT*) pObjects[ix];
    D4CD_SendMessage(&d4cd_msg);
    
    // invalidate object which is getting focus
    pObjects[i]->flags->bits.bRedraw = 1;
    
    // move the focus
    pData->focusedObjIx = i;
    
    // prepare message
    d4cd_msg.pScreen = pScreen;
    d4cd_msg.nMsgId = D4CD_MSG_SETFOCUS;
    d4cd_msg.pObject = (D4CD_OBJECT*)  pObjects[i];
    D4CD_SendMessage(&d4cd_msg);
        
}






/**************************************************************//*!
*
* Redraw non client screen area - Header, background, title, etc.
*
******************************************************************/

static void D4CD_DrawScreenNC(D4CD_SCREEN* pScreen, D4CD_BOOL active)
{
  
  if(active)
  {    
    if(pScreen->flags.bits.bBackground)
    {    
      D4CD_FillRect(&pScreen->position, &pScreen->size, D4CD_SCR_BCKG_CHAR);
    }
    
    if(pScreen->flags.bits.bOutline)
    {
      D4CD_POINT tmp_point = pScreen->position;
      
      // draw left top corner
      D4CD_PutChar(&tmp_point, D4CD_SCR_OUTLINE_CORNER_CHAR);
      
      // draw vertical left line
      tmp_point.y++;
      D4CD_MoveTo(&tmp_point);
      tmp_point.y = (D4CD_COOR)(pScreen->position.y + pScreen->size.cy - 2);
      D4CD_LineTo(&tmp_point, D4CD_SCR_OUTLINE_COLUMN_CHAR);
      
      // draw left bottom corner
      tmp_point.y++;
      D4CD_PutChar(&tmp_point, D4CD_SCR_OUTLINE_CORNER_CHAR);          
        
      // draw horizontal bottom line
      tmp_point.x++;
      D4CD_MoveTo(&tmp_point);
      tmp_point.x = (D4CD_COOR)(pScreen->position.x + pScreen->size.cx - 2);
      D4CD_LineTo(&tmp_point, D4CD_SCR_OUTLINE_ROW_CHAR);
      
      // draw right bottom corner
      tmp_point.x++;
      D4CD_PutChar(&tmp_point, D4CD_SCR_OUTLINE_CORNER_CHAR);          
      
      // draw vertical right line
      tmp_point.y--;
      D4CD_MoveTo(&tmp_point);
      tmp_point.y = (D4CD_COOR)(pScreen->position.y + 1);
      D4CD_LineTo(&tmp_point, D4CD_SCR_OUTLINE_COLUMN_CHAR);
      
      // draw right top corner
      tmp_point.y--;
      D4CD_PutChar(&tmp_point, D4CD_SCR_OUTLINE_CORNER_CHAR);          
      
      // draw horizontal top line
      tmp_point.x--;
      D4CD_MoveTo(&tmp_point);
      tmp_point.x = (D4CD_COOR)(pScreen->position.x + 1);
      D4CD_LineTo(&tmp_point, D4CD_SCR_OUTLINE_ROW_CHAR);
    }
  }
 
  if(pScreen->textBuff.pText)
  {        
    D4CD_DrawTextLineXY((D4CD_COOR)(pScreen->position.x + 1), pScreen->position.y, (D4CD_COOR)(pScreen->position.x + pScreen->size.cx - 1), &pScreen->textBuff, D4CD_SCR_OUTLINE_ROW_CHAR);  
  }
    
}
 
/**************************************************************//*!
*
* Redraw all objects that need it on given screen
*
******************************************************************/
typedef union
{
  D4CD_BIT_FIELD all;
  struct 
  {
    unsigned  bCompleteRedraw  :1;
    unsigned  bTimeTick        :1;
    unsigned  bBlinkChange     :1;
  }bits;
}D4CD_TMP_REDRAWFLAGS;

void D4CD_RedrawScreen(D4CD_SCREEN* pScreen)
{
    D4CD_SCREEN_DATA* pData = pScreen->pData;
    D4CD_OBJECT* pFocus = (D4CD_OBJECT*) D4CD_GetFocusedObject(pScreen);
    D4CD_OBJECT* pObj;
    Byte i;
    D4CD_TMP_REDRAWFLAGS tmp_flags;
    
    tmp_flags.all = 0;
    
    if(pData->flags.bits.bCompleteRedraw)
    {
      tmp_flags.bits.bCompleteRedraw = 1;
      pData->flags.bits.bCompleteRedraw = 0;
    }
    
    if(d4cd_systemFlags.bits.bTimeTick)
    {
      tmp_flags.bits.bTimeTick = 1;
      d4cd_systemFlags.bits.bTimeTick = 0;
    
      if(d4cd_blink.state.bits.bChange)
      {
        tmp_flags.bits.bBlinkChange = 1;
        d4cd_blink.state.bits.bChange = 0;
      }
    }
    
    

    
    // prepare message
    d4cd_msg.pScreen = pScreen;
    
    // redraw screen non client area if necessary
    if(tmp_flags.bits.bCompleteRedraw)
    {
      D4CD_DrawScreenNC(pScreen, D4CD_TRUE);
    }
    
    
    // redraw objects   
    for(i=0; pScreen->pObjects[i] != NULL; i++)
    {
        pObj = (D4CD_OBJECT*) pScreen->pObjects[i];
        
        if(!(pObj->flags->bits.bVisible))
          continue;
        
        d4cd_msg.pObject = pObj;
        /*
        if(!(pObj->flags->bits.bInitDone))
        {            
            d4cd_msg.nMsgId = D4CD_MSG_ONINIT;
            // send the ON INIT message
            D4CD_SendMessage(&d4cd_msg);
            pObj->flags->bits.bInitDone = 1;            
        }
        */
        if(tmp_flags.bits.bTimeTick)
        {
          d4cd_msg.nMsgId = D4CD_MSG_TIMETICK;
          // send the TIMETICK message
          d4cd_msg.prm.blink = d4cd_blink.state;
          d4cd_msg.prm.blink.bits.bChange = tmp_flags.bits.bBlinkChange;
          D4CD_SendMessage(&d4cd_msg);  
        }
        
        d4cd_msg.prm.draw.all = 0;
        
        
        /* forced complete redraw of all objects or object wants to be completely redrawn*/
        if((tmp_flags.bits.bCompleteRedraw) || (pObj->flags->bits.bRedrawC))
        {
            d4cd_msg.prm.draw.bits.bComplete = 1;
        }
        else
        {
            /* object does not need to redraw */
            if(!pObj->flags->bits.bRedraw)
                continue;
        }

        
       
        // draw special?
        if((pObj == pFocus) && (pObj->flags->bits.bTabStop))
        {
            d4cd_msg.prm.draw.bits.bFocused = 1;

            if(pObj == d4cd_pKeysCapturer)
                d4cd_msg.prm.draw.bits.bCapturing = 1;
        }
       
        if((d4cd_cursor.state.bVisible) && (!d4cd_cursor.state.bUpdate))
        {
          D4CD_LCD_HideCursor();
          d4cd_cursor.state.bUpdate = 1;
          
        }
       
        // will draw now            
        pObj->flags->bits.bRedraw = 0;
        pObj->flags->bits.bRedrawC = 0;       
        
        // send the DRAW message
        d4cd_msg.nMsgId = D4CD_MSG_DRAW;
        D4CD_SendMessage(&d4cd_msg);
        // send the DRAWDONE message
        d4cd_msg.nMsgId = D4CD_MSG_DRAWDONE;
        D4CD_SendMessage(&d4cd_msg);
        
        
        
    }
    
    
    if(d4cd_cursor.state.bUpdate)
    {
      d4cd_cursor.state.bUpdate = 0;
      D4CD_LCD_SetCursor(&d4cd_cursor);
    }
    
}


/**************************************************************//*!
*
* Convert Client point into the global screen point
*
******************************************************************/

D4CD_POINT D4CD_GetClientToScreenPoint(D4CD_SCREEN* pScreen, D4CD_POINT* nClientPoint)
{
  D4CD_POINT tmp_point;
  
  if(pScreen == NULL)
  {
    return *nClientPoint;  
  }
  
  tmp_point.x = (D4CD_COOR)(pScreen->position.x + nClientPoint->x);
  tmp_point.y = (D4CD_COOR)(pScreen->position.y + nClientPoint->y);
  
  if(pScreen->flags.bits.bOutline)
    tmp_point.x++;
  
  if(pScreen->flags.bits.bOutline || pScreen->textBuff.pText)
    tmp_point.y++;
  
  return tmp_point;
}

/**************************************************************//*!
*
* Convert global screen point into the client point
*
******************************************************************/

D4CD_POINT D4CD_GetScreenToClientPoint(D4CD_SCREEN* pScreen, D4CD_POINT* nScreenPoint)
{
  D4CD_POINT tmp_point;
  
  if(pScreen == NULL)
  {
    return *nScreenPoint;  
  }
  
  tmp_point.x = (D4CD_COOR)(nScreenPoint->x - pScreen->position.x);
  tmp_point.y = (D4CD_COOR)(nScreenPoint->y - pScreen->position.y);
  
  if(pScreen->flags.bits.bOutline)
    tmp_point.x--;
  
  if(pScreen->flags.bits.bOutline || pScreen->textBuff.pText)
    tmp_point.y--;
  
  return tmp_point;
}