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
* @file      d4cdlcd_template.c
*
* @author    Petr Gargulak
* 
* @version   0.0.1.0
* 
* @date      Jan-6-2011
* 
* @brief     D4CD driver - template lcd driver source c file 
*
******************************************************************************/

#include "d4cd.h"            // include of all public items (types, function etc) of D4CD driver
#include "d4cd_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "d4cd_private.h"    // include the private header file that contains perprocessor macros as D4CD_MK_STR


// identification string of driver - must be same as name D4CDLCD_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4cdlcd_template_ID 1


// copilation enable preprocessor condition
// the string d4cdlcd_template_ID must be replaced by define created one line up
#if (D4CD_MK_STR(D4CD_LLD_LCD) == d4cdlcd_template_ID)
  
  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4CD configuration file
  #include "d4cdlcd_template.h"
  /******************************************************************************
  * Macros 
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes 
  ******************************************************************************/

  static unsigned char D4CDLCD_Init_Template(void);
  static unsigned char D4CDLCD_GoTo_Template(unsigned char x, unsigned char y);
  static unsigned char D4CDLCD_Send_Char_Template(unsigned char value) ;
  static unsigned char D4CDLCD_Read_Char_Template(void);  
  static unsigned char D4CDLCD_WriteUserChar_Template(unsigned char ix, unsigned char *pData);  
  static void D4CDLCD_Flush_Template(void);
  static void D4CDLCD_Delay_ms_Template(unsigned short period);
  static unsigned char D4CDLCD_DeInit_Template(void);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/
  
  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4CD
  // so this name has to be used in main configuration header file of D4CD driver to enable this driver
  const D4CDLCD_FUNCTIONS d4cdlcd_template = 
  {
    D4CDLCD_Init_Template,    
    D4CDLCD_GoTo_Template,
    D4CDLCD_Send_Char_Template,
    D4CDLCD_Read_Char_Template,
    D4CDLCD_WriteUserChar_Template,
    D4CDLCD_Flush_Template,
    D4CDLCD_Delay_ms_Template,
    D4CDLCD_DeInit_Template,
  };

  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/

  /**************************************************************//*!
  *
  * Functions bodies
  *
  ******************************************************************/

   
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCD_Init_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4CDLCD_Init_Template(void)
  {
    
  }
     
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCD_DeInit_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
   //-----------------------------------------------------------------------------  
  static unsigned char D4CDLCD_DeInit_Template(void)
  {
    
  }    
  
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCD_GoTo_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function sets start coordination in memory of LCD driver
  // 
  // PARAMETERS:  unsigned char x - left cordination 
  //              unsigned char y - top cordination
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
    //----------------------------------------------------------------------------- 
  static unsigned char D4CDLCD_GoTo_Template(unsigned char x, unsigned char y)
  {
    
  }

  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCD_Send_Char_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one char into LCD  
  //
  // PARAMETERS:  unsigned char ch    char to print
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed                
  //-----------------------------------------------------------------------------
  static unsigned char D4CDLCD_Send_Char_Template(unsigned char ch)
  {       
  
  }
  
 
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCD_Read_Char_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one char from LCD (if this function is supported)  
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned char - the readed char
  //              
  //-----------------------------------------------------------------------------
  static unsigned char D4CDLCD_Read_Char_Template(void)
  {       
  
  }
  
    //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCD_WriteUserChar_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function load new char into char LCD display
  // 
  // PARAMETERS:  unsigned char ix - internal index of user char table  
  //              unsigned char *pData - pointer of new char data
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
    //----------------------------------------------------------------------------- 
  static unsigned char D4CDLCD_WriteUserChar_Template(unsigned char ix, unsigned char *pData)
  {
    
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCD_Flush_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For buffered low level interfaces is used to inform
  //              driver the complete object is drawed and pending pixels should be flushed
  //
  // PARAMETERS:  none
  //              
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4CDLCD_Flush_Template(void)
  {       

  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCD_Delay_ms_Template
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For do some small delays in ms
  //
  // PARAMETERS:  period - count of ms
  //              
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4CDLCD_Delay_ms_Template(unsigned short period)
  {       

  }


#endif //(D4CD_MK_STR(D4CD_LLD_LCD) == d4cdlcd_template_ID)