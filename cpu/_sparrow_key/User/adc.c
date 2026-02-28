////#include <stdint.h>
////#include "iostm8s103f3.h"
#include "my_misc.h"

///#include "../brd/rook_u3_key_brd.h"

uint32_t summ_adc_dat=0;
uint32_t num_adc_dat=0;
volatile unsigned int adc_results[2]; 

void adc_init(void)
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
RCC_ADCCLKConfig(RCC_PCLK2_Div8);
NVIC_InitTypeDef NVIC_InitStructure = {0};

    ADC_InitTypeDef  ADC_InitStructure = {0};
 ////   GPIO_InitTypeDef GPIO_InitStructure = {0};
 ////   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
 ////   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
 /////   GPIO_Init(GPIOC, &GPIO_InitStructure);

////    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 ////   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
 ////   GPIO_Init(GPIOD, &GPIO_InitStructure);

    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;///DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

 ///   ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_241Cycles);
    ADC_InjectedChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_241Cycles);
    ADC_Calibration_Vol(ADC1, ADC_CALVOL_50PERCENT);
    ADC_AutoInjectedConvCmd(ADC1, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
     ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

#if 0
ADC1_DeInit();
ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS,
ADC_CH_VREF,
ADC1_PRESSEL_FCPU_D18, ////ADC1_PRESSEL_FCPU_D18,
ADC1_EXTTRIG_TIM,DISABLE,
ADC1_ALIGN_RIGHT,
ADC_ST_VREF,
ENABLE);

ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE);
ADC1_Cmd(ENABLE);
ADC1_StartConversion(); 
#endif
summ_adc_dat=0;
num_adc_dat=0;

}

volatile uint16_t vtmp;
////==========================================================  
void hnd_adc(void)
{

uint16_t tmp;
tmp = ADC_GetConversionValue(ADC1);
vtmp=tmp;
summ_adc_dat+=tmp;
num_adc_dat++;

}

void obr_adc_dat(void)
{
uint32_t tmp;
float tmp1;
//// return; 
if(num_adc_dat)
  {
  tmp= summ_adc_dat/num_adc_dat;
  if(tmp==0)
      tmp=1;
  tmp1=ADC_IN_VAL*1023.0;
  tmp1/=tmp;
   cur_adc_dat= (uint16_t)tmp1;
   summ_adc_dat=0;
   num_adc_dat=0;
  }
}
///=====================================================
void ADC1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void ADC1_IRQHandler(void)
{
  ////  u16 ADC_val;

    if(ADC_GetITStatus(ADC1, ADC_IT_EOC))
    {
        hnd_adc();
 ////       printf("ADC Extline trigger conversion...\r\n");
 ////       ADC_val = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1);
 ////       printf("JADC-%04d\r\n", ADC_val);
    }

    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
}



