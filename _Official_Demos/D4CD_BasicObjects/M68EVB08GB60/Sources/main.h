#ifndef MAIN_H
#define MAIN_H

//#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#include "d4cd.h"
#include "d4cd_lldapi.h"
#include "user_chars.h"

// typedef definitions

typedef union
{
  D4CD_BIT_FIELD all;
  
  struct
  {
    unsigned b25ms :1;
    unsigned b50ms : 1;
    unsigned b100ms : 1;
    unsigned b200ms : 1;
    unsigned b400ms : 1;
    unsigned b800ms : 1;
    unsigned b1600ms : 1;    
    unsigned b3200ms : 1;
  }bits;
  
}TIME_FLAGS;



// global variables externs
extern TIME_FLAGS time;
extern LWord time100sm_cnt;

#endif MAIN_H