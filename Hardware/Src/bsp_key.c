#include "bsp_key.h"
#include "main.h"
#include "service_event.h" // 驱动层上报事件给业务层

/**
 * @brief 重写 HAL 库的外部中断弱回调函数
 * @note  这个函数会在 EXTI_IRQHandler 中被自动调用，处于中断上下文中！
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    BSP_Key_ID_t key_id = BSP_KEY_UNKNOWN;

    // 1. 物理引脚 -> 逻辑按键的映射
    if (GPIO_Pin == KEY_WKUP_Pin) {
        key_id = BSP_KEY_WKUP;
    }
    else if (GPIO_Pin == KEY_2_Pin) {
        key_id = BSP_KEY_2;
    }
    else if (GPIO_Pin == KEY_EXTERNAL_Pin) {
        key_id = BSP_KEY_EXTERNAL;
    }

    // 2. 将事件抛给业务层处理 (必须确保业务层函数极其简短，不能阻塞)
    if (key_id != BSP_KEY_UNKNOWN) {
        Service_Key_EXTI_Notify((uint8_t)key_id);
    }
}

/**
 * @brief  获取逻辑按键的当前物理状态
 * @param  key_id: 按键枚举 ID
 * @retval 1: 当前处于按下状态 | 0: 当前处于松开状态
 */
uint8_t BSP_Key_IsPressed(BSP_Key_ID_t key_id)
{
    switch (key_id)
    {
        case BSP_KEY_WKUP:
            // 野火指南者的 WKUP 是高电平有效
            return (HAL_GPIO_ReadPin(GPIOA, KEY_WKUP_Pin) == GPIO_PIN_SET) ? 1 : 0;

        case BSP_KEY_2:
            // 假设 KEY2 是外部上拉，低电平有效
            return (HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin) == GPIO_PIN_RESET) ? 1 : 0;

        case BSP_KEY_EXTERNAL:
            // 假设外部按键也是低电平有效
            return (HAL_GPIO_ReadPin(KEY_EXTERNAL_GPIO_Port, KEY_EXTERNAL_Pin) == GPIO_PIN_RESET) ? 1 : 0;

        default:
            return 0;
    }
}
