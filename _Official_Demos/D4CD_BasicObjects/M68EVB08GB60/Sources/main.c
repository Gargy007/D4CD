
#include "main.h"


byte i = 0; 



D4CD_EXTERN_SCREEN(scr_test);
D4CD_EXTERN_OBJECT(tstTimeScr_Time);

void MCU_init(void)
{
  /* ### MC9S08GB60 "Cpu" init code ... */
  /*  PE initialization code after reset */
  /* Common initialization of the write once registers */
  /* SOPT: COPE=0,COPT=1,STOPE=0,BKGDPE=1 */
  SOPT = 0x53;                                      
  /* SPMSC1: LVDF=0,LVDACK=0,LVDIE=0,LVDRE=1,LVDSE=1,LVDE=1 */
  SPMSC1 = 0x1C;                                      
  /* SPMSC2: LVWF=0,LVWACK=0,LVDV=0,LVWV=0,PPDF=0,PPDACK=0,PDC=0,PPDC=0 */
  SPMSC2 = 0x00;                                      
  /*  System clock initialization */
  /* ICGC1: RANGE=1,REFS=0,CLKS1=0,CLKS0=1,OSCSTEN=1 */
  ICGC1 = 0x4C;                                      
  /* ICGC2: LOLRE=0,MFD2=1,MFD1=1,MFD0=1,LOCRE=0,RFD2=0,RFD1=0,RFD0=0 */
  ICGC2 = 0x70;                                      
  ICGTRM = *(unsigned char*far)0xFFBE; /* Initialize ICGTRM register from a non volatile memory */
  while(!ICGS1_LOCK) {                 /* Wait */
  }
}

void TPM1_Init(void)
{
  //25ms overflow
  TPM1SC = TPM1SC_TOIE_MASK | TPM1SC_CLKSA_MASK | TPM1SC_PS_MASK; // overflow interrupt enebled, bus clock and prescaler is set to 128
  TPM1MOD = (D4CD_MCU_BUS_CLOCK / 128 / (1 / 0.025));
  
  (void)(TPM1SC);
  TPM1SC_TOF = 1;  
}

void Keys_Init(void)
{
  PTADD &= ~0xf0;
  PTAPE |= 0xf0;
}

Byte Keys_Get(void)
{
  return ((PTAD >> 4) & 0x0f);
}


void Keys_Handle(void)
{
  #define KEYS_AUTOREPEAT_ENABLE_CNT 40
  #define KEYS_AUTOREPEAT_PERIOD_CNT 5
  static Byte last_state = 0;
  
  static Byte push_cnt[4] = {0, 0, 0, 0 };
  
  const D4CD_KEY_SCANCODE scan_codes[4] =
  {
    D4CD_KEY_SCANCODE_ENTER,
    D4CD_KEY_SCANCODE_ESC,
    D4CD_KEY_SCANCODE_UP,
    D4CD_KEY_SCANCODE_DOWN  
  };
  
  Byte state;
  Byte change;
  Byte i;
  
  
  state = Keys_Get();
  
  change = state ^ last_state;
  
  if(change)
  {
    for(i=0;i<4;i++)
    {
      if(change & 1<<i)
      {
        D4CD_KEY_SCANCODE code;
        
        code = scan_codes[i];
        if(state & 1<<i)
        {
          push_cnt[i] = 0;
          code |= D4CD_KEY_SCANCODE_RELEASEMASK;
        }
        D4CD_NewKeyEvent(code);
          
      }
    }
  }
  
  // auto repeat section
  if((state & 0x0f) != 0x0f)
  {
    for(i=0;i<4;i++)
    {
      if(!(state & 1<<i))
      {
        push_cnt[i]++;
        
        if(push_cnt[i] > KEYS_AUTOREPEAT_ENABLE_CNT) 
        {
          if(push_cnt[i] > KEYS_AUTOREPEAT_PERIOD_CNT + KEYS_AUTOREPEAT_ENABLE_CNT)
          {
            D4CD_NewKeyEvent(scan_codes[i]);
            D4CD_NewKeyEvent(scan_codes[i] | D4CD_KEY_SCANCODE_RELEASEMASK);
            push_cnt[i] = KEYS_AUTOREPEAT_ENABLE_CNT;  
          }
        }
          
      }
    }
  }
  
  last_state = state;

}

interrupt void TPM1_InterruptOverFlow(void)
{
  static byte int_cnt = 0;
  
  if(!(int_cnt % 4))
    D4CD_TimeTickPut();
  
  if(int_cnt++ > 10)
  {
    int_cnt = 0;
    D4CD_TimeAddSecond((D4CD_OBJECT*)&tstTimeScr_Time);
  }
  
  Keys_Handle();
  
  (void)(TPM1SC);
  TPM1SC_TOF = 0;
}



void main(void) {

  MCU_init();
  Keys_Init();
  TPM1_Init();
  if(!D4CD_Init(&scr_test))
    for(;;);
  
  asm CLI; //Enable Interrupts

  for(;;) {
    D4CD_Poll();
  } /* loop forever */
  /* please make sure that you never leave main */
}
