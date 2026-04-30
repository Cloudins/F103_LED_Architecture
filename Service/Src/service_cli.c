#include "service_cli.h"
#include "bsp_uart.h"
#include "bsp_led.h"
#include <stdio.h>
#include <string.h>

#include "bsp_delay.h" // 引入 DWT
#include "stm32f1xx.h" // 获取 SystemCoreClock

#define CMD_MAX_LEN 32
static char cmd_buffer[CMD_MAX_LEN];
static uint8_t cmd_index = 0;

void Service_CLI_Task(void) {
    uint8_t rx_char;
    
    // 把缓冲区里积压的字符全部读完
    while (BSP_UART_ReadByte(&rx_char)) {
        
        // 假设电脑端发送命令以换行符 '\n' 结尾
        if (rx_char == '\n' || rx_char == '\r') {
            if (cmd_index > 0) {
                cmd_buffer[cmd_index] = '\0'; // 加上字符串结束符
                
                // --- 指令解析与分发 ---
                printf("[SYS] 收到指令: %s\r\n", cmd_buffer);
                
                if (strcmp(cmd_buffer, "LED_R_ON") == 0) {
                    BSP_LED_SetState(BSP_LED_COLOR_RED, BSP_LED_STATE_ON);
                    printf("[SYS] 红灯已点亮。\r\n");
                } 
                else if (strcmp(cmd_buffer, "LED_R_OFF") == 0) {
                    BSP_LED_SetState(BSP_LED_COLOR_RED, BSP_LED_STATE_OFF);
                    printf("[SYS] 红灯已熄灭。\r\n");
                }
                else {
                    printf("[SYS] 未知指令！\r\n");
                }
                
                cmd_index = 0; // 清空指令缓冲区，准备接收下一条
            }
        } 
        else {
            // 存入指令缓冲区，防止越界
            if (cmd_index < CMD_MAX_LEN - 1) {
                cmd_buffer[cmd_index++] = rx_char;
            }
        }
    }
}

void Service_CLI_Execute(const char* cmd)
{
    if (strcmp(cmd, "profile") == 0)
    {
        /* 1. 记录开始时的 CPU 周期数 */
        uint32_t start_cycle = DWT->CYCCNT;

        /* 2. 执行待测量的代码段 */
        printf("[Profiler] Testing printf execution time...\r\n");

        /* 3. 记录结束时的 CPU 周期数 */
        uint32_t end_cycle = DWT->CYCCNT;

        /* 4. 计算耗时 (注意处理无符号回绕) */
        uint32_t cycles_used = end_cycle - start_cycle;
        uint32_t time_us = cycles_used / (SystemCoreClock / 1000000);

        printf("-> Executed in: %lu CPU cycles (Approx %lu us)\r\n", cycles_used, time_us);
    }
    else
    {
        printf("Unknown Command: %s\r\n", cmd);
    }
}