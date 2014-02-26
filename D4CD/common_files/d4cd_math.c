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
* @file      d4cd_math.c
*
* @author    Petr Gargulak
* 
* @version   0.0.1.0
* 
* @date      Jan-6-2011
* 
* @brief     D4CD driver math functions c file 
*
******************************************************************************/

#include "d4cd.h"
#include "d4cd_private.h"

const sByte d4cd_sinTbl[64] = 
{
    0x00,
    0x03,
    0x06,
    0x09,
    0x0D,
    0x10,
    0x13,
    0x16,
    0x19,
    0x1C,
    0x1F,
    0x22,
    0x25,
    0x28,
    0x2B,
    0x2E,
    0x31,
    0x34,
    0x37,
    0x3A,
    0x3D,
    0x40,
    0x42,
    0x45,
    0x48,
    0x4A,
    0x4D,
    0x4F,
    0x52,
    0x54,
    0x56,
    0x59,
    0x5B,
    0x5D,
    0x5F,
    0x61,
    0x63,
    0x65,
    0x67,
    0x69,
    0x6B,
    0x6C,
    0x6E,
    0x70,
    0x71,
    0x72,
    0x74,
    0x75,
    0x76,
    0x77,
    0x78,
    0x79,
    0x7A,
    0x7B,
    0x7C,
    0x7C,
    0x7D,
    0x7D,
    0x7E,
    0x7E,
    0x7E,
    0x7E,
    0x7F,
    0x7F
};

/***************************************************************************//*!
*
* @brief  Fast 16/8=8 divide operation with rounding and saturation
*
* @param num The 16-bit numerator to be divided
* @param div The 8-bit denominator
*
* This function divides a given 16-bit value by a 8-bit divisor and returns
* 8-bit quotient. Half of the divisor is added to divident before the division
* so the result is rounded.
*
* @return This function returns the 8-bit quotient. Returned value is 255 
*         when overflow occurs.
*
******************************************************************************/
/*
#pragma NO_ENTRY 
#pragma NO_EXIT 
#pragma NO_RETURN 
#pragma NO_FRAME 
*/
static Byte D4CD_Divide8(Word num, Byte div)
{
    D4CD_UNUSED(num);
    D4CD_UNUSED(div);

#if (D4CD_MCU_TYPE == D4CD_HC08) || (D4CD_MCU_TYPE == D4CD_HCS08)
    asm
    {
        PSHA
        PSHX
        PSHH
        
        LSRA            /* div/2 */
        ADD   2,SP      /* add div/2 to num on the stack */
        STA   2,SP
        CLRA  
        ADC   1,SP
        STA   1,SP

        PULH            
        PULA            /* HA = num + div/2 */
        PULX            /* X = div */
        
        DIV             /* A=HA/X, H=HA%X */
        BCC ok          /* return A when okay */
        LDA #0xff       /* return 255 on overflow */
    ok: RTS 
    }

    /* ignored dead code (just for C syntax) */    
    return 0;
#elif (D4CD_MCU_TYPE == D4CD_HC12) || (D4CD_MCU_TYPE == D4CD_HCS12) || (D4CD_MCU_TYPE == D4CD_HCS12X)    
    return (Byte)(num/div);
#elif (D4CD_MCU_TYPE == D4CD_MCF51) || (D4CD_MCU_TYPE == D4CD_MCF52) || (D4CD_MCU_TYPE == D4CD_KINETIS)
    return (Byte)(num/div);
#else 
  #error "Not supported MCU for D4CD math divide!"
#endif

    // ignored dead code (just for C syntax)     

}

static Word D4CD_Divide16(LWord num, Word div)
{
  return (Word)(num/div);

}

Byte D4CD_MulDivUU8(Byte u1, Byte u2, Byte d)
{
    Word n = (Word)(((Word)u1 * (Word)u2) + (d / 2));
    return D4CD_Divide8(n, d);
}

sByte D4CD_MulDivSU8(sByte s, Byte u, Byte d)
{
    Word n;
    Byte r;
    
    if(s >= 0)
    {
        n = (Word)(((Byte)s * (Word)u) + (d / 2));
        r = D4CD_Divide8(n, d);
        return (sByte)((r > (Byte)127)? 127 : r);
    }
    else
    {
        n = (Word)((((Byte)-s) * (Word)u) + (d / 2));
        r = D4CD_Divide8(n, d);
        return (sByte)(r > 128 ? -128 : (sByte)(-1 * ((sByte)r)));
    }
}

Word D4CD_MulDivUU16(Word u1, Word u2, Word d)
{
    LWord n = (LWord)((LWord)((LWord)u1 * (LWord)u2) + (d / 2));
    return D4CD_Divide16(n, d);
}

sWord D4CD_MulDivSU16(sWord s, Word u, Word d)
{
    LWord n;
    Word r;
    
    if(s >= 0)
    {
        n = (LWord)(((Word)s * (LWord)u) + (d / 2));
        r = D4CD_Divide16(n, d);
        return (sWord)((r > (Word)0x7FFF)? 0x7FFF : r);
    }
    else
    {
        n = (LWord)((((Word)-s) * (LWord)u)) + (d / 2);
        r = D4CD_Divide16(n, d);
        return (sWord)(r > 0x8000 ? -(sWord)0x8000 : (sWord)(-1 * ((sWord)r)));
    }
}

Word D4CD_Abs16(sWord num) {
  if(num < 0)
    num = (sWord)(num * -1);
  
  return (Word)num;
}

Byte D4CD_Abs8(sByte num) {
  if(num < 0)
    num = (sByte)(num * -1);
  
  return (Byte)num;
}


/***************************************************************************//*!
*
* @brief  Sine calculation using lookup table
*
* @param x as unsigned integer <0..0xFF> ~ <0..2PI)
*
* This function calculates the sine value of argument phase 
* 64 x 8 bit look-up table. 
*
* @return signed sine value <-128..-1,0..127> ~ <0xFF..0x80,0x00..0x7F> ~ <-1,+1>
*
*****************************************************************************/

sByte D4CD_Sin(Byte x)
{
    if(x < 0x80)
    {
        if(x <= 0x3F)
            return d4cd_sinTbl[x];
        else
            return d4cd_sinTbl[0x3F-(x-0x40)];
    }
    else
    {
        if (x <= 0xBF)
            return (sByte)(-d4cd_sinTbl[x-0x80]);
        else
            return (sByte)(-d4cd_sinTbl[0x3F-(x-0xC0)]);
    }
}

sByte D4CD_Cos(Byte x)
{
    if(x < 0x80)
    {
        if(x <= 0x3F)
            return d4cd_sinTbl[0x3F-x];
        else
            return (sByte)(-d4cd_sinTbl[x-0x40]);
    }
    else
    {
        if (x <= 0xBF)
            return (sByte)(-d4cd_sinTbl[0x3F-(x-0x80)]);
        else
            return d4cd_sinTbl[x-0xC0];
    }
}

