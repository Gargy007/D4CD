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
* @file      d4cd_user_cfg.h
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Jan-18-2011
* 
* @brief     D4CD Driver user configuration file
*
*******************************************************************************/

#ifndef __D4CD_USER_CFG_H
#define __D4CD_USER_CFG_H

/******************************************************************************
* Desciption : This file allows user depends modification of D4CD Freescale 
*   graphical driver. 
*
*
******************************************************************************/

/**************************************************************//*!
*
* Low Level driver specification for LCD diplay
*
******************************************************************/
// List of implemented low level LCD drivers

// d4cdlcd_hd44780 - driver for LCD displays with d4cdlcd_hd44780 controller


// Please define a used low LCD driver
#define D4CD_LLD_LCD d4cdlcd_hd44780   // the name of low level driver descriptor structure

// List of implemented low level LCD hw interface drivers

// d4cdlcdhw_gpio_8bit - low level hw interface driver for paralel 8 bit communication
// d4cdlcdhw_gpio_4bit - low level hw interface driver for paralel 4 bit communication
  
// Please (if it's needed) define a used LCD hw interface driver
#define D4CD_LLD_LCD_HW d4cdlcdhw_gpio_4bit   // the name of LCD hw interface driver descriptor structure



/******************************************************************************
*
*        ==================== D4CD General ======================              
*
*               User definition of general bahaviour of D4CD
*     To apply user definition uncomment and modify the accurate line
*
******************************************************************************/


/******************************************************************************
* Types
******************************************************************************/

// debug define - that enables checking overflow of scrapad memory area
#define D4CD_SANITY_CHECK 0

// The D4CDrv contains own standard types for cases that in the whole project missing 
// loaded standard types as Byte, Word, LWord etc.
// To disable using of D4CDrv own standard types change the below define to D4CD_TRUE 

#define D4CD_USE_STANDARD_TYPES D4CD_FALSE

// Variable of coordinations of display
// in simple - if display has resolution bigger than 256, than you have to specify 
// bigger type than unsigned char (recommended is unsigned short) 
//#define D4CD_COOR_TYPE Word

// Bitfield modification section 
// By this options can be modified driver to run with different bitfield representations

      // The first modification is alignment of bitfields sets in compilator right or left align
      #define D4CD_BITFIELD_LSB_ALIGNMENT D4CD_BITFIELD_LSB_ALLIGMENT_RIGHT      // jasne specifikovat na boolean

      // The second one allows adapt to 2/4 byte bitfileds for example in Coldfire V2 compilators
      #define D4CD_BITFIELD_SHIFT 0

      // Example for Codewarrior Coldfire V2 Compilator
      // #define D4CD_BITFIELD_LSB_ALIGNMENT D4CD_BITFIELD_LSB_ALLIGMENT_LEFT
      // #define D4CD_BITFIELD_SHIFT 24

      // Example for Codewarrior Coldfire V1/S08 Compilator
      // #define D4CD_BITFIELD_LSB_ALIGNMENT D4CD_BITFIELD_LSB_ALLIGMENT_RIGHT
      // #define D4CD_BITFIELD_SHIFT 0

 
/******************************************************************************
*
*   User definition of input KEYS format 
*     To apply user definition uncomment the accurate line
*
******************************************************************************/

//#define D4CD_KEYS_BUFF_LENGTH 8

// Here is place for any change of standard keys codes

//#define D4CD_KEY_SCANCODE_UP     0x51
//#define D4CD_KEY_SCANCODE_DOWN   0x50
//#define D4CD_KEY_SCANCODE_LEFT   0x4B
//#define D4CD_KEY_SCANCODE_RIGHT  0x4D
//#define D4CD_KEY_SCANCODE_ENTER  0x1C
//#define D4CD_KEY_SCANCODE_ESC    0x01

#define D4CD_KEY_FUNC_FOCUS_NEXT    D4CD_KEY_SCANCODE_UP
#define D4CD_KEY_FUNC_FOCUS_PREV    D4CD_KEY_SCANCODE_DOWN

