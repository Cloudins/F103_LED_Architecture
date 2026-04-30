# 🌟 STM32 企业级四层架构开发框架 

![STM32](https://img.shields.io/badge/MCU-STM32F103VET6-blue?logo=stmicroelectronics)
![C](https://img.shields.io/badge/Language-C-00599C?logo=c)
![GCC](https://img.shields.io/badge/Compiler-GCC-orange?logo=gnu)
![CMake](https://img.shields.io/badge/Build-CMake-064F8C?logo=cmake)
![CLion](https://img.shields.io/badge/IDE-CLion-black?logo=clion)
![Architecture](https://img.shields.io/badge/Architecture-4--Layer-success)

本项目是一个为 **STM32F103VET6** 微控制器量身定制的高内聚、低耦合嵌入式系统框架。本项目彻底摒弃了传统 `main.c` 中的“面条式代码”，采用严格的四层架构设计，为后续接入复杂的能量管理电路、多传感器融合以及 RTOS 打下坚实的底层基础。

## 🏗️ 架构总览 (Architecture Overview)

系统严格遵循软件分层规范，将业务逻辑与硬件依赖彻底解耦：

* **应用层 (Application / `Application/`)**：包含 `app_main_task`。仅负责顶层任务的轮询调度与事件分发，极致简洁。
* **业务层 (Service / `Service/`)**：包含纯业务逻辑（如 LED 动效控制、按键事件邮箱、CLI 命令解析）。**该层绝对不包含任何底层 `stm32f1xx_hal.h` 依赖。**
* **驱动层 (Hardware / `Hardware/`)**：自定义的板级支持包 (BSP) 模块（如 `bsp_led`、`bsp_key`、`bsp_uart`）。负责硬件引脚映射、劫持 HAL 库弱回调函数（中断管理），向下对接 HAL 库，向上提供纯 C 接口。
* **硬件层 (HAL/BSP / `Core/`)**：由 STM32CubeMX 自动生成的初始化代码与标准 HAL 库驱动。

## 🚀 核心特性与里程碑 (Key Features)

* **⏱️ 零中断微秒级精确延时**：引入 ARM Cortex-M 内核的 **DWT (周期计数器)**，实现了绝对精确的微秒级延时。免疫一切中断嵌套死锁问题，完美适配未来 1-Wire 等严格时序协议的底层驱动。
* **📊 纳秒级系统性能剖析器 (Profiler)**：结合 DWT 与非阻塞 CLI 终端，实现了侵入式极低的代码执行耗时测量（例如精确测量 `printf` 函数占用的 CPU 周期与微秒数）。
* **📬 异步事件邮箱机制 (Event Mailbox)**：基于 EXTI 外部中断与 20ms 时间戳，实现了高效无阻塞的机械按键消抖，并将底层硬件触发统一抽象为事件队列供应用层读取。
* **💻 非阻塞命令行交互 (CLI)**：基于 UART RXNE 单字节接收中断与 **环形缓冲区 (Ring Buffer)** 构建。配合 GCC 下的 `_write` 重定向，实现了流畅的系统调试与指令下发。

## 🛠️ 硬件与工具链 (Hardware & Toolchain)

* **目标开发板**：野火指南者 (STM32F103VET6)
* **编译工具链**：`arm-none-eabi-gcc`
* **构建系统**：CMake (已配置 `GLOB_RECURSE` 自动递归包含源文件，以及 `-u _printf_float` 支持浮点打印)
* **推荐开发环境**：CLion + OpenOCD

## 📂 目录结构 (Directory Structure)

```text
/Project_Root
├── Application/        # [应用层] 主任务与状态机调度
├── Service/            # [业务层] 数据处理、协议解析与事件中转
├── Hardware/           # [驱动层] 外设抽象与中断处理 (BSP)
├── Core/               # [硬件层] STM32CubeMX 自动生成文件
├── CMakeLists.txt      # 核心构建脚本
└── .gitignore          # 专为 CLion + CMake 定制的版本控制忽略规则
