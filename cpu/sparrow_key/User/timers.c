#include "../brd/sparrow_brd.h"
#include "timers.h"
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