// Backward compatibility section
  #define D4CD_KEY_UP     0x01
  #define D4CD_KEY_DOWN   0x02
  #define D4CD_KEY_LEFT   0x80
  #define D4CD_KEY_RIGHT  0x40
  #define D4CD_KEY_ENTER  0x04
  #define D4CD_KEY_ESC    0x08

/******************************************************************************
* Constants
******************************************************************************/

//Select the MCU type that is used in this project  supported types:
//D4CD_HC08  
//D4CD_HCS08 
//D4CD_HC12  
//D4CD_HCS12 
//D4CD_HCS12X
//D4CD_MCF51 
//D4CD_MCF52 
//D4CD_MCF53 
//D4CD_MCF54 

#define D4CD_MCU_TYPE D4CD_HCS08

#define D4CD_ENABLE_AUTOSIZE  D4CD_TRUE // This option disable autosize part of code in driver
                                        // So it save some place in Flash but all parameters has to be declared   


#define D4CD_SCREEN_HISTORY 10         // This option define depth of history of screens

#define D4CD_SCREEN_CHAR_CNT  16  // The character count per line  of physical LCD

#define D4CD_SCREEN_LINE_CNT  2  // The lines count of physical LCD

#define D4CD_MCU_BUS_CLOCK 20000000L  // Mcu bus clock in Hz mainly for few delay loops in low level

#define D4CD_USER_CHAR_TABLE_DEFAULT userCharTable1  // the default user character table

/******************************************************************************
* CallBack function definition
******************************************************************************/


//#define D4CD_INPUT_EVENT_CALLBACK D4CD_InputEventCB // This name of callback function 
                                                  // that is called with all input events
                                                  // from keyboard or touchscreen. 
                                                  // Prototype is void D4CD_INPUT_EVENT_CALLBACK(void)
                                                  // Uncomment to used it.           


/******************************************************************************
*
*        ==================== D4CD_SCREEN ======================              
*
*               User definition of defaults of D4CD screen
*     To apply user definition uncomment and modify the accurate line
*
******************************************************************************/


//#define D4CD_SCR_F_DEFAULT        (0)

/******************************************************************************
*
*        ==================== D4CD_OBJECT ======================              
*
*          User definition of defaults  of individual objects
*          To apply user definition uncomment the accurate line
*
******************************************************************************/



/******************************************************************************
* Button Object
******************************************************************************/

/***********************
* Properties
***********************/

// the default button system flags
//#define D4CD_BTN_F_DEFAULT  (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED | D4CD_OBJECT_F_TABSTOP | D4CD_OBJECT_F_CURSOR_EN )

// the default button  text properties 
//#define D4CD_BTN_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)

// definition of button left border char - zero value hide the button border char
// #define D4CD_BTN_SEL_NORM_CHAR_LEFT 0  //'('

// definition of button right border char - zero value hide the button border char
// #define D4CD_BTN_SEL_NORM_CHAR_RIGHT 0 //')'

// definition of button left focused border char - zero value hide the button border char
//  #define D4CD_BTN_SEL_FOCUS_CHAR_LEFT 0  //'>'

// definition of button right focused border char - zero value hide the button border char
//  #define D4CD_BTN_SEL_FOCUS_CHAR_RIGHT 0 //'<'

/***********************
* Sizes constants
***********************/

/******************************************************************************
* CheckBox Object
******************************************************************************/

/***********************
* Properties
***********************/

// the default check box system flags
// #define D4CD_CHECKBOX_F_DEFAULT  (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED | D4CD_OBJECT_F_TABSTOP\
//                                    | D4CD_OBJECT_F_CURSOR_EN  | D4CD_OBJECT_F_CURSOR_MOD)

// the default check box text properties 
// #define D4CD_CHECKBOX_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_LEFT_MASK)

// definition of check box char that presented CHECKED state 
// #define D4CD_CHECKBOX_CHECKED_CHAR 'X'

