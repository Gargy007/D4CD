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
* @file      d4cd_base.h
*
* @author    Petr Gargulak
* 
* @version   0.0.1.0
* 
* @date      Jan-6-2011
* 
* @brief     D4CD Driver core and base functions header file
*
*******************************************************************************/

#ifndef __D4CD_BASE_H
#define __D4CD_BASE_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Constants
******************************************************************************/

/*********************************************************
*
* D4CD constants 
*
*********************************************************/

#define D4CD_LINE_THIN           0x00
#define D4CD_LINE_THICK          0x01

/*********************************************************
*
* general helper constants
*
*********************************************************/

#ifndef D4CD_MCU_TYPE
  #define D4CD_MCU_TYPE D4CD_MCF51
  #warning "There is no definition for D4CD_MCU_TYPE in configuration file. The driver is using as default D4CD_MCF51."
#endif    

#ifndef D4CD_SCREEN_HISTORY
  #define D4CD_SCREEN_HISTORY 5
#endif 

#ifndef D4CD_ENABLE_AUTOSIZE
  #define D4CD_ENABLE_AUTOSIZE D4CD_FALSE
#endif
  
#ifdef D4CD_SCREEN_CHAR_CNT
  #define D4CD_SCREEN_SIZE_X  D4CD_SCREEN_CHAR_CNT
#else
  #define D4CD_SCREEN_CHAR_CNT 16
  #define D4CD_SCREEN_SIZE_X  16
  #warning "The D4CD configuration file doesn't contains the information of char count. The D4CD sets this value to default: 16 chars."
#endif

#ifdef D4CD_SCREEN_LINE_CNT
  #define D4CD_SCREEN_SIZE_Y  D4CD_SCREEN_LINE_CNT
#else
  #define D4CD_SCREEN_LINE_CNT 2
  #define D4CD_SCREEN_SIZE_Y  2
  #warning "The D4CD configuration file doesn't contains the information of LCD line count. The D4CD sets this value to default: 2 lines."
#endif

#ifndef D4CD_MCU_BUS_CLOCK 
  #define D4CD_MCU_BUS_CLOCK 24000000L
#endif  
  
/*********************************************************
*
* keys setting  constants
*
*********************************************************/



#ifndef D4CD_KEY_UP
  #define D4CD_KEY_UP     0x01
#endif
#ifndef D4CD_KEY_DOWN
  #define D4CD_KEY_DOWN   0x02
#endif
#ifndef D4CD_KEY_LEFT  
  #define D4CD_KEY_LEFT   0x04
#endif
#ifndef D4CD_KEY_RIGHT  
  #define D4CD_KEY_RIGHT  0x08
#endif
#ifndef D4CD_KEY_ENTER  
  #define D4CD_KEY_ENTER  0x10
#endif
#ifndef D4CD_KEY_ESC
  #define D4CD_KEY_ESC    0x20
#endif


#ifndef D4CD_KEY_SCANCODE_UP
  #define D4CD_KEY_SCANCODE_UP     0x51
#endif

#ifndef D4CD_KEY_SCANCODE_DOWN
  #define D4CD_KEY_SCANCODE_DOWN   0x50
#endif

#ifndef D4CD_KEY_SCANCODE_LEFT  
  #define D4CD_KEY_SCANCODE_LEFT   0x4B
#endif

#ifndef D4CD_KEY_SCANCODE_RIGHT  
  #define D4CD_KEY_SCANCODE_RIGHT  0x4D
#endif

#ifndef D4CD_KEY_SCANCODE_ENTER  
  #define D4CD_KEY_SCANCODE_ENTER  0x1C
#endif

#ifndef D4CD_KEY_SCANCODE_ESC
  #define D4CD_KEY_SCANCODE_ESC    0x01
#endif

#define D4CD_KEY_SCANCODE_KEYMASK  0x7F
#define D4CD_KEY_SCANCODE_RELEASEMASK  0x80 

#ifndef D4CD_KEY_FUNC_FOCUS_NEXT
  #define D4CD_KEY_FUNC_FOCUS_NEXT    D4CD_KEY_SCANCODE_DOWN
#endif 

#ifndef D4CD_KEY_FUNC_FOCUS_PREV
  #define D4CD_KEY_FUNC_FOCUS_PREV    D4CD_KEY_SCANCODE_UP
#endif 


#ifndef D4CD_KEYS_BUFF_LENGTH
  #define D4CD_KEYS_BUFF_LENGTH 4
#endif 


/******************************************************************************
* Types
******************************************************************************/

/*********************************************************
*
* general helper types
*
*********************************************************/

#define D4CD_BITFIELD_LSB_ALLIGMENT_RIGHT 0
#define D4CD_BITFIELD_LSB_ALLIGMENT_LEFT  1


