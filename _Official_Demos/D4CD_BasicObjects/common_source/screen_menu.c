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
* @file      screen_menu.c
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Jan-18-2011
* 
* @brief     Screen for testing menu object
*
******************************************************************************/

#include "d4cd.h"                                         

// extern all used (called) screens
D4CD_EXTERN_SCREEN(scr_testChckB);
D4CD_EXTERN_SCREEN(scr_testNum);
D4CD_EXTERN_SCREEN(scr_testDate);
D4CD_EXTERN_SCREEN(scr_testTime);
D4CD_EXTERN_SCREEN(scr_testListBox);
D4CD_EXTERN_SCREEN(scr_testPassWord);

// declare the static functions headers
static void Menu_OnClick(D4CD_OBJECT* pThis, D4CD_MENU_INDEX ix);
static Byte Menu_OnUsrMessage(D4CD_MESSAGE* pMsg);


// D4CD_MENU with title line
D4CD_DECLARE_MENU_BEGIN(scrTestMenu_Menu1, "Menu", 0, 0, D4CD_SCREEN_CHAR_CNT, D4CD_SCREEN_LINE_CNT, D4CD_MENU_F_DEFAULT, NULL, Menu_OnClick, Menu_OnUsrMessage)
  D4CD_DECLARE_MENU_ITEM_FULL("Check Boxes", &scr_testChckB)
  D4CD_DECLARE_MENU_ITEM_FULL("Number", &scr_testNum)
  D4CD_DECLARE_MENU_ITEM_FULL("List Box", &scr_testListBox)
  D4CD_DECLARE_MENU_ITEM_FULL("Date", &scr_testDate)
  D4CD_DECLARE_MENU_ITEM_FULL("Time", &scr_testTime)
  D4CD_DECLARE_MENU_ITEM_FULL("Password", &scr_testPassWord)
D4CD_DECLARE_MENU_END(scrTestMenu_Menu1)  

// D4CD_MENU without title line
D4CD_DECLARE_MENU_BEGIN(scrTestMenu_Menu2, NULL, 0, 0, D4CD_SCREEN_CHAR_CNT, D4CD_SCREEN_LINE_CNT, (D4CD_OBJECT_F_ENABLED | D4CD_OBJECT_F_TABSTOP), NULL, Menu_OnClick, Menu_OnUsrMessage)
  D4CD_DECLARE_MENU_ITEM_FULL("Check Boxes", &scr_testChckB)
  D4CD_DECLARE_MENU_ITEM_FULL("Number", &scr_testNum)
  D4CD_DECLARE_MENU_ITEM_FULL("List Box", &scr_testListBox)
  D4CD_DECLARE_MENU_ITEM_FULL("Date", &scr_testDate)
  D4CD_DECLARE_MENU_ITEM_FULL("Time", &scr_testTime)
  D4CD_DECLARE_MENU_ITEM_FULL("Password", &scr_testPassWord)
D4CD_DECLARE_MENU_END(scrTestMenu_Menu2) 
  
// D4CD_SCREEN declaration
D4CD_DECLARE_SCREEN_BEGIN(scr_testMenu, TestMenuScreen_, 0 ,0, D4CD_SCREEN_CHAR_CNT, D4CD_SCREEN_LINE_CNT, NULL, D4CD_USER_CHAR_TABLE_DEFAULT, (D4CD_SCR_F_BCKG))
  D4CD_DECLARE_SCREEN_OBJECT(scrTestMenu_Menu1)
  D4CD_DECLARE_SCREEN_OBJECT(scrTestMenu_Menu2)
D4CD_DECLARE_SCREEN_END()

// Menu  On Click call back function
static void Menu_OnClick(D4CD_OBJECT* pThis, D4CD_MENU_INDEX ix)
{
  D4CD_SCREEN* pScr;
  
  D4CD_UNUSED(ix);
  
  // get the screen pointer
  pScr = (D4CD_SCREEN*)D4CD_MenuGetItemUserData(pThis);
  
  // if screen exists just activated it
  if(pScr)
    D4CD_ActivateScreen( pScr, D4CD_FALSE); 
}

// menu on user message call back - it is defined to create more user friendly escape of screen on screen with only one captured object
static Byte Menu_OnUsrMessage(D4CD_MESSAGE* pMsg)
{
  if(pMsg->nMsgId == D4CD_MSG_KEYDOWN)
  {
    if(pMsg->prm.key == D4CD_KEY_SCANCODE_ESC)
    {
      D4CD_EscapeScreen();      
    }
  }  
  return D4CD_MSG_NOSKIP; 
}

// One time called screen function in screen initialization proces
static void TestMenuScreen_OnInit()
{

}

// Screen on Activate function called with each screen activation
static void TestMenuScreen_OnActivate()
{
  // activate capture keys for right type of menu object
  if(scrTestMenu_Menu1.flags->bits.bVisible)
    D4CD_CaptureKeys(&scrTestMenu_Menu1);  
  else
    D4CD_CaptureKeys(&scrTestMenu_Menu2);
}


// Screen "Main" function called periodically in each D4CD_poll runs
static void TestMenuScreen_OnMain()
{
    
}


// Screen on DeActivate function called with each screen deactivation
static void TestMenuScreen_OnDeactivate()
{
    
}

// Screen on message function called with each internal massage for this screen
static Byte TestMenuScreen_OnObjectMsg(D4CD_MESSAGE* pMsg)
{
    D4CD_UNUSED(pMsg);
    return 0;
}