// definition of check box char that presented UNCHECKED state
// #define D4CD_CHECKBOX_UNCHECKED_CHAR '_'

// definition of check box left border focus char - zero value hide the check box border char
// #define D4CD_CHECKBOX_FOCUS_CHAR 0 //'>'

// definition of check box left border char - zero value hide the check box border char
// #define D4CD_CHECKBOX_UNFOCUS_CHAR 0 //' '
  
/***********************
* Sizes constants
***********************/

// definition of check box text offset to CHECKED/UNCHECKED char
// #define D4CD_CHECKBOX_TEXT_OFFSET 1

/******************************************************************************
* Label Object
******************************************************************************/

/***********************
* Properties
***********************/

// the default label system flags
//#define #define D4CD_LBL_F_DEFAULT  (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED)

// the default button  text properties
//#define D4CD_LBL_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)


/***********************
* Sizes constants
***********************/


/******************************************************************************
* Menu Object
******************************************************************************/

/***********************
* Properties
***********************/

// the default menu system flags
//  #define D4CD_MENU_F_DEFAULT  (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED | D4CD_OBJECT_F_TABSTOP | D4CD_MENU_F_INDEX )

// the default menu title text properties
//  #define D4CD_MENU_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)

// the default menu index text properties
//  #define D4CD_MENU_IX_TXT_PRTY_DEFAULT  ( D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)

// the default menu items text properties
//  #define D4CD_MENU_ITEM_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_LEFT_MASK)

// definition of menu char that printed on last separating line
//  #define D4CD_MENU_ENDMENULINE_CHAR '-'

// definition of menu char that printed for non selected menu lines
//  #define D4CD_MENU_SIDEBAR_NORM_CHAR '|'

// definition of menu char that printed for selected menu lines
//  #define D4CD_MENU_SIDEBAR_FOCUS_CHAR '<'



/******************************************************************************
* ListBox Object
******************************************************************************/

/***********************
* Properties
***********************/

// the default list box system flags
//  #define D4CD_LIST_BOX_F_DEFAULT  (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED | D4CD_OBJECT_F_TABSTOP | D4CD_OBJECT_F_CURSOR_EN)

// the default list box title text properties
//  #define D4CD_LIST_BOX_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)

// the default list box items text properties
//  #define D4CD_LISTBOX_ITEM_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)

// definition of list box left border focus char - zero value hide the list box border char
//  #define D4CD_LISTBOX_SEL_FOCUS_CHAR '>'

// definition of list box left border normal char - zero value hide the list box border char
//  #define D4CD_LISTBOX_SEL_NORMAL_CHAR ' '

// definition of key (key scan code) that is used for change to next item in list
//  #define D4CD_LISTBOX_KEY_SCANCODE_PLUS D4CD_KEY_SCANCODE_UP

// definition of key (key scan code) that is used for change to previous item in list
//  #define D4CD_LISTBOX_KEY_SCANCODE_MINUS D4CD_KEY_SCANCODE_DOWN

/***********************
* Sizes constants
***********************/

 /******************************************************************************
* Number Object
******************************************************************************/

/***********************
* Properties
***********************/

// the default number system flags
// #define D4CD_NUMBER_F_DEFAULT  (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED | D4CD_OBJECT_F_TABSTOP | D4CD_OBJECT_F_CURSOR_EN )

// the default number title text properties
// #define D4CD_NUMBER_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)

// the default number item text properties
// #define D4CD_NUMBER_NUM_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)

// definition of number left border focus char - zero value hide the number border char
// #define D4CD_NUMBER_SEL_FOCUS_CHAR '>'

// definition of number left border normal char - zero value hide the number border char
// #define D4CD_NUMBER_SEL_NORMAL_CHAR ' '

// definition of number decimal point char
// #define D4CD_NUMBER_DEC_POINT_CHAR '.'

// definition of key (key scan code) that is used for increment change
// #define D4CD_NUMBER_KEY_SCANCODE_PLUS D4CD_KEY_SCANCODE_UP

