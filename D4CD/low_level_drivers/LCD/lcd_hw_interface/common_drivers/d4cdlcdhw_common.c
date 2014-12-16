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
* @file      d4cdlcdhw_common.c
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Feb-14-2011
* 
* @brief     D4CD driver - common general low level drivers c file 
*
******************************************************************************/



#include "d4cd.h"
#include "d4cd_lldapi.h"
#include "d4cd_private.h"
  
#include "d4cdlcdhw_common.h"
#if (D4CD_MCU_TYPE != D4CD_KINETIS)
#include "derivative.h"
#endif
  /******************************************************************************
  * Macros 
  ******************************************************************************/


  /******************************************************************************
  * Functions definitions
  ******************************************************************************/

  /**************************************************************//*!
  *
  * Public variables
  *
  ******************************************************************/  
  
  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/

  /**************************************************************//*!
  *
  * Initialize
  *
  ******************************************************************/

  void D4CDLCDHW_CommonInit(void)
  {
     
  }
  






  #define MCU_BUS_CYCLES_100US (D4CD_MCU_BUS_CLOCK / 10000) 
  #define MCU_BUS_CYCLES_20US (D4CD_MCU_BUS_CLOCK / 50000) 
  
  
  #if (D4CD_MCU_TYPE == D4CD_HCS08)
  
  #define MCU_BUS_CYCLES_20US_WITHOUT_OVERHEAD (MCU_BUS_CYCLES_20US - 18)
  #define DELAY20US_LOOP_CNT ((MCU_BUS_CYCLES_20US_WITHOUT_OVERHEAD / 5))

  void Cpu_Delay20US(Word us20)
  {
    (void)us20;
    /* Total irremovable overhead: 16 cycles */
    /* ldhx: 5 cycles overhead (load parameter into register) */
    /* jsr:  5 cycles overhead (jump to subroutine) */
    /* rts:  6 cycles overhead (return from subroutine) */

    /* aproximate irremovable overhead for each 100us cycle (counted) : 8 cycles */
    /* aix:  2 cycles overhead  */
    /* cphx: 3 cycles overhead  */
    /* bne:  3 cycles overhead  */
    asm {
  loop:
      /* 20 us delay block begin */
      /*
       * Delay
       *   - requested                  : 100 us @ 25.165824MHz,
       *   - possible                   : 2517 c, 100016.59 ns, delta 16.59 ns
       *   - without removable overhead : 2509 c, 99698.7 ns
       */
      pshh                               /* (2 c: 79.47 ns) backup H */
      pshx                               /* (2 c: 79.47 ns) backup X */
      ldhx #(DELAY20US_LOOP_CNT)               /* (3 c: 119.21 ns) number of iterations */
  label0:
      aix #-1                            /* (2 c: 79.47 ns) decrement H:X */
      cphx #0                            /* (3 c: 119.21 ns) compare it to zero */
      bne label0                         /* (3 c: 119.21 ns) repeat 312x */
      pulx                               /* (3 c: 119.21 ns) restore X */
      pulh                               /* (3 c: 119.21 ns) restore H */
      /* 20 us delay block end */
      aix #-1                            /* us100 parameter is passed via H:X registers */
      cphx #0
      bne loop                           /* next loop */
      rts                                /* return from subroutine */
    }
  }
  
  #elif (D4CD_MCU_TYPE == D4CD_MCF51)
  
  #define MCU_BUS_CYCLES_20US_WITHOUT_OVERHEAD (MCU_BUS_CYCLES_20US - 0)
  #define DELAY20US_LOOP_CNT ((MCU_BUS_CYCLES_20US_WITHOUT_OVERHEAD / 1))
  
  //#0x031E pro 24MHz a 100us
  
  __declspec(register_abi) void Cpu_Delay20US(Word us20)
  {
    #pragma unused(us20)
      /* irremovable one time overhead (ignored): 11 cycles */
    /* move: 1 cycle overhead (load parameter into register) */
    /* jsr:  3 cycles overhead (jump to subroutine) */
    /* andi: 1 cycle overhead (clear upper word of d0) */
    /* tpf: 1 cycle overhead (alignment) */
    /* rts:  5 cycles overhead (return from subroutine) */

    /* aproximate irremovable overhead for each 100us cycle (counted) : 3 cycles */
    /* subq.l 1 cycles overhead  */
    /* bne.b  2 cycles overhead  */

    /* Disable MISRA rule 55 checking - Non-case label used */
    /*lint -esym( 961, 55)   */
    asm {
      naked
      andi.l #0xFFFF,d0                  /* parameter is word - clear the rest of d0 register */
      tpf                                /* alignment */
  loop:
      /* 100 us delay block begin */
      /*
       * Delay
       *   - requested                  : 100 us @ 24MHz,
       *   - possible                   : 2400 c, 100000 ns
       *   - without removable overhead : 2397 c, 99875 ns
       */
      move.l #DELAY20US_LOOP_CNT,d1                  /* (1 c: 41.67 ns) number of iterations */
  label0:
      subq.l #1,d1                       /* (1 c: 41.67 ns) decrement d1 */
      bne.b label0                       /* (2 c: 83.33 ns) repeat 798x */
      tpf                                /* (1 c: 41.67 ns) wait for 1 c */
      /* 100 us delay block end */
      subq.l #1,d0                       /* parameter is passed via d0 register */
      bne.w loop                         /* next loop */
      rts                                /* return from subroutine */
    }
    /* Restore MISRA rule 55 checking - Non-case label used */
    /*lint +esym( 961, 55)   */
  }
  
  #elif (D4CD_MCU_TYPE == D4CD_KINETIS)
  void Cpu_Delay20US(Word us20)
  {
    // Core clock ussually 2x faster then bus clock
    unsigned long i = MCU_BUS_CYCLES_20US * us20;
    while(i--){};
      
  }

  #else
    #error "Unsupported MCU type for delay 20us loop in low level common driver!"
  #endif  
  
  #if (D4CD_MCU_TYPE == D4CD_HCS08)
  
  #define MCU_BUS_CYCLES_100US_WITHOUT_OVERHEAD (MCU_BUS_CYCLES_100US - 18)
  #define DELAY_LOOP_CNT ((MCU_BUS_CYCLES_100US_WITHOUT_OVERHEAD / 5))

  static void Cpu_Delay100US(Word us100)
  {
    (void)us100;
    /* Total irremovable overhead: 16 cycles */
    /* ldhx: 5 cycles overhead (load parameter into register) */
    /* jsr:  5 cycles overhead (jump to subroutine) */
    /* rts:  6 cycles overhead (return from subroutine) */

    /* aproximate irremovable overhead for each 100us cycle (counted) : 8 cycles */
    /* aix:  2 cycles overhead  */
    /* cphx: 3 cycles overhead  */
    /* bne:  3 cycles overhead  */
    asm {
  loop:
      /* 100 us delay block begin */
      /*
       * Delay
       *   - requested                  : 100 us @ 25.165824MHz,
       *   - possible                   : 2517 c, 100016.59 ns, delta 16.59 ns
       *   - without removable overhead : 2509 c, 99698.7 ns
       */
      pshh                               /* (2 c: 79.47 ns) backup H */
      pshx                               /* (2 c: 79.47 ns) backup X */
      ldhx #(DELAY_LOOP_CNT)               /* (3 c: 119.21 ns) number of iterations */
  label0:
      aix #-1                            /* (2 c: 79.47 ns) decrement H:X */
      cphx #0                            /* (3 c: 119.21 ns) compare it to zero */
      bne label0                         /* (3 c: 119.21 ns) repeat 312x */
      pulx                               /* (3 c: 119.21 ns) restore X */
      pulh                               /* (3 c: 119.21 ns) restore H */
      /* 100 us delay block end */
      aix #-1                            /* us100 parameter is passed via H:X registers */
      cphx #0
      bne loop                           /* next loop */
      rts                                /* return from subroutine */
    }
  }
 

 #elif (D4CD_MCU_TYPE == D4CD_HCS12) || (D4CD_MCU_TYPE == D4CD_HCS12X)
   
   
   #define MCU_BUS_CYCLES_100US_WITHOUT_OVERHEAD (MCU_BUS_CYCLES_100US - 13)

 
   
   #define DELAY_LOOP_CNT (MCU_BUS_CYCLES_100US_WITHOUT_OVERHEAD /3)
   
   #pragma CODE_SEG __NEAR_SEG NON_BANKED
   #pragma NO_ENTRY
   #pragma NO_EXIT
   #pragma MESSAGE DISABLE C5703
   
   static void Cpu_Delay100US(unsigned short us100)
  {
  /* irremovable overhead (ignored): 13 cycles */
  /* ldd:  2 cycles overhead (load parameter into register) */
  /* jsr:  4 cycles overhead (call this function) */
  /* rts:  7 cycles overhead (return from this function) */

  /* irremovable overhead for each 100us cycle (counted): 13 cycles */
  /* dbne:  3 cycles overhead (return from this function) */

  asm {
 loop:
      /* 100 us delay block begin */
      /*
       * Delay
       *   - requested                  : 100 us @ 40MHz,
       *   - possible                   : 4000 c, 100000 ns
       *   - without removable overhead : 3997 c, 99925 ns
       */
        pshd                               /* (2 c: 50 ns) backup D */
        ldd #1062     //#DELAY_LOOP_CNT                /* (2 c: 50 ns) number of iterations */
 label0:
        dbne d, label0                     /* (3 c: 75 ns) repeat 1330x */
        puld                               /* (3 c: 75 ns) restore D */
        /* 100 us delay block end */
        dbne d, loop                       /* us100 parameter is passed via D register */
        rts                                /* return from subroutine */
      }
    }
   #pragma CODE_SEG DEFAULT
 //===========================================================================

 
 #elif (D4CD_MCU_TYPE == D4CD_MCF51) || (D4CD_MCU_TYPE == D4CD_MCF52) 
 
 #define MCU_BUS_CYCLES_100US_WITHOUT_OVERHEAD (MCU_BUS_CYCLES_100US - 9)
 #define DELAY_LOOP_CNT (unsigned int)(MCU_BUS_CYCLES_100US_WITHOUT_OVERHEAD / 3)
 
  __declspec(register_abi) static void Cpu_Delay100US(unsigned int us100:__D0)
  {
    /* Total irremovable overhead: 9 cycles */
    /* move: 1 cycles overhead (load parameter into register) */
    /* jsr:  3 cycles overhead (jump to subroutine) */
    /* rts:  5 cycles overhead (return from subroutine) */

  #pragma unused(us100)
    asm {
      naked
  loop:
      /* 100 us delay block begin */

      /*
       * Delay
       *   - requested                  : 100 us @ 25.165824MHz,
       *   - possible                   : 2517 c, 100016.59 ns, delta 16.59 ns
       *   - without removable overhead : 2508 c, 99658.97 ns
       */
      move.l #DELAY_LOOP_CNT,d1                  /* (1 c: 39.74 ns) number of iterations */
  label0:
      subq.l #0x01,d1                    /* (1 c: 39.74 ns) decrement d1 */
      bne.b label0                       /* (2 c: 79.47 ns) repeat DELAY_LOOP_CNT x */
      tpf                                /* (1 c: 39.74 ns) wait for 1 c */

      /* 100 us delay block end */
      subq.l #0x01,d0                    /* parameter is passed via d0 register */
      bne.b loop                         /* next loop */
      rts                                /* return from subroutine */
    }
  }
#elif (D4CD_MCU_TYPE == D4CD_KINETIS)
  static void Cpu_Delay100US(unsigned short us100)
  {
    // Core clock ussually 2x faster then bus clock
    unsigned long i = MCU_BUS_CYCLES_100US * us100;

    while(i--){};
      
  }
  
#define  __RESET_WATCHDOG()
 #else
  #error "Unsupported MCU type for delay loop in loe level common driver!"
 
 #endif

  void D4CDLCD_Delay_ms_Common(unsigned short period)   //delay routine (milliseconds)
  {

      while (period != 0)
      {
          Cpu_Delay100US (10);
          __RESET_WATCHDOG(); /* feeds the dog */
          period--;    
      }
      
  }  
