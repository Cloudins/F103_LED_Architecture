#include "bsp_delay.h"
#include "stm32f1xx.h" /* 仅在此处引入内核头文件，获取系统时钟频率 */

void BSP_Delay_Init(void)
{
    /* 1. 使能 DWT 外设的跟踪功能 (Trace Enable) */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    
    /* 2. 清零 DWT 周期计数器 */
    DWT->CYCCNT = 0;
    
    /* 3. 启动 DWT 周期计数器 */
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void BSP_Delay_us(uint32_t us)
{
    /* 计算所需经过的 CPU 周期数 */
    /* SystemCoreClock 通常为 72000000 (72MHz) */
    // 72 个 ticks 是 1 us
    uint32_t ticks = us * (SystemCoreClock / 1000000);
    uint32_t start_tick = DWT->CYCCNT;
    
    /* 利用无符号整型的回绕特性，安全处理计数器溢出 */
    while ((DWT->CYCCNT - start_tick) < ticks)
    {
        /* 紧凑型空循环，等待周期到达 */
    }
}

void BSP_Delay_ms(uint32_t ms)
{
    while (ms--)
    {
        BSP_Delay_us(1000);
    }
}