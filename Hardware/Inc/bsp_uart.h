#ifndef __BSP_UART_H
#define __BSP_UART_H

#include <stdint.h>
#include <stdbool.h>

#define UART_RX_BUF_SIZE 128

// 环形缓冲区结构体定义
typedef struct {
    uint8_t  buffer[UART_RX_BUF_SIZE];
    volatile uint16_t head; // 读指针
    volatile uint16_t tail; // 写指针
} RingBuffer_t;

// 驱动层初始化（开启中断）
void BSP_UART_Init(void);

// 供业务层读取缓冲区的接口
bool BSP_UART_ReadByte(uint8_t *data);

#endif /* __BSP_UART_H */