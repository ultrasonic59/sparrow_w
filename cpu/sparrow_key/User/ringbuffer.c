#include <stdlib.h>
#include <string.h>

#include "ringbuffer.h"
////extern void *pvPortMalloc( size_t xWantedSize );

uint8_t*  ringbuffer_init(ringbuffer_t *rbuffer,int size)
{
rbuffer->buffer=(uint8_t*)malloc(size);
if(rbuffer->buffer!=0)
  {
  memset(rbuffer->buffer, 0, size);
  rbuffer->put_index = 0;
  rbuffer->get_index = 0;
  rbuffer->size = size;
////  rbuffer->cnt = 0;
  }
return rbuffer->buffer;
}
void  ringbuffer_clr(ringbuffer_t *rbuffer)
{
  memset(rbuffer->buffer, 0, rbuffer->size);
  rbuffer->put_index = 0;
  rbuffer->get_index = 0;
///  rbuffer->cnt = 0;
}

///inline 
int ringbuffer_putc(ringbuffer_t* rbuffer,char ch)
{
///int level;
    /* disable interrupt */
///    level = rt_hw_interrupt_disable();
#if 0
if(rbuffer->cnt>=rbuffer->size)
  {
  return -1;
  }
#endif
rbuffer->buffer[rbuffer->put_index] = ch;
rbuffer->put_index++;
if(rbuffer->put_index>=rbuffer->size)
  rbuffer->put_index=0;
///rbuffer->cnt++;
return 0;///rbuffer->cnt;
}
////inline 
int ringbuffer_getc(ringbuffer_t *rbuffer ,uint8_t *o_ch)
{
uint8_t ch;
uint16_t cnt=1;
///int level;
///if(rbuffer->cnt==0)
///  return -1;
if (rbuffer->get_index == rbuffer->put_index)
  return -1;

////if (rbuffer->get_index != rbuffer->put_index)
    {
    ch = rbuffer->buffer[rbuffer->get_index];
    *o_ch=ch;
    rbuffer->get_index++;
    if(rbuffer->get_index>=rbuffer->size)
        rbuffer->get_index=0;
    if (rbuffer->get_index == rbuffer->put_index)
      cnt=0;
    else
      cnt=1;
    
 ///   rbuffer->cnt--;
 ///   cnt = rbuffer->cnt;
    }
///else
///  {
///  rbuffer->cnt=0;
///  cnt=-1;
///  }
return cnt;
}
////uint32_t ring_buffer_used_space( wiced_ring_buffer_t* ring_buffer )
////inline 
uint16_t get_ringbuffer_cnt(ringbuffer_t *ring_buffer)
{
uint32_t head_to_end = ring_buffer->size - ring_buffer->get_index;
return (uint16_t)((head_to_end + ring_buffer->put_index) % ring_buffer->size);
}


