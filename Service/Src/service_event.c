#include "service_event.h"
#include "bsp_key.h" // 识别传入的 key_id
// #include "main.h" // 若使用 HAL_GetTick，业务层可有限度包含，或通过系统抽象层提供

#define DEBOUNCE_TIME_MS 20

// 简单的事件邮箱（单缓冲），实际复杂项目会用环形队列(RingBuffer)或 FreeRTOS 消息队列
static volatile Service_Event_t g_current_event = EVENT_NONE;

/**
 * @brief 按键中断通知回调 (运行在中断上下文中)
 */
void Service_Key_EXTI_Notify(uint8_t hardware_key_id)
{
    // 静态数组记录每个按键的上一次触发时间
    static uint32_t last_trigger_tick[3] = {0, 0, 0};
    extern uint32_t HAL_GetTick(void); // 获取系统时间戳

    uint32_t current_tick = HAL_GetTick();

    // 1. 时间戳消抖逻辑
    if (hardware_key_id == BSP_KEY_WKUP) {
        if ((current_tick - last_trigger_tick[0]) > DEBOUNCE_TIME_MS) {
            g_current_event = EVENT_KEY_WKUP_PRESSED; // 确认按下，抛出事件
            last_trigger_tick[0] = current_tick;
        }
    }
    else if (hardware_key_id == BSP_KEY_2) {
        if ((current_tick - last_trigger_tick[1]) > DEBOUNCE_TIME_MS) {
            g_current_event = EVENT_KEY_2_PRESSED;
            last_trigger_tick[1] = current_tick;
        }
    }
    else if (hardware_key_id == BSP_KEY_EXTERNAL) {
        if ((current_tick - last_trigger_tick[2]) > DEBOUNCE_TIME_MS) {
            g_current_event = EVENT_KEY_EXTERNAL_PRESSED;
            last_trigger_tick[2] = current_tick;
        }
    }
}

/**
 * @brief 读取当前待处理的事件 (运行在主循环中)
 */
Service_Event_t Service_Event_Read(void)
{
    Service_Event_t event_to_return = g_current_event;
    g_current_event = EVENT_NONE; // 读后清空（消费事件）
    return event_to_return;
}