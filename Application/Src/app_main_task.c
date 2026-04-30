#include "app_main_task.h"
#include "main.h"                 // 需要获取 HAL_GetTick
#include "service_led_effect.h"   // 引入 LED 业务服务
#include "service_event.h"  // 引入事件服务
#include "bsp_led.h"        // App层负责调度驱动，控制哪个灯
#include "bsp_uart.h"        // 引入串口初始化
#include "service_cli.h"     // 引入命令行服务
#include "bsp_delay.h"
#include "bsp_key.h"
#include <stdio.h>

/**
 * @brief   应用层主任务初始化入口
 * @note    在 main 函数的 while(1) 之前调用
 */
void App_Main_Init(void)
{
    /*======================= DWT实验 ============================*/
    BSP_Delay_Init(); // 初始化 DWT 内核计数器
    BSP_UART_Init();  // 开启串口接收中断

    printf("\r\n=== System Boot Success ===\r\n");
    printf("Architecture: 4-Layer | Toolchain: GCC\r\n");
    printf("Type 'profile' to test DWT...\r\n");

    // BSP_UART_Init(); // 启动串口接收中断
    // printf("\r\n================================\r\n");
    // printf("系统启动成功！编译时间: %s\r\n", __TIME__);
    // printf("架构级嵌入式系统运行中...\r\n");
    // printf("支持指令: LED_R_ON, LED_R_OFF\r\n");
    // printf("================================\r\n");

    // 确保上电时所有 LED 处于关闭状态
    // (在 Hardware 驱动中我们定义过 BSP_LED_SetState，需在 app 层引用 bsp_led.h，
    //  或者在驱动层 bsp_led.c 写一个 BSP_LED_Init()。这里为了保持层级纯粹，假设 CubeMX 已设为默认熄灭)
}

/**
 * @brief   应用层主任务循环入口
 * @note    在 main 函数的 while(1) 中高频调用
 */
void App_Main_Routine(void)
{
    // 获取当前系统的毫秒级时间戳
    // uint32_t current_tick = HAL_GetTick();

    // --- 任务调度区 ---

    // 1. 运行 LED 心跳灯服务 (非阻塞)
    // Service_LED_Heartbeat_Task(current_tick);


    // --- 事件驱动分发区 ---
    // Service_Event_t current_evt = Service_Event_Read();
    //
    // switch (current_evt) {
    //     case EVENT_KEY_WKUP_PRESSED:
    //         // WK_UP 键按下，翻转红灯
    //         BSP_LED_Toggle(BSP_LED_COLOR_RED);
    //         break;
    //
    //     case EVENT_KEY_2_PRESSED:
    //         // KEY_2 键按下，翻转蓝灯
    //         BSP_LED_Toggle(BSP_LED_COLOR_BLUE);
    //         break;
    //
    //     case EVENT_KEY_EXTERNAL_PRESSED:
    //         // 外部按键 按下，翻转绿灯
    //         // BSP_LED_SetState(BSP_LED_COLOR_GREEN,BSP_LED_STATE_ON);
    //         BSP_LED_Toggle(BSP_LED_COLOR_RED);
    //         break;
    //
    //     case EVENT_NONE:
    //     default:
    //         break; // 无事件，直接退出
    // }
    // 2. 未来在这里继续挂载其他服务：
    // uint8_t key_event = Service_Key_Scan_Task(current_tick);
    // Service_Event_Handler(key_event);


    // 运行命令行解析任务 (完全非阻塞，没数据瞬间退出)
    // Service_CLI_Task();

    /*======================= DWT实验 ============================*/
    while(1)
    {
        uint32_t current_tick = HAL_GetTick(); // 应用层统一获取时间
        // 1. 驱动业务层轮询运转 (非阻塞)
        Service_CLI_Task();     // 处理串口数据包与指令
        Service_Event_Task(current_tick);   // 处理按键消抖状态机

        // 2. 消费事件邮箱
        uint8_t event = Service_Event_ReadMailbox();

        // 当识别到 WKUP 按键被确实按下时
        if (event == BSP_KEY_EXTERNAL)
        {
            printf("[App] WKUP Key valid! Simulating 480us pulse...\r\n");

            // 使用蓝灯模拟信号线电平
            BSP_LED_SetState(BSP_LED_COLOR_BLUE, BSP_LED_STATE_ON);
            BSP_Delay_us(480); // 绝对精确的微秒死等
            BSP_LED_SetState(BSP_LED_COLOR_BLUE, BSP_LED_STATE_OFF);

            printf("[App] 480us Pulse generated.\r\n");
        }
    }
    /*======================= DWT实验 END ============================*/
}