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
* @file      d4cd_date.c
*
* @author    Petr Gargulak
* 
* @version   0.0.3.0
* 
* @date      Feb-14-2011
* 
* @brief     D4CD driver date object c file 
*
******************************************************************************/

#include "d4cd.h"
#include "d4cd_private.h"

//Internal API
void D4CD_DateOnMessage(D4CD_MESSAGE* pMsg);
static void D4CD_DateDateTick(D4CD_MESSAGE* pMsg);
static Byte D4CD_DateGetCurrSetVal(D4CD_DATE_DATA* pData);

const D4CD_OBJECT_SYS_FUNCTION d4cd_dateSysFunc = 
{
  D4CD_OBJECT_DATE,
  D4CD_DateOnMessage,
  NULL
};

typedef struct 
{
    D4CD_POINT Position;
    D4CD_SIZE Size;
    D4CD_COOR daySize;
    D4CD_COOR monthSize;
} D4CD_DATE_TMP_VAL;

#define _calc (*((D4CD_DATE_TMP_VAL*)d4cd_scratchPad))
//static D4CD_DATE_TMP_VAL _calc;
    
/**************************************************************//*!
*
* convert decimal number to 2 digit string    00 - 99
*
******************************************************************/
void D4CD_SprintDec99(Byte value, char *pText) 
{
  if(value > 99)
    value = 99;
  
  *pText++ = (char)((value/10) + '0');
  value %= 10; 

  *pText = (char)((value) + '0');
}

/**************************************************************//*!
*
* convert decimal number to 1 digit string    0 - 9
*
******************************************************************/
void D4CD_SprintDec9(Byte value, char *pText) 
{
  if(value > 9)
    value = 9;
  
  *pText = (char)((value) + '0');
}    
    
static void D4CD_DateValue2Coor(D4CD_OBJECT* pThis)
{
  D4CD_DATE* pDate = D4CD_GET_DATE(pThis);
  D4CD_DATE_DATA* pData = D4CD_GET_DATE_DATA(pThis);
  
  
  _calc.Size = pDate->scrSize;
  _calc.Position = D4CD_GetClientToScreenPoint(*pThis->pScreen, &pDate->scrPos);
  
#if  D4CD_ENABLE_AUTOSIZE != 0       // Enable/Disable autosize code capatibilities
        
    //Check Size of date in axis X   
    if(!_calc.Size.cx)
    {
      D4CD_COOR tmp_size = 0; 
      
#if (D4CD_DATE_SEL_CHAR == D4CD_TRUE)
      tmp_size++;
#endif
      
      if(pData->status.bits.bVisibleDay)
      {
        if(pDate->pDayNamesTable == NULL)
          _calc.daySize = 1;
        else
          _calc.daySize = (D4CD_COOR)(pDate->pDayNamesTable->maxLength - 1);
        
        tmp_size += _calc.daySize;
      }
      
      if(pData->status.bits.bVisibleDate)
        tmp_size += 2;
      
      if(pData->status.bits.bVisibleMonth)
      {
        if(pDate->pMonthNamesTable == NULL)
          _calc.monthSize = 2;
        else
          _calc.monthSize = (D4CD_COOR)(pDate->pMonthNamesTable->maxLength - 1);
        tmp_size += _calc.monthSize;
      }
      if(pData->status.bits.bVisibleYear)
        tmp_size += 4;
      
      
      _calc.Size.cx = tmp_size;
    }
    
    //Check Size of date in axis Y
    
    if(!_calc.Size.cy)
    {
      _calc.Size.cy++;
    }
    
#endif
  
}

/*******************************************************
*
* DATE Drawing routine
*
*******************************************************/

