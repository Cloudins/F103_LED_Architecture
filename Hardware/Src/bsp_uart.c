#include "bsp_uart.h"
#include "usart.h" // 包含 CubeMX 生成的 huart1 句柄
#include <stdio.h>

// 实例化一个静态的环形缓冲区，对上层隐藏细节
static RingBuffer_t rx_fifo = {.head = 0, .tail = 0};

// 用于 HAL 库单字节接收中断的临时存放变量
static uint8_t rx_temp_byte;

// =========================================================
// 1. GCC (CLion) 环境下的 printf 重定向核心代码
// =========================================================
#ifdef __GNUC__
int _write(int file, char *ptr, int len) {
    // 调用 HAL 库的阻塞发送函数将 printf 的内容通过 USART1 发出
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}
#endif

// =========================================================
// 2. UART 初始化与中断开启
// =========================================================
void BSP_UART_Init(void) {
    // 开启单字节接收中断。收到 1 个字节后，硬件会自动调用 HAL_UART_RxCpltCallback
    // 开启中断，存放在 rx_temp_byte 临时存放变量（8位）
    // 指定接收缓冲和长度，完成指定数量后才回调。
    HAL_UART_Receive_IT(&huart1, &rx_temp_byte, 1);
}

// =========================================================
// 3. 串口接收中断回调函数 (快递员塞信)
// =========================================================
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        // 计算下一个写指针的位置 (取余实现环形)
        uint16_t next_tail = (rx_fifo.tail + 1) % UART_RX_BUF_SIZE;
        
        // 防止溢出覆盖：如果写指针追上了读指针，说明满了，丢弃新数据
        if (next_tail != rx_fifo.head) {
            rx_fifo.buffer[rx_fifo.tail] = rx_temp_byte;
            rx_fifo.tail = next_tail;
        }

        // 重要：重新开启接收中断！（HAL库的设计机制，中断一次就关了，必须手动再开）
        HAL_UART_Receive_IT(&huart1, &rx_temp_byte, 1);
    }
}

// =========================================================
// 4. 供上层读取数据的接口 (老板取信)
// =========================================================
bool BSP_UART_ReadByte(uint8_t *data) {
    if (rx_fifo.head == rx_fifo.tail) {
        return false; // 队列为空
    }
    *data = rx_fifo.buffer[rx_fifo.head];
    rx_fifo.head = (rx_fifo.head + 1) % UART_RX_BUF_SIZE;
    return true; // 成功读取一个字节
}