#ifndef __TIMERS_H__
#define __TIMERS_H__

#define TIM1_PRESC 899 ///599  //// 999  ////159
#define TIM1_PERIOD 999

#define TIM2_PERIOD 35000///25000///699 ////25000
#define TIM2_PRESC  128 ////32     ////TIM2_PRESCALER_32 ///TIM2_PRESCALER_128          ///7

extern void TIM1_Config(void);
extern void TIM2_Config(void);

#endif //// __TIMERS_H__
