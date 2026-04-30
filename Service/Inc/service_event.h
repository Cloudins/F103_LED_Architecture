#ifndef __SERVICE_EVENT_H
#define __SERVICE_EVENT_H

#include <stdint.h>
#include <stdbool.h>

// 业务层定义的按键事件枚举
typedef enum {
    EVENT_NONE = 0,
    EVENT_KEY_WKUP_PRESSED,
    EVENT_KEY_2_PRESSED,
    EVENT_KEY_EXTERNAL_PRESSED
} Service_Event_t;

// 供驱动层(中断)调用的通知接口
void Service_Key_EXTI_Notify(uint8_t hardware_key_id);

// 供应用层轮询获取事件的接口
Service_Event_t Service_Event_Read(void);

#endif /* __SERVICE_EVENT_H */