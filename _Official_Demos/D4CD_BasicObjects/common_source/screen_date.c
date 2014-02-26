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
* @file      screen_date.c
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Jan-18-2011
* 
* @brief     Screen for testing date user object
*
******************************************************************************/

#include "d4cd.h"

// declare the static functions headers
static void D4CD_DateOnChange(D4CD_OBJECT* pThis);


// the date name table
const D4CD_CHAR monthNames[12][4] =
{
  "Jan",
  "Feb",
  "Mar",
  "Apr",
  "May",
  "Jun",
  "Jul",
  "Aug",
  "Sep",
  "Oct",
  "Nov",
  "Dec"      
};

const D4CD_DATE_MULTITEXT monthTable = 
{
 4,
 12,
 (D4CD_CHAR* const)&monthNames
};


// D4CD_DATE declaration
D4CD_DECLARE_STD_DATE_AUTOSIZE(tstDateScr_Date, 0, 0, NULL, &monthTable, D4CD_DateOnChange)

// D4CD_SCREEN declaration
D4CD_DECLARE_SCREEN_BEGIN(scr_testDate, TestDateScreen_, 0 ,0, D4CD_SCREEN_CHAR_CNT, D4CD_SCREEN_LINE_CNT, "Test Date", D4CD_USER_CHAR_TABLE_DEFAULT, (D4CD_SCR_F_BCKG))
  D4CD_DECLARE_SCREEN_OBJECT(tstDateScr_Date)
D4CD_DECLARE_SCREEN_END()

// Date On Change call back function
static void D4CD_DateOnChange(D4CD_OBJECT* pThis)
{
  D4CD_UNUSED(pThis);
}

// One time called screen function in screen initialization proces
static void TestDateScreen_OnInit()
{

}

// Screen on Activate function called with each screen activation
static void TestDateScreen_OnActivate()
{
  
}


// Screen "Main" function called periodically in each D4CD_poll runs
static void TestDateScreen_OnMain()
{

}


// Screen on DeActivate function called with each screen deactivation
static void TestDateScreen_OnDeactivate()
{
    
}

// Screen on message function called with each internal massage for this screen
static Byte TestDateScreen_OnObjectMsg(D4CD_MESSAGE* pMsg)
{
    D4CD_UNUSED(pMsg);
    return 0;
}