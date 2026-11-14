#ifndef USB_COM_H
#define USB_COM_H

#include <stdint.h>

/* Second FE1.1 port: USB-COM adapter. 0 opened, 1 not a COM, -1 failed. */
int com_attach(uint8_t addr, uint8_t speed, uint8_t hub, uint8_t port, uint16_t mps0, const uint8_t desc[18],
               const uint8_t* cfg, uint16_t cfg_len);

/* baud 0 keeps the current rate. Bytes are written to the adapter UART unchanged. */
int com_write(uint32_t baud, const uint8_t* data, uint16_t length);

/* Best-effort bulk IN until '\r' or timeout. Returns 0 if any byte was read. */
int com_read(uint8_t* data, uint16_t cap, uint16_t* actual, uint32_t timeout_ms);

#endif