static void D4CD_DateOnDraw(D4CD_MESSAGE* pMsg)
{
    D4CD_DATE* pDate = D4CD_GET_DATE(pMsg->pObject);
    D4CD_DATE_DATA* pData = D4CD_GET_DATE_DATA(pMsg->pObject);    
    D4CD_OBJECT_DRAWFLAGS draw = pMsg->prm.draw;
    D4CD_CHAR tmp_str[9];
    Byte tmp_len = 0;
    Byte tmp_currSetVal = D4CD_DateGetCurrSetVal(pData);
    Word tmp_dateVal;
    Byte i;
    D4CD_DateValue2Coor(pMsg->pObject);
    
    if(draw.bits.bComplete)
    {
      D4CD_FillRect(&_calc.Position, &_calc.Size, ' ');
    }
    
#if (D4CD_DATE_SEL_CHAR == D4CD_TRUE)
      if(draw.bits.bFocused)
        D4CD_PutChar(&_calc.Position, D4CD_DATE_SEL_FOCUS_CHAR);
      else
        D4CD_PutChar(&_calc.Position, D4CD_DATE_SEL_NORMAL_CHAR);
      
      _calc.Position.x++;
#endif
    
    if(pData->status.bits.bVisibleDay)
    {
      if(tmp_currSetVal != 0xff)
        tmp_dateVal = (Byte)pData->new_date.day;
      else
        tmp_dateVal = (Byte)pData->date.day;
      
      if((tmp_currSetVal == 0) && D4CD_GetBlink())
      {
        for(i=0; i <_calc.daySize; i++)
        { 
          D4CD_PutChar(&_calc.Position, (D4CD_CHAR)('1' + tmp_dateVal));
          _calc.Position.x++;
        }  
      } else
      {
        if(pDate->pDayNamesTable == NULL)
        {
          D4CD_PutChar(&_calc.Position, (D4CD_CHAR)('1' + tmp_dateVal));          
        }else
        {
          D4CD_DrawSimpleText(&_calc.Position, (D4CD_CHAR*)(pDate->pDayNamesTable->pStartTable + (tmp_dateVal * pDate->pDayNamesTable->maxLength)));
        }
        _calc.Position.x += _calc.daySize;  
      }      
    }
      
    if(pData->status.bits.bVisibleDate)
    {
      if(pData->status.bits.bVisibleDay)
      {
        D4CD_PutChar(&_calc.Position, D4CD_DATE_DIVIDER_CHAR);
        _calc.Position.x++;
      }
      
      if(tmp_currSetVal != 0xff)
        tmp_dateVal = pData->new_date.date;
      else
        tmp_dateVal = pData->date.date;
      
      if((tmp_currSetVal == 1) && D4CD_GetBlink())
      {
        D4CD_PutChar(&_calc.Position, ' ');
          _calc.Position.x++;
          D4CD_PutChar(&_calc.Position, ' ');
          _calc.Position.x++;
      } else
      {
        D4CD_SprintDec99((Byte)(tmp_dateVal + 1), tmp_str);
        tmp_str[2] = 0;
        D4CD_DrawSimpleText(&_calc.Position, tmp_str);
        _calc.Position.x += 2;  
      }
      
    }
    
    if(pData->status.bits.bVisibleMonth)
    {
      if(pData->status.bits.bVisibleDay || pData->status.bits.bVisibleDate)
      {
        D4CD_PutChar(&_calc.Position, D4CD_DATE_DIVIDER_CHAR);
        _calc.Position.x++;
      }
      
      if(tmp_currSetVal != 0xff)
        tmp_dateVal = (Byte)pData->new_date.month;
      else
        tmp_dateVal = (Byte)pData->date.month;
      
      if((tmp_currSetVal == 2) && D4CD_GetBlink())
      {
        for(i=0; i <_calc.monthSize; i++)
        { 
          D4CD_PutChar(&_calc.Position, ' ');
          _calc.Position.x++;
        }  
      } else
      {
        if(pDate->pMonthNamesTable == NULL)
        {
          D4CD_SprintDec99((Byte)(tmp_dateVal + 1), tmp_str);
          tmp_str[2] = 0;
          D4CD_DrawSimpleText(&_calc.Position, tmp_str);         
        }else
        {
          D4CD_DrawSimpleText(&_calc.Position, (D4CD_CHAR*)(pDate->pMonthNamesTable->pStartTable + (tmp_dateVal * pDate->pMonthNamesTable->maxLength)));
        }
        _calc.Position.x += _calc.monthSize;  
      }      
    }
    
    if(pData->status.bits.bVisibleYear)
    {
      if(pData->status.bits.bVisibleDay || pData->status.bits.bVisibleDate || pData->status.bits.bVisibleMonth)
      {
        D4CD_PutChar(&_calc.Position, D4CD_DATE_DIVIDER_CHAR);
        _calc.Position.x++;
      }
      
      if(tmp_currSetVal != 0xff)
        tmp_dateVal = pData->new_date.year;
      else
        tmp_dateVal = pData->date.year;
      
      if((tmp_currSetVal == 3) && D4CD_GetBlink())
      {
        D4CD_MoveTo(&_calc.Position);
        _calc.Position.x += 4;
        D4CD_LineTo(&_calc.Position, ' ');        
      } else
      {
        tmp_str[D4CD_SprintDecU16(tmp_dateVal, tmp_str, 0)] = 0; 
        D4CD_DrawSimpleText(&_calc.Position, tmp_str);        
      }
      
    }
    

    
}

