// [路径: Project_Root/Hardware/bsp_led.h]
#ifndef BSP_LED_H
#define BSP_LED_H

#include <stdint.h>

/**
 * @brief  LED 颜色枚举定义 (对应野火指南者 PB0, PB1, PB5)
 */
typedef enum {
    BSP_LED_COLOR_RED = 0,
    BSP_LED_COLOR_GREEN,
    BSP_LED_COLOR_BLUE
}BSP_LED_Color_t;

/**
 * @brief  LED 状态枚举定义
 */
typedef enum {
    BSP_LED_STATE_OFF = 0,
    BSP_LED_STATE_ON
} BSP_LED_State_t;

void BSP_LED_SetState(BSP_LED_Color_t color, BSP_LED_State_t state);
void BSP_LED_Toggle(BSP_LED_Color_t color);

#endif // BSP_LED_H