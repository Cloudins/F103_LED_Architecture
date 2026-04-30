#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include <stdint.h>

// 定义逻辑按键枚举
typedef enum {
    BSP_KEY_WKUP = 0,
    BSP_KEY_2,
    BSP_KEY_EXTERNAL,
    BSP_KEY_UNKNOWN
} BSP_Key_ID_t;

// 驱动层不处理消抖，只负责硬件初始化(若需)和事件抛出
// 这里不需要向外暴露函数，依靠中断回调自动触发 Service 层

#endif /* __BSP_KEY_H */