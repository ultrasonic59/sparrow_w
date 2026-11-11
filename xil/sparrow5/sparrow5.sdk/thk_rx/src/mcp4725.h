#ifndef MCP4725_H
#define MCP4725_H

#include <stdint.h>

/* I2C1, MIO52/MIO53. Amplitude address 0x60, zero-offset address 0x61.
   Code 0..4095. 0 means success. */
int mcp4725_init(void);
int mcp4725_write(uint16_t amplitude, uint16_t offset);

#endif
