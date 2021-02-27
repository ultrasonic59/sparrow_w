#include <stdlib.h>
#include <string.h>

#include "ringbuffer.h"

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
inline uint16_t get_ringbuffer_cnt(ringbuffer_t *ring_buffer)
{
uint32_t head_to_end = ring_buffer->size - ring_buffer->get_index;
return (uint16_t)((head_to_end + ring_buffer->put_index) % ring_buffer->size);
}
///======================================================
void *cb_init(circ_buffer_t *cb, size_t capacity, size_t sz)
{
cb->buffer = (void*)malloc(capacity * sz);
if(cb->buffer == 0)
	return 0;
cb->capacity = capacity;
cb->sz = sz;
cb->put_index = 0;
cb->get_index = 0;
return cb->buffer;
}
void cb_clear(circ_buffer_t *cb)
{
if(cb->buffer == 0)
	return ;
///cb->buffer_end = (char *)cb->buffer + capacity * sz;
cb->get_index = 0;
cb->put_index = 0;
}
void cb_clear_all(circ_buffer_t *cb)
{
if(cb->buffer == 0)
	return ;
memset(cb->buffer,0,cb->capacity * cb->sz);
cb->get_index = 0;
cb->put_index = 0;
}
int cb_get_count(circ_buffer_t *cb)
{
uint32_t head_to_end = cb->capacity - cb->get_index;
return (uint16_t)((head_to_end + cb->put_index) % cb->capacity);
}

int cb_push(circ_buffer_t *cb, const void *item)
{
///if(cb->count == cb->capacity)
////	return 0;
memcpy((uint8_t*)(cb->buffer+cb->put_index), item, cb->sz);
cb->put_index++;;
if(cb->put_index>=cb->capacity)
  cb->put_index=0;
return cb_get_count(cb);
}
int cb_pop(circ_buffer_t *cb, void *item)
{
if (cb->get_index == cb->put_index)
	return -1;
memcpy(item, (uint8_t*)(cb->buffer+cb->get_index), cb->sz);
cb->get_index++;
if(cb->get_index>=cb->capacity)
    cb->get_index=0;
return cb_get_count(cb);
}

