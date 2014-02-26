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
* @file      d4cd_private.h
*
* @author    Petr Gargulak
* 
* @version   0.0.1.0
* 
* @date      Jan-6-2011
* 
* @brief     D4CD Driver private header file
*
*******************************************************************************/


#ifndef __D4CD_PRIVATE_H
#define __D4CD_PRIVATE_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/


/******************************************************************************
* Macros 
******************************************************************************/

#define D4CD_MK_STR1(x) x##_ID
#define D4CD_MK_STR(x) D4CD_MK_STR1(x)


/******************************************************************************
* Global variables
******************************************************************************/

// screen activation hisotry array & index
extern D4CD_SCREEN* d4cd_screenHistory[D4CD_SCREEN_HISTORY];
extern Byte d4cd_screenHistoryIndex;

// if not NULL, keys are captured by this object
extern D4CD_OBJECT* d4cd_pKeysCapturer;

// one and only message object being used to route information to objects
extern D4CD_MESSAGE d4cd_msg;

// The D4CD system flags
extern D4CD_SYSTEM_FLAGS d4cd_systemFlags;

// zero size structure for automatic function capability
extern const D4CD_SIZE d4cd_size_zero; 

// LCD HW cursor 
extern D4CD_CURSOR d4cd_cursor;

// Blink data storage
extern D4CD_BLINK d4cd_blink;

/******************************************************************************
* Global functions
******************************************************************************/

// Internal API of base functions

void D4CD_HandleKeys(void);

void D4CD_SendMessage(D4CD_MESSAGE* pMsg);

void D4CD_ObjOnMessage(D4CD_MESSAGE* pMsg);
void D4CD_ChangeText(D4CD_STRING* pBuff, char* pNewText, char fillChar);

void D4CD_FocusNextObject(D4CD_SCREEN* pScreen, Byte bInitialSearch);
void D4CD_FocusPrevObject(D4CD_SCREEN* pScreen);


void D4CD_RedrawScreen(D4CD_SCREEN* pScreen);

// Font private function declaration


#endif /* __D4CD_PRIVATE_H */
