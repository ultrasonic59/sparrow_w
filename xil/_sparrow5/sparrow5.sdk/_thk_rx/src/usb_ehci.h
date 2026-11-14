#ifndef USB_EHCI_H
#define USB_EHCI_H

#include <stdint.h>

/* Zynq USB0 host. Behind the FE1.1 hub: RTL8152 (high-speed) and USB-COM.
   speed: 2 high-speed, 0 full-speed, 1 low-speed.
   For full/low, hub is the hub address and port is the 1-based port number. */

int ehci_init(void);
void ehci_wait_ms(unsigned ms);

/* setup is 8 bytes. data may be NULL when length is 0.
   Data direction comes from setup[0] bit 7. */
int ehci_control(uint8_t addr, const uint8_t setup[8], void* data, uint16_t length);
int ehci_control_tt(uint8_t addr, uint8_t speed, uint8_t hub, uint8_t port, uint16_t mps,
                    const uint8_t setup[8], void* data, uint16_t length);

int ehci_bulk_out(uint8_t addr, uint8_t ep, uint16_t mps, const void* data, uint16_t length, int* toggle);
int ehci_bulk_out_tt(uint8_t addr, uint8_t speed, uint8_t hub, uint8_t port, uint8_t ep, uint16_t mps,
                     const void* data, uint16_t length, int* toggle);
int ehci_bulk_in(uint8_t addr, uint8_t ep, uint16_t mps, void* data, uint16_t length, uint16_t* actual,
                 int* toggle, uint32_t timeout_ms);

#endif
