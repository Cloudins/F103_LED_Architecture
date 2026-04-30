#ifndef __SERVICE_EVENT_H
#define __SERVICE_EVENT_H

#include <stdint.h>

void Service_Key_EXTI_Notify(uint8_t key_id);
void Service_Event_Task(uint32_t sys_tick); // 增加参数
uint8_t Service_Event_ReadMailbox(void);

#endif