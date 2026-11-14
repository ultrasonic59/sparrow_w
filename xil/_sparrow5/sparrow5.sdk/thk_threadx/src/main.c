#include "tx_api.h"
#include "xil_printf.h"

#define THK_STACK_BYTES 2048

static TX_THREAD g_print;
static TX_THREAD g_count;
static ULONG g_print_stack[THK_STACK_BYTES / sizeof(ULONG)];
static ULONG g_count_stack[THK_STACK_BYTES / sizeof(ULONG)];
static ULONG g_seconds;

static void print_entry(ULONG input)
{
    (void)input;
    for (;;) {
        xil_printf("ThreadX %lu s\r\n", g_seconds);
        tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND);
    }
}

static void count_entry(ULONG input)
{
    (void)input;
    for (;;) {
        g_seconds++;
        tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND);
    }
}

void tx_application_define(void *first_unused_memory)
{
    (void)first_unused_memory;
    tx_thread_create(&g_print, "print", print_entry, 0,
                     g_print_stack, sizeof g_print_stack,
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
    tx_thread_create(&g_count, "count", count_entry, 0,
                     g_count_stack, sizeof g_count_stack,
                     2, 2, TX_NO_TIME_SLICE, TX_AUTO_START);
}

int main(void)
{
    xil_printf("Hello from ThreadX\r\n");
    tx_kernel_enter();
    return 0;
}
///======================================================================
