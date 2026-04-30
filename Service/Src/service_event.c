#include "service_event.h"
#include "bsp_key.h" // 识别传入的 key_id

#define DEBOUNCE_TIME_MS 20

// 记录哪个按键触发了硬件中断
static volatile uint8_t hw_exti_key_id = BSP_KEY_UNKNOWN;
// 最终对外的事件邮箱
static uint8_t s_EventMailbox = BSP_KEY_UNKNOWN;


// ==========================================
// 1. 供驱动层 bsp_key.c 在 EXTI 中断中调用的 API
// ==========================================
void Service_Key_EXTI_Notify(uint8_t key_id) {
    // 仅仅记录中断来源，绝不能在这里写 HAL_Delay！
    if (hw_exti_key_id == BSP_KEY_UNKNOWN) {
        hw_exti_key_id = key_id;
    }
}
// ==========================================
// 2. 业务层的消抖状态机 (需在应用层大循环中轮询)
// ==========================================
void Service_Event_Task(uint32_t sys_tick) {
    static uint8_t state = 0; // 0:空闲, 1:等待消抖
    static uint32_t press_tick = 0;
    static uint8_t pending_key = BSP_KEY_UNKNOWN;

    switch (state) {
        case 0: // 空闲态，捕捉中断标志
            if (hw_exti_key_id != BSP_KEY_UNKNOWN) {
                pending_key = hw_exti_key_id;
                hw_exti_key_id = BSP_KEY_UNKNOWN; // 清除中断标志
                press_tick = sys_tick;       // 记录时间戳
                state = 1;
            }
            break;

        case 1: // 20ms 时间戳消抖态
            if ((sys_tick - press_tick) >= 20) {

                // 完全依赖下层提供的逻辑状态，业务层不再关心是高电平还是低电平
                if (BSP_Key_IsPressed(pending_key)) {
                    s_EventMailbox = pending_key; // 动态投递触发中断的那个按键
                }
                state = 0;
            }
            break;
    }
}


// ==========================================
// 3. 供应用层读取事件邮箱的 API
// ==========================================
uint8_t Service_Event_ReadMailbox(void) {
    uint8_t event = s_EventMailbox;
    s_EventMailbox = BSP_KEY_UNKNOWN; // 读后即清
    return event;
}