#include "service_led_effect.h"
#include "bsp_led.h"  // 业务层调用驱动层的接口
#include "bsp_delay.h"

// 定义心跳闪烁的时间间隔（毫秒）
#define HEARTBEAT_INTERVAL_MS  500

/**
 * @brief   LED 心跳灯业务任务
 * @param   sys_tick: 当前系统的毫秒级时间戳 (通常由外部 HAL_GetTick() 提供传入)
 * @note    设计为非阻塞式，需在主循环或调度器中高频调用
 */
void Service_LED_Heartbeat_Task(uint32_t sys_tick)
{
    // 使用 static 变量保存上一次执行任务的时间戳
    static uint32_t last_execution_tick = 0;

    // 计算时间差，处理溢出情况（uint32_t 溢出也适用此减法逻辑）
    if ((sys_tick - last_execution_tick) >= HEARTBEAT_INTERVAL_MS)
    {
        // 翻转绿色 LED，表示系统正在健康运行
        BSP_LED_Toggle(BSP_LED_COLOR_GREEN);

        // 更新记录的时间戳
        last_execution_tick = sys_tick;
    }
}