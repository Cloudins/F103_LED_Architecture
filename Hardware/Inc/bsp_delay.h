#ifndef BSP_DELAY_H
#define BSP_DELAY_H

#include <stdint.h>

/* DWT 初始化，必须在系统启动时调用一次 */
void BSP_Delay_Init(void);

/* 微秒级非阻塞/阻塞延时 (依托 DWT) */
void BSP_Delay_us(uint32_t us);

/* 毫秒级延时 (替代 HAL_Delay) */
void BSP_Delay_ms(uint32_t ms);

#endif /* BSP_DELAY_H */