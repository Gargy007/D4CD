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
* @file      d4cd_time.c
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Feb-14-2011
* 
* @brief     D4CD driver time object c file 
*
******************************************************************************/

#include "d4cd.h"
#include "d4cd_private.h"

//Internal API
void D4CD_TimeOnMessage(D4CD_MESSAGE* pMsg);
static void D4CD_TimeTimeTick(D4CD_MESSAGE* pMsg);
static Byte D4CD_TimeGetCurrSetVal(D4CD_TIME_DATA* pData);

const D4CD_OBJECT_SYS_FUNCTION d4cd_timeSysFunc = 
{
  D4CD_OBJECT_TIME,
  D4CD_TimeOnMessage,
  NULL
};

typedef struct 
{
    D4CD_POINT Position;
    D4CD_SIZE Size;
} D4CD_TIME_TMP_VAL;

#define _calc (*((D4CD_TIME_TMP_VAL*)d4cd_scratchPad))

    
static void D4CD_TimeValue2Coor(D4CD_OBJECT* pThis)
{
  D4CD_TIME* pTime = D4CD_GET_TIME(pThis);
  D4CD_TIME_DATA* pData = D4CD_GET_TIME_DATA(pThis);
  
  
  _calc.Size = pTime->scrSize;
  _calc.Position = D4CD_GetClientToScreenPoint(*pThis->pScreen, &pTime->scrPos);
  
#if  D4CD_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities
        
    //Check Size of time in axis X   
    if(!_calc.Size.cx)
    {
      D4CD_COOR tmp_size = 0; // focus char
      
#if (D4CD_TIME_SEL_CHAR == D4CD_TRUE)
        tmp_size += 1;
#endif      
      if(pData->status.bits.bVisibleSec)
        tmp_size += 3;
      
      if(pData->status.bits.bVisibleMin)
        tmp_size += 3;
      
      if(pData->status.bits.bVisibleHour)
        tmp_size += 2;
      
      _calc.Size.cx = tmp_size;
    }
    
    //Check Size of time in axis Y
    
    if(!_calc.Size.cy)
    {
      _calc.Size.cy++;
    }
    
#endif
  
}

/*******************************************************
*
* TIME Drawing routine
*
*******************************************************/

static void D4CD_TimeOnDraw(D4CD_MESSAGE* pMsg)
{
    D4CD_TIME* pTime = D4CD_GET_TIME(pMsg->pObject);
    D4CD_TIME_DATA* pData = D4CD_GET_TIME_DATA(pMsg->pObject);    
    D4CD_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4CD_CHAR tmp_str[9];
    Byte tmp_len = 0;
    Byte tmp_currSetVal = D4CD_TimeGetCurrSetVal(pData);
    Byte tmp_timeVal;
    D4CD_TimeValue2Coor(pMsg->pObject);
    
    if(draw.bits.bComplete)
    {
      D4CD_FillRect(&_calc.Position, &_calc.Size, ' ');
    }
    
#if (D4CD_TIME_SEL_CHAR == D4CD_TRUE)
      if(draw.bits.bFocused)
        D4CD_PutChar(&_calc.Position, D4CD_TIME_SEL_FOCUS_CHAR);
      else
        D4CD_PutChar(&_calc.Position, D4CD_TIME_SEL_NORMAL_CHAR);
      
      _calc.Position.x++;      
#endif    
    
    if(pData->status.bits.bVisibleHour)
    {
      if(tmp_currSetVal != 0xff)
        tmp_timeVal = pData->new_time.hours;
      else
        tmp_timeVal = pData->time.hours;
      
      if((tmp_currSetVal == 0) && D4CD_GetBlink())
      {
        tmp_str[0] = ' ';
        tmp_str[1] = ' ';
      } else
      {
        if((pData->status.bits.bMode12_24) && (pData->time.hours >= 12))
          D4CD_SprintDec99((Byte)(tmp_timeVal - 12), tmp_str);  
        else
          D4CD_SprintDec99(tmp_timeVal, tmp_str);  
      }
      tmp_len += 2;
    }
    
    if(pData->status.bits.bVisibleMin)
    {
      if(tmp_len)
      {
        if(D4CD_GetBlink())
          tmp_str[tmp_len++] = ' ';
        else
          tmp_str[tmp_len++] = D4CD_TIME_HOUR_MIN_CHAR;
      }
      
      if(tmp_currSetVal != 0xff)
        tmp_timeVal = pData->new_time.minutes;
      else
        tmp_timeVal = pData->time.minutes;
      
      if((tmp_currSetVal == 1) && D4CD_GetBlink())
      {
        tmp_str[tmp_len] = ' ';
        tmp_str[tmp_len + 1] = ' ';
      } else
      {
        D4CD_SprintDec99(tmp_timeVal, &tmp_str[tmp_len]);  
      }
      tmp_len += 2;
    }
    
    if(pData->status.bits.bVisibleSec)
    {
      if(tmp_len)
        tmp_str[tmp_len++] = D4CD_TIME_MIN_SEC_CHAR;
      
      if(tmp_currSetVal != 0xff)
        tmp_timeVal = pData->new_time.seconds;
      else
        tmp_timeVal = pData->time.seconds;
      
      if((tmp_currSetVal == 2) && D4CD_GetBlink())
      {
        tmp_str[tmp_len] = ' ';
        tmp_str[tmp_len + 1] = ' ';
      } else
      {
        D4CD_SprintDec99(tmp_timeVal, &tmp_str[tmp_len]);  
      }
      tmp_len += 2;
    }
    
    tmp_str[tmp_len] = 0;
       
    D4CD_DrawSimpleText(&_calc.Position, tmp_str);
    
    
    
}

