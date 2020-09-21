#ifndef __BOARD_RASP_H__
#define __BOARD_RASP_H__

#if defined(RASPI3) || defined(RASPI2)
#define PERIPHERAL_BASE 0x3F000000
#elif defined(RASPI4)
#define PERIPHERAL_BASE 0xFE000000
#endif

#define PERIPHERAL_LENGTH 0x01000000

#define SYSTEM_TIMER_OFFSET 0x3000
#define INTERRUPTS_OFFSET 0xB000
#define MAILBOX_OFFSET 0xB880
#define UART0_OFFSET 0x201000
#define GPIO_OFFSET 0x200000

#define GPU_IO_BASE 0x7E000000
#define GPU_UNCACHED_BASE 0xC0000000
#define GPU_MEM_BASE GPU_UNCACHED_BASE
#define BUS_ADDRESS(addr) (((addr) & ~0xC0000000) | GPU_MEM_BASE)

#endif// __BOARD_RASP_H__