#ifndef D4CD_BITFIELD_LSB_ALIGNMENT
    #define D4CD_BITFIELD_LSB_ALIGNMENT D4CD_BITFIELD_LSB_ALLIGMENT_RIGHT
#endif

#ifndef D4CD_USER_CHAR_TABLE_DEFAULT
  #define D4CD_USER_CHAR_TABLE_DEFAULT NULL
#endif  


#ifndef D4CD_BITFIELD_SHIFT 
  #define D4CD_BITFIELD_SHIFT 0
#endif   

#ifndef D4CD_BLINK_MODULO 
  #define D4CD_BLINK_MODULO 4
#endif



typedef union
{
  D4CD_BIT_FIELD all;
  struct 
  {
    unsigned bTimeTick:1;
  }bits;  
}D4CD_SYSTEM_FLAGS;




typedef struct
{
  D4CD_POINT pos;
  struct
  {
    unsigned bVisible :1;
    unsigned bBlink   :1;
    unsigned bUpdate  :1;    
  }state;
}D4CD_CURSOR;

#ifndef D4CD_BLINK_CNT
  typedef Byte D4CD_BLINK_CNT;
#endif  


typedef union
{
  D4CD_BIT_FIELD all;
  struct 
  {
    unsigned bBlink     :1;
    unsigned bChange    :1;
  }bits; 
}D4CD_BLINK_STATE;

typedef struct
{
  D4CD_BLINK_CNT cnt;
  D4CD_BLINK_CNT mod;
  D4CD_BLINK_STATE state;
}D4CD_BLINK;

typedef struct
{
  D4CD_KEY_SCANCODE buff[D4CD_KEYS_BUFF_LENGTH];
  Byte readPos;
  Byte writePos;
} D4CD_KEYS_BUFFER;


typedef union D4CD_OBJECT_DRAWFLAGS_S
{
    D4CD_BIT_FIELD all;
    
    struct
    {
        unsigned bComplete  :1;   // complete redraw
        unsigned bFocused   :1;    // draw as focused
        unsigned bCapturing :1;  // draw as active (capturing keys)        
    } bits;
} D4CD_OBJECT_DRAWFLAGS;

/*********************************************************
*
* object messages
*
*********************************************************/

typedef enum 
{
    D4CD_MSG_DRAW,           // parameter wprm_DRAW
    D4CD_MSG_DRAWDONE,       // parameter wprm_DRAW
    D4CD_MSG_KEYUP,          // parameter wprm_KEYS
    D4CD_MSG_KEYDOWN,        // parameter wprm_KEYS
    D4CD_MSG_SETFOCUS,       // no parameter
    D4CD_MSG_KILLFOCUS,      // no parameter
    D4CD_MSG_SETCAPTURE,     // no parameter
    D4CD_MSG_KILLCAPTURE,    // no parameter
    D4CD_MSG_ONINIT,         // no parameter
    D4CD_MSG_TIMETICK        // no parameter      
} D4CD_MSGID;

/* the message object */

struct D4CD_OBJECT_S;

typedef struct D4CD_MESSAGE_S
{
    struct D4CD_OBJECT_S* pObject;
    struct D4CD_SCREEN_S* pScreen;
    D4CD_MSGID   nMsgId;  
     
    union
    {
        D4CD_OBJECT_DRAWFLAGS draw;
        D4CD_KEY_SCANCODE key;      // there will be stored only code of key without release / press information
        D4CD_BLINK_STATE blink;                           // this information is handlad by message ID 
        
    } prm;
    
} D4CD_MESSAGE;

#define D4CD_MSG_NOSKIP 0
#define D4CD_MSG_SKIP 1


/******************************************************************************
* Macros 
******************************************************************************/



/*********************************************************
*
* internal macros
*
*********************************************************/

#define D4CD_UNUSED(x) (void)(x); 
#define D4CD_CONST const
#define D4CD_NO_CONST 


/*********************************************************
*
* static declaration macros
*
*********************************************************/

/******************************************************************************
* Global variables
******************************************************************************/
   
// global shared memory used by multiple modules as temporary storage
#define D4CD_SCRATCHPAD_SIZE 32
extern Byte d4cd_scratchPad[D4CD_SCRATCHPAD_SIZE];



// assert macro
#if (D4CD_SANITY_CHECK !=0 )
#define D4CD_ASSERT(cond)  if(!(cond)) for(;;)
#else
  #define D4CD_ASSERT(cond)  (cond);
#endif

/******************************************************************************
* Global functions
******************************************************************************/

#ifdef D4CD_INPUT_EVENT_CALLBACK
   extern void D4CD_INPUT_EVENT_CALLBACK(void);
#endif

/*********************************************************
*
* internal API
*
*********************************************************/


#endif /* __D4CD_BASE_H */










