#include "bsp_led.h"
#include "main.h"   // 驱动层允许且必须包含 main.h，以获取 CubeMX 生成的引脚宏

/**
 * @brief   设置指定颜色的 LED 状态
 * @param   color: 目标 LED 的颜色 (BSP_LED_COLOR_RED/GREEN/BLUE)
 * @param   state: 期望的状态 (BSP_LED_STATE_ON/OFF)
 * @retval  无
 */
void BSP_LED_SetState(BSP_LED_Color_t color, BSP_LED_State_t state)
{
    GPIO_TypeDef* port = NULL;
    uint16_t      pin  = 0;

    // 1. 映射逻辑颜色到物理引脚
    switch (color)
    {
        case BSP_LED_COLOR_RED:
            port = LED_R_GPIO_Port; // 需要在 CubeMX 中将 PB5 命名为 LED_R
            pin  = LED_R_Pin;
            break;
        case BSP_LED_COLOR_GREEN:
            port = LED_G_GPIO_Port; // 需要在 CubeMX 中将 PB0 命名为 LED_G
            pin  = LED_G_Pin;
            break;
        case BSP_LED_COLOR_BLUE:
            port = LED_B_GPIO_Port; // 需要在 CubeMX 中将 PB1 命名为 LED_B
            pin  = LED_B_Pin;
            break;
        default:
            return; // 非法参数直接返回
    }

    // 2. 映射逻辑状态到物理电平 (低电平点亮)
    GPIO_PinState pin_state = (state == BSP_LED_STATE_ON) ? GPIO_PIN_RESET : GPIO_PIN_SET;

    // 3. 执行硬件操作
    HAL_GPIO_WritePin(port, pin, pin_state);
}

/**
 * @brief   翻转指定颜色的 LED 状态
 * @param   color: 目标 LED 的颜色
 * @retval  无
 */
void BSP_LED_Toggle(BSP_LED_Color_t color)
{
    GPIO_TypeDef* port = NULL;
    uint16_t      pin  = 0;

    switch (color)
    {
        case BSP_LED_COLOR_RED:
            port = LED_R_GPIO_Port;
            pin  = LED_R_Pin;
            break;
        case BSP_LED_COLOR_GREEN:
            port = LED_G_GPIO_Port;
            pin  = LED_G_Pin;
            break;
        case BSP_LED_COLOR_BLUE:
            port = LED_B_GPIO_Port;
            pin  = LED_B_Pin;
            break;
        default:
            return;
    }

    HAL_GPIO_TogglePin(port, pin);
}