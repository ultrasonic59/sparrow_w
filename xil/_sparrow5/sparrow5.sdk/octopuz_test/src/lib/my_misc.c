#include <stdint.h>
#include "octopuz.h"


#include "board_cnf.h"
#include "my_misc.h"
///#include "ff.h"

///#include "usb_dcd_int.h"

///#include "wiced.h"

void wait_config_fpga(void)
{
////while(GPIO_ReadInputDataBit(FPGA_DONE_GPIO,FPGA_DONE_PIN)==0);
}

void uDelay (const uint32_t usec)
{
uint32_t count = 0;
const uint32_t utime = 45*usec;////(120 * usec / 7);
do
  {
    if ( ++count > utime )
    {
      return ;
    }
  }
  while (1);
}
/*
void put_tst1(int on_off) ///27 pin
{
if(on_off&0x1) 
  TST1_GPIO->BSRRL= TST1_PIN;
else
  TST1_GPIO->BSRRH= TST1_PIN;
}
*/

///======================================   
