////#include <stdint.h>
////#include "iostm8s103f3.h"
///#include "my_misc.h"
#include "../brd/enc6701_brd.h"
#include "timers.h"
#include "adc.h"
#include "softuart.h"

static uint32_t cur_ms=0;
uint32_t get_curr_ms(void)
{
  return cur_ms;
}
#if 0
void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast"))); /* Interrupt handler function */
void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetFlagStatus(TIM1, TIM_FLAG_Update) != RESET)
     {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        cur_ms++;
     }
}
#endif
void TIM1_Config(void)
{
NVIC_InitTypeDef NVIC_InitStructure={0};
TIM_TimeBaseInitTypeDef TIM_TimeInitStructure={0};
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

////TIM_DeInit(TIM1);
TIM_TimeInitStructure.TIM_ClockDivision = TIM_CKD_DIV2;///TIM_CKD_DIV4;
TIM_TimeInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeInitStructure.TIM_Period = SUART_TIM_PERIOD;
TIM_TimeInitStructure.TIM_Prescaler = SUART_TIM_PRESC;
////TIM_TimeInitStructure.TIM_RepetitionCounter = 0;
TIM_TimeBaseInit(TIM1, &TIM_TimeInitStructure);

////TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
////TIM_ARRPreloadConfig(TIM1, ENABLE);

NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_Init(&NVIC_InitStructure);
TIM_Cmd(TIM1,ENABLE);
}
#if 0
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
if (TIM1_GetITStatus(TIM1_IT_UPDATE)) {
  TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
  cur_ms++;
  }

}
#endif
#if 1
void obr_timer2(void);

void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast"))); /* Interrupt handler function */
void TIM2_IRQHandler(void)
{
///    if (TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) != RESET)
   if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        obr_timer2();
     }
}

void TIM2_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure={0};
    TIM_TimeBaseInitTypeDef TIM_TimeInitStructure={0};
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    ////TIM_DeInit(TIM2);
    TIM_TimeInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeInitStructure.TIM_Period = TIM2_PERIOD;
    TIM_TimeInitStructure.TIM_Prescaler = TIM2_PRESC;
 ////   TIM_TimeInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeInitStructure);

  ////  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
   //// TIM_ARRPreloadConfig(TIM2, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM2,ENABLE);

#if 0
    TIM_OCInitTypeDef TIM_OCInitstructure={0};
    TIM_OCInitstructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitstructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitstructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitstructure.TIM_Pulse = 0x5;
    TIM_OC1Init(TIM2, &TIM_OCInitstructure);

    TIM_OC1PreloadConfig(SoftUARTHardware.txTIM, TIM_OCPreload_Disable);
    TIM_ARRPreloadConfig(SoftUARTHardware.txTIM, ENABLE);
#endif

#if 0
TIM2_TimeBaseInit(TIM2_PRESC,TIM2_PERIOD);
////TIM2_OC2Init(TIM2_OCMODE_TIMING, TIM2_OUTPUTSTATE_DISABLE, TIM2_PERIOD/4, TIM2_OCPOLARITY_HIGH);

TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
TIM2_Cmd(ENABLE);
#endif
}
#endif
///=======================================
void obr_timer2(void)
{
////static uint8_t cnt_bled=0;  
obr_adc_dat(); 
}


