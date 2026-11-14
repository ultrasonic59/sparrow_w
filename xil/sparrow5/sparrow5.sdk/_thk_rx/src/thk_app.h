#ifndef THK_APP_H
#define THK_APP_H

#include "thk_proto.h"

#include <stdint.h>

/* THK frame is already bounded by the UDP packet. The reply is written to out. Length 0 means stay silent. */
uint16_t thk_handle(const uint8_t* frame, uint16_t frame_len, uint8_t* out, uint16_t out_cap);

#endif
