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
* @file      d4cd_math.h
*
* @author    Petr Gargulak
* 
* @version   0.0.1.0
* 
* @date      Jan-6-2011
* 
* @brief     D4CD Driver math helper functions header file
*
*******************************************************************************/

#ifndef __D4CD_MATH_H
#define __D4CD_MATH_H

/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Types
******************************************************************************/



/******************************************************************************
* Macros 
******************************************************************************/

// convert deg angle of -180..180 to internal representation (-128..127)
#define D4CD_ANGLE_DEG(deg) (((deg)==180)?0xff:((deg)*0x100/180))

/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/

sByte D4CD_Sin(Byte x);
sByte D4CD_Cos(Byte x);

Byte D4CD_MulDivUU8(Byte u1, Byte u2, Byte d);
sByte D4CD_MulDivSU8(sByte s, Byte u, Byte d);

Word D4CD_MulDivUU16(Word u1, Word u2, Word d);
sWord D4CD_MulDivSU16(sWord s, Word u, Word d);

Word D4CD_Abs16(sWord num);

Byte D4CD_Abs8(sByte num);

#ifdef D4CD_COOR_TYPE
  
  #define D4CD_MulDivUU  D4CD_MulDivUU16
  #define D4CD_MulDivSU  D4CD_MulDivSU16
#else
  #define D4CD_MulDivUU  D4CD_MulDivUU8
  #define D4CD_MulDivSU  D4CD_MulDivSU8
#endif

#endif /* __D4CD_MATH_H */