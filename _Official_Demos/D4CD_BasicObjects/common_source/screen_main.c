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
* @file      screen_main.c
*
* @author    Petr Gargulak
* 
* @version   0.0.3.0
* 
* @date      Feb-4-2011
* 
* @brief     Screen for testing buttons objects
*
******************************************************************************/

#include "d4cd.h"

// externs of used objects / screens in other files
D4CD_EXTERN_SCREEN(scr_testMenu);

D4CD_EXTERN_OBJECT(scrTestMenu_Menu1);
D4CD_EXTERN_OBJECT(scrTestMenu_Menu2);

// declare the static functions headers
static void D4CD_BtnMenu1OnClick(D4CD_OBJECT* pThis);
static void D4CD_BtnMenu2OnClick(D4CD_OBJECT* pThis);


// D4CD_BUTTON declarations
D4CD_DECLARE_STD_BUTTON_AUTOSIZE(btn_test1, "Menu1", 0, 0, D4CD_BtnMenu1OnClick)
D4CD_DECLARE_STD_BUTTON_AUTOSIZE(btn_test2, "Menu2", 8, 0, D4CD_BtnMenu2OnClick)

// D4CD_SCREEN declaration
D4CD_DECLARE_SCREEN_BEGIN(scr_test, TestScreen_, 0 ,0, D4CD_SCREEN_CHAR_CNT, D4CD_SCREEN_LINE_CNT, "D4CD", D4CD_USER_CHAR_TABLE_DEFAULT, (D4CD_SCR_F_BCKG))
  D4CD_DECLARE_SCREEN_OBJECT(btn_test1)
  D4CD_DECLARE_SCREEN_OBJECT(btn_test2)
D4CD_DECLARE_SCREEN_END()

// Button1 On Click call back function
static void D4CD_BtnMenu1OnClick(D4CD_OBJECT* pThis)
{
  (void)pThis;
  D4CD_ShowObject(&scrTestMenu_Menu1, D4CD_TRUE);
  D4CD_ShowObject(&scrTestMenu_Menu2, D4CD_FALSE);
  
  D4CD_ActivateScreen(&scr_testMenu, D4CD_FALSE);
}

// Button2 On Click call back function
static void D4CD_BtnMenu2OnClick(D4CD_OBJECT* pThis)
{
  (void)pThis;
  D4CD_ShowObject(&scrTestMenu_Menu1, D4CD_FALSE);
  D4CD_ShowObject(&scrTestMenu_Menu2, D4CD_TRUE);
  
  D4CD_ActivateScreen(&scr_testMenu, D4CD_FALSE);
}

// One time called screen function in screen initialization proces
static void TestScreen_OnInit()
{

}

// Screen on Activate function called with each screen activation
static void TestScreen_OnActivate()
{
  
}


// Screen "Main" function called periodically in each D4CD_poll runs
static void TestScreen_OnMain()
{

}


// Screen on DeActivate function called with each screen deactivation
static void TestScreen_OnDeactivate()
{
    
}

// Screen on message function called with each internal massage for this screen
static Byte TestScreen_OnObjectMsg(D4CD_MESSAGE* pMsg)
{
    D4CD_UNUSED(pMsg);
    return 0;
}