// definition of key (key scan code) that is used for decrement change
// #define D4CD_NUMBER_KEY_SCANCODE_MINUS D4CD_KEY_SCANCODE_DOWN

// definition of default size of change
// #define D4CD_NUMBER_CHANGE_STEP 5


 /******************************************************************************
* Time Object
******************************************************************************/

/***********************
* Properties
***********************/

// the default time system flags
// #define D4CD_TIME_F_DEFAULT  (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED | D4CD_OBJECT_F_TABSTOP | D4CD_OBJECT_F_CURSOR_EN )

// definition of time left border focus char - zero value hide the time border char
// #define D4CD_TIME_SEL_FOCUS_CHAR '>'

// definition of time left border normal char - zero value hide the time border char
// #define D4CD_TIME_SEL_NORMAL_CHAR ' '

// definition of time separator minute char
// #define D4CD_TIME_HOUR_MIN_CHAR ':'

// definition of time separator second char
// #define D4CD_TIME_MIN_SEC_CHAR '.'

// definition of key (key scan code) that is used for increment change
// #define D4CD_TIME_KEY_SCANCODE_PLUS D4CD_KEY_SCANCODE_UP

// definition of key (key scan code) that is used for decrement change
// #define D4CD_TIME_KEY_SCANCODE_MINUS D4CD_KEY_SCANCODE_DOWN
  
  
/******************************************************************************
* Date Object
******************************************************************************/

/***********************
* Properties
***********************/

// the default date system flags
// #define D4CD_DATE_F_DEFAULT  (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED | D4CD_OBJECT_F_TABSTOP | D4CD_OBJECT_F_CURSOR_EN )

// definition of date left border focus char - zero value hide the date border char
// #define D4CD_DATE_SEL_FOCUS_CHAR '>'

// definition of date left border normal char - zero value hide the date border char
// #define D4CD_DATE_SEL_NORMAL_CHAR ' '

// definition of date separator char
// #define D4CD_DATE_DIVIDER_CHAR '/'

// definition of key (key scan code) that is used for increment change
// #define D4CD_DATE_KEY_SCANCODE_PLUS D4CD_KEY_SCANCODE_UP

// definition of key (key scan code) that is used for decrement change
// #define D4CD_DATE_KEY_SCANCODE_MINUS D4CD_KEY_SCANCODE_DOWN

// definition of start year value (the year information is stored in 8 bit variable)
// #define D4CD_DATE_YEAR_MIN 2000

// definition of end year value (the year information is stored in 8 bit variable)
// #define D4CD_DATE_YEAR_MAX 2099

  
/******************************************************************************
* Password Object
******************************************************************************/

/***********************
* Properties
***********************/


// the default password system flags
// #define D4CD_PSWRD_F_DEFAULT  (D4CD_OBJECT_F_VISIBLE | D4CD_OBJECT_F_ENABLED | D4CD_OBJECT_F_TABSTOP | D4CD_OBJECT_F_CURSOR_EN)

// the default password text properties
// #define D4CD_PSWRD_TXT_PRTY_DEFAULT  (D4CD_TXT_PRTY_ALIGN_H_CENTER_MASK)

// definition of password left border focus char - zero value hide the password border char
// #define D4CD_PSWRD_SEL_FOCUS_CHAR '>'

// definition of password left border normal char - zero value hide the password border char
// #define D4CD_PSWRD_SEL_NORMAL_CHAR ' '

// definition of password hide password char
// #define D4CD_PSWRD_HIDEPSWRD_CHAR '*'

// definition of key (key scan code) that is used for increment change
// #define D4CD_PSWRD_KEY_SCANCODE_PLUS D4CD_KEY_SCANCODE_UP

// definition of key (key scan code) that is used for decrement change
// #define D4CD_PSWRD_KEY_SCANCODE_MINUS D4CD_KEY_SCANCODE_DOWN

 
#endif /* __D4CD_USER_CFG_H */