/*******************************************************
*
* TIME key handling routine
*
*******************************************************/

static Byte D4CD_TimeGetCurrSetVal(D4CD_TIME_DATA* pData)
{
  if(!pData->status.mergedBits.bSet)
    return 0xff;
  
  if(!pData->status.bits.bSetMin)
    return 0;
    
  if(!pData->status.bits.bSetSec)
    return 1;
  
  return 2;
}

static void D4CD_TimeOnKeyUp(D4CD_MESSAGE* pMsg)    
{
  D4CD_TIME* pTime = D4CD_GET_TIME(pMsg->pObject);
  D4CD_TIME_DATA* pData = D4CD_GET_TIME_DATA(pMsg->pObject);
    
                 
  // invoke Change time procedure when enter key is pressed
  if(pMsg->prm.key == D4CD_KEY_SCANCODE_ENTER)
  {
    D4CD_POINT tmp_point;
    // enter to set hour mode
    if(!pData->status.mergedBits.bSet)
    {
      pData->new_time = pData->time;
      D4CD_CaptureKeys(pMsg->pObject);
    }
    
    tmp_point = D4CD_GetClientToScreenPoint(pMsg->pScreen, &(D4CD_GET_TIME(pMsg->pObject)->scrPos));
    
    #if (D4CD_TIME_SEL_CHAR == D4CD_TRUE)
      tmp_point.x++;
    #endif
      
      
      
      
        
    if((pData->status.bits.bVisibleHour) && (!pData->status.bits.bSetHour))
    {
      pData->status.bits.bSetHour = 1;  
      D4CD_SetCursor(&tmp_point, pMsg->pObject->flags->bits.bCursorEna, pMsg->pObject->flags->bits.bCursorMod);
    
    } else if((pData->status.bits.bVisibleMin) && (!pData->status.bits.bSetMin))
    {
      pData->status.bits.bSetHour = 1;
      pData->status.bits.bSetMin = 1;
      if(pData->status.bits.bVisibleHour)
        tmp_point.x += 3;
      
      D4CD_SetCursor(&tmp_point, pMsg->pObject->flags->bits.bCursorEna, pMsg->pObject->flags->bits.bCursorMod);  
    } else if((pData->status.bits.bVisibleSec) && (!pData->status.bits.bSetSec))
    {
      pData->status.bits.bSetHour = 1;
      pData->status.bits.bSetMin = 1;  
      pData->status.bits.bSetSec = 1;
      
      if(pData->status.bits.bVisibleHour)
        tmp_point.x += 3;
      
      if(pData->status.bits.bVisibleMin)
        tmp_point.x += 3;
      D4CD_SetCursor(&tmp_point, pMsg->pObject->flags->bits.bCursorEna, pMsg->pObject->flags->bits.bCursorMod);  
    }else
    
    {
      pData->time = pData->new_time;
      pData->status.mergedBits.bSet = 0;
      
      D4CD_CaptureKeys(NULL);
      
      if(pTime->OnSet)
        pTime->OnSet(pMsg->pObject);
    }
    
    
    pMsg->pObject->flags->bits.bRedraw = 1;        
  }
  
  if(pMsg->prm.key == D4CD_TIME_KEY_SCANCODE_PLUS)
  {
    switch(D4CD_TimeGetCurrSetVal(pData))
    {
      case 0: // hours
        if(++pData->new_time.hours > 23)
          pData->new_time.hours = 0;  
        break;
      case 1: // minutes
        if(++pData->new_time.minutes > 59)
          pData->new_time.minutes = 0;  
        break;
      case 2: // seconds
        if(++pData->new_time.seconds > 59)
          pData->new_time.seconds = 0;  
        break;
    }
  }
  
  if(pMsg->prm.key == D4CD_TIME_KEY_SCANCODE_MINUS)
  {
    switch(D4CD_TimeGetCurrSetVal(pData))
    {
      case 0: // hours
        if(pData->new_time.hours == 0)
          pData->new_time.hours = 23;
        else
          pData->new_time.hours--;
        break;
      case 1: // minutes
        if(pData->new_time.minutes == 0)
          pData->new_time.minutes = 59;
        else
          pData->new_time.minutes--;
        break;
      case 2: // seconds
        if(pData->new_time.seconds == 0)
          pData->new_time.seconds = 59;
        else
          pData->new_time.seconds--; 
        break;
    }
  }
  
  if(pMsg->prm.key == D4CD_KEY_SCANCODE_ESC)
  {
    pData->status.mergedBits.bSet = 0;
    D4CD_CaptureKeys(NULL);  
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
* The main TIME message handler 
*
*******************************************************/

void D4CD_TimeOnMessage(D4CD_MESSAGE* pMsg)
{
    D4CD_POINT tmp_point;
    
    switch(pMsg->nMsgId)
    {
    case D4CD_MSG_DRAW:
        D4CD_TimeOnDraw(pMsg);
        break;

    case D4CD_MSG_KEYUP:
        if(pMsg->pObject->flags->bits.bEnabled)
          D4CD_TimeOnKeyUp(pMsg);
        break;
        
    case D4CD_MSG_TIMETICK:
      if(pMsg->prm.blink.bits.bChange)
        pMsg->pObject->flags->bits.bRedraw = 1;
      
      break;    
        
    case D4CD_MSG_KILLFOCUS:
        D4CD_SetCursor((D4CD_POINT*) &d4cd_size_zero, D4CD_FALSE, D4CD_FALSE);
        break;
    
    case D4CD_MSG_KILLCAPTURE:
      D4CD_GET_TIME(pMsg->pObject)->pData->status.mergedBits.bSet = 0;
      pMsg->pObject->flags->bits.bRedraw = 1;
    
    case D4CD_MSG_SETFOCUS:
      tmp_point = D4CD_GetClientToScreenPoint(pMsg->pScreen, &(D4CD_GET_TIME(pMsg->pObject)->scrPos));
      #if (D4CD_TIME_SEL_CHAR == D4CD_TRUE)
        tmp_point.x++;
      #endif  
      D4CD_SetCursor(&tmp_point, pMsg->pObject->flags->bits.bCursorEna, pMsg->pObject->flags->bits.bCursorMod);
      break;    

    default:
        // call the default behavior
        D4CD_ObjOnMessage(pMsg);
    }
}


/*******************************************************
*
* Set SLIDER value
*
*******************************************************/

void D4CD_TimeSetValue(D4CD_OBJECT_PTR pThis, Byte hours, Byte minutes, Byte seconds)
{
  D4CD_TIME* pTime = D4CD_GET_TIME(pThis);
  D4CD_TIME_DATA* pData = pTime->pData;

  if(seconds > 59)
    return;
  
  if(minutes > 59)
    return;
  
  if(hours > 23)
    return;
  
  pData->time.seconds = seconds;
  pData->time.minutes = minutes;
  pData->time.hours = hours;
  
  pThis->flags->bits.bRedraw = 1;
}

D4CD_TIME_VALUE* D4CD_TimeGetValue(D4CD_OBJECT_PTR pThis)
{
  D4CD_TIME* pTime = D4CD_GET_TIME(pThis);
  D4CD_TIME_DATA* pData = pTime->pData;
  
  return &(pData->time);      
}


/*******************************************************
*
* Modify SLIDER value by a value
*
*******************************************************/

void D4CD_TimeAddSecond(D4CD_OBJECT_PTR pThis)
{
  D4CD_TIME* pTime = D4CD_GET_TIME(pThis);
  D4CD_TIME_DATA* pData = pTime->pData;
  
  pData->time.seconds++;
  
  if(pData->time.seconds > 59)
  {
    pData->time.seconds = 0;
    
    pData->time.minutes++;
  
    if(pData->time.minutes > 59)
    {
      pData->time.minutes = 0;
      
      pData->time.hours++;
  
      if(pData->time.hours > 23)
        pData->time.hours = 0;
    }
  }
  
  pThis->flags->bits.bRedraw = 1;
}

void D4CD_TimeSetVisibility(D4CD_OBJECT_PTR pThis, D4CD_BOOL hours, D4CD_BOOL minutes, D4CD_BOOL seconds)
{
  D4CD_TIME* pTime = D4CD_GET_TIME(pThis);
  D4CD_TIME_DATA* pData = pTime->pData;
  
  pData->status.bits.bVisibleSec = seconds;
  pData->status.bits.bVisibleMin = minutes;
  pData->status.bits.bVisibleHour = hours;
  
  pThis->flags->bits.bRedraw = 1;
}


void D4CD_TimeSetMode(D4CD_OBJECT_PTR pThis, D4CD_BOOL enable24mode)
{
  D4CD_TIME* pTime = D4CD_GET_TIME(pThis);
  D4CD_TIME_DATA* pData = pTime->pData;
  
  pData->status.bits.bMode12_24 = enable24mode;

  pThis->flags->bits.bRedraw = 1;
}