/*******************************************************
*
* DATE key handling routine
*
*******************************************************/

static Byte D4CD_DateGetCurrSetVal(D4CD_DATE_DATA* pData)
{
  if(!pData->status.mergedBits.bSet)
    return 0xff;
  
  if(!pData->status.bits.bSetDate)
    return 0;
    
  if(!pData->status.bits.bSetMonth)
    return 1;
  
  if(!pData->status.bits.bSetYear)
    return 2;
  
  return 3;
}

static void D4CD_DateOnKeyUp(D4CD_MESSAGE* pMsg)    
{
  D4CD_DATE* pDate = D4CD_GET_DATE(pMsg->pObject);
  D4CD_DATE_DATA* pData = D4CD_GET_DATE_DATA(pMsg->pObject);
    
                 
  // invoke Change date procedure when enter key is pressed
  if(pMsg->prm.key == D4CD_KEY_SCANCODE_ENTER)
  {
    D4CD_POINT tmp_point;
    
    // enter to set hour mode
    if(!pData->status.mergedBits.bSet)
    {
      pData->new_date = pData->date;
      D4CD_CaptureKeys(pMsg->pObject);
    }
    
    tmp_point = D4CD_GetClientToScreenPoint(pMsg->pScreen, &(D4CD_GET_DATE(pMsg->pObject)->scrPos));
    
    #if (D4CD_DATE_SEL_CHAR == D4CD_TRUE)
      tmp_point.x++;
    #endif
    
    if((pData->status.bits.bVisibleDate) && (!pData->status.bits.bSetDate))
    {
      pData->status.bits.bSetDate = 1;  
      
      if(pData->status.bits.bVisibleDay)
      {
        if(pDate->pDayNamesTable == NULL)
          tmp_point.x += 2;
        else
          tmp_point.x += pDate->pDayNamesTable->maxLength;
      }
      
      D4CD_SetCursor(&tmp_point, pMsg->pObject->flags->bits.bCursorEna, pMsg->pObject->flags->bits.bCursorMod);
    
    } else if((pData->status.bits.bVisibleMonth) && (!pData->status.bits.bSetMonth))
    {
      pData->status.bits.bSetDate = 1;
      pData->status.bits.bSetMonth = 1;
      
      if(pData->status.bits.bVisibleDay)
      {
        if(pDate->pDayNamesTable == NULL)
          tmp_point.x += 2;
        else
          tmp_point.x += pDate->pDayNamesTable->maxLength;
      }
      
      if(pData->status.bits.bVisibleDate)
        tmp_point.x += 3;
      
      D4CD_SetCursor(&tmp_point, pMsg->pObject->flags->bits.bCursorEna, pMsg->pObject->flags->bits.bCursorMod);
      
    } else if((pData->status.bits.bVisibleYear) && (!pData->status.bits.bSetYear))
    {
      pData->status.bits.bSetDate = 1;
      pData->status.bits.bSetMonth = 1;
      pData->status.bits.bSetYear = 1;
      
      if(pData->status.bits.bVisibleDay)
      {
        if(pDate->pDayNamesTable == NULL)
          tmp_point.x += 2;
        else
          tmp_point.x += pDate->pDayNamesTable->maxLength;
      }
      
      if(pData->status.bits.bVisibleDate)
        tmp_point.x += 3;
      
      if(pData->status.bits.bVisibleMonth)
      {
        if(pDate->pMonthNamesTable == NULL)
          tmp_point.x += 3;
        else
          tmp_point.x += pDate->pMonthNamesTable->maxLength;        
      }
      
      D4CD_SetCursor(&tmp_point, pMsg->pObject->flags->bits.bCursorEna, pMsg->pObject->flags->bits.bCursorMod);
      
    }else
    {
      pData->date = pData->new_date;
      pData->status.mergedBits.bSet = 0;
      
      D4CD_CaptureKeys(NULL);
      
      if(pDate->OnSet)
        pDate->OnSet(pMsg->pObject);
    }
    pMsg->pObject->flags->bits.bRedraw = 1;        
  }
  
  if(pMsg->prm.key == D4CD_DATE_KEY_SCANCODE_PLUS)
  {
    switch(D4CD_DateGetCurrSetVal(pData))
    {
      case 1: // date
        if(++pData->new_date.date > 30)
          pData->new_date.date = 0;  
        break;
      case 2: // month
        if(++pData->new_date.month > 11)
          pData->new_date.month = 0;  
        break;
      case 3: // year
        if(++pData->new_date.year > D4CD_DATE_YEAR_MAX)
          pData->new_date.year = D4CD_DATE_YEAR_MIN;  
        break;
    }
  }
  
  if(pMsg->prm.key == D4CD_DATE_KEY_SCANCODE_MINUS)
  {
    switch(D4CD_DateGetCurrSetVal(pData))
    {
      case 1: // date
        if(pData->new_date.date == 0)
          pData->new_date.date = 30;
        else
          pData->new_date.date--;
        break;
      case 2: // month
        if(pData->new_date.month == 0)
          pData->new_date.month = 11;
        else
          pData->new_date.month--;
        break;
      case 3: // year
        if(pData->new_date.year == D4CD_DATE_YEAR_MIN)
          pData->new_date.year = D4CD_DATE_YEAR_MAX;
        else
          pData->new_date.year--; 
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
* The main DATE message handler 
*
*******************************************************/

void D4CD_DateOnMessage(D4CD_MESSAGE* pMsg)
{
    D4CD_POINT tmp_point;
    
    switch(pMsg->nMsgId)
    {
    case D4CD_MSG_DRAW:
        D4CD_DateOnDraw(pMsg);
        break;

    case D4CD_MSG_KEYUP:
        if(pMsg->pObject->flags->bits.bEnabled)
          D4CD_DateOnKeyUp(pMsg);
        break;
        
    case D4CD_MSG_TIMETICK:
      if(pMsg->prm.blink.bits.bChange)
        if((D4CD_GET_DATE(pMsg->pObject))->pData->status.mergedBits.bSet)
          pMsg->pObject->flags->bits.bRedraw = 1;
      break;    
        
    case D4CD_MSG_KILLFOCUS:
        D4CD_SetCursor((D4CD_POINT*) &d4cd_size_zero, D4CD_FALSE, D4CD_FALSE);
        break;
    
    case D4CD_MSG_KILLCAPTURE:
      D4CD_GET_DATE(pMsg->pObject)->pData->status.mergedBits.bSet = 0;
      pMsg->pObject->flags->bits.bRedraw = 1;
    
    case D4CD_MSG_SETFOCUS:
      tmp_point = D4CD_GetClientToScreenPoint(pMsg->pScreen, &(D4CD_GET_DATE(pMsg->pObject)->scrPos));
      #if (D4CD_DATE_SEL_CHAR == D4CD_TRUE)
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

void D4CD_DateSetValue(D4CD_OBJECT_PTR pThis, Byte day, Byte date, Byte month, Word year)
{
  D4CD_DATE* pDate = D4CD_GET_DATE(pThis);
  D4CD_DATE_DATA* pData = pDate->pData;

  if(day > 6)
    return;
  
  if(date > 30)
    return;
  
  if(month > 11)
    return;
  
  if((year > D4CD_DATE_YEAR_MAX) || (year < D4CD_DATE_YEAR_MIN))
    return;
  
  pData->date.day = (D4CD_DATE_DAY)day;
  pData->date.date = (Byte)date;
  pData->date.month = (Byte)month;
  pData->date.year = (Word)year;
  
  pThis->flags->bits.bRedraw = 1;
}

D4CD_DATE_VALUE* D4CD_DateGetValue(D4CD_OBJECT_PTR pThis)
{
  D4CD_DATE* pDate = D4CD_GET_DATE(pThis);
  D4CD_DATE_DATA* pData = pDate->pData;
  return &(pData->date);      
}


/*******************************************************
*
* Modify SLIDER value by a value
*
*******************************************************/


void D4CD_DateSetVisibility(D4CD_OBJECT_PTR pThis, D4CD_BOOL day, D4CD_BOOL date, D4CD_BOOL month, D4CD_BOOL year)
{
  D4CD_DATE* pDate = D4CD_GET_DATE(pThis);
  D4CD_DATE_DATA* pData = pDate->pData;
  
  pData->status.bits.bVisibleDay = day;
  pData->status.bits.bVisibleDate = date;
  pData->status.bits.bVisibleMonth = month;
  pData->status.bits.bVisibleYear = year;
  
  pThis->flags->bits.bRedraw = 1;
}

