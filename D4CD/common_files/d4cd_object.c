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
* @file      d4cd_object.c
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Mar-11-2011
* 
* @brief     D4CD driver object function c file 
*
******************************************************************************/

#include "d4cd.h"
#include "d4cd_private.h"

/**************************************************************//*!
*
* Global variables
*
******************************************************************/

/**************************************************************//*!
*
* invalidate object
*
******************************************************************/

void D4CD_InvalidateObject(D4CD_OBJECT_PTR pObject, Byte bComplete)
{
    pObject->flags->bits.bRedraw = 1;
    
    if(bComplete)
        pObject->flags->bits.bRedrawC = 1;
}

/**************************************************************//*!
*
* Show / Hide object
*
******************************************************************/

void D4CD_ShowObject(D4CD_OBJECT_PTR pObject, Byte bShow)
{
    if(bShow)
    {
        pObject->flags->bits.bVisible = 1;
        pObject->flags->bits.bRedraw = 1;
        pObject->flags->bits.bRedrawC = 1;
    }
    else
    {
        pObject->flags->bits.bVisible = 0;
        
        if(!bShow)
        {
          if(pObject == D4CD_GetFocusedObject(D4CD_GetActiveScreen()))
            D4CD_ResetObjectFocus(D4CD_GetActiveScreen());
        }
        
        D4CD_InvalidateScreen(D4CD_GetActiveScreen(), D4CD_TRUE);        
    }
}

/**************************************************************//*!
*
* Get the user pointer / data of of object
*
******************************************************************/

void* D4CD_GetUserPointer(D4CD_OBJECT *pThis)
{
  return pThis->userPointer;
}

/**************************************************************//*!
*
* Capturing keys
*
******************************************************************/

void D4CD_CaptureKeys(D4CD_OBJECT_PTR  pObj)
{
    // NOTE: we need to send message, but we may just be in the middle of
    //       message processing (very likely). This may cause problem with
    //       the global d4cd_msg object as we are changing it...
    //       We better use the temporary memory to instanitate the message
    
    D4CD_MESSAGE* pMsg = (D4CD_MESSAGE*) d4cd_scratchPad;

    pMsg->pScreen = D4CD_GetActiveScreen();
    
    if(pMsg->pScreen == NULL)
    {
      d4cd_pKeysCapturer = NULL;
      return;
    }
    
    if(d4cd_pKeysCapturer)
    {
        pMsg->nMsgId = D4CD_MSG_KILLCAPTURE;
        pMsg->pObject = d4cd_pKeysCapturer;
        D4CD_SendMessage(pMsg);
        
        d4cd_pKeysCapturer->flags->bits.bRedraw = 1;
    }
    
    d4cd_pKeysCapturer = (D4CD_OBJECT*) pObj;

    if(pObj != NULL)
    {
        pMsg->nMsgId = D4CD_MSG_SETCAPTURE;
        pMsg->pObject = d4cd_pKeysCapturer;
        D4CD_SendMessage(pMsg);
        
        pObj->flags->bits.bRedraw = 1;
    }
}

/**************************************************************//*!
*
* Get Object that is capturing keys
*
******************************************************************/


D4CD_OBJECT* D4CD_GetCapturedObject(void)
{
  return d4cd_pKeysCapturer;
}


/**************************************************************//*!
*
* enable or diasble tab stop for an OBJECT
*
******************************************************************/

void D4CD_EnableTabStop(D4CD_OBJECT_PTR pObj, Byte bEnable)
{
    pObj->flags->bits.bTabStop = bEnable ? 1 : 0;
    
    if(!bEnable)
    {
      if(pObj == D4CD_GetFocusedObject(D4CD_GetActiveScreen()))
        D4CD_ResetObjectFocus(D4CD_GetActiveScreen());
    }
}

/**************************************************************//*!
*
* enable or diasble  an OBJECT
*
******************************************************************/

void D4CD_EnableObject(D4CD_OBJECT_PTR pObj, Byte bEnable)
{
  if((pObj->flags->bits.bEnabled && !bEnable) || (!pObj->flags->bits.bEnabled && bEnable))
  {      
    pObj->flags->bits.bEnabled = bEnable ? 1 : 0;
    
    if(!bEnable)
    {
      if(pObj == D4CD_GetFocusedObject(D4CD_GetActiveScreen()))
        D4CD_ResetObjectFocus(D4CD_GetActiveScreen());
    }
    
    pObj->flags->bits.bRedrawC = 1;
  }
}


/**************************************************************//*!
*
* defualt message handler for all OBJECTS
*
******************************************************************/

void D4CD_ObjOnMessage(D4CD_MESSAGE* pMsg)
{
    D4CD_UNUSED(pMsg); 
}