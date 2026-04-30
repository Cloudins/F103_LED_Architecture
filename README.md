# STM32 Enterprise-Grade Four-Layer Architecture Framework

This repository contains a highly modular, low-coupling embedded system framework developed for the STM32F103VET6 microcontroller. It abandons traditional "spaghetti code" in `main.c` in favor of a strict four-layer architecture, laying a solid foundation for complex energy management circuits, multi-sensor fusion, and future RTOS integration.

## Architecture Overview

The system strictly adheres to a software layering standard to separate business logic from hardware dependencies:

* **Application Layer (`Application/`):** Contains `app_main_task`. Exclusively handles task polling, scheduling, and event distribution.
* **Service Layer (`Service/`):** Contains hardware-agnostic business logic (e.g., LED effects, event mailbox, CLI parsing). This layer maintains zero dependency on `stm32f1xx_hal.h`.
* **Hardware Driver Layer (`Hardware/`):** Contains custom Board Support Package (BSP) modules (e.g., `bsp_led`, `bsp_key`, `bsp_uart`). It handles hardware pin mapping, interrupt hijacking, and provides pure C interfaces upward.
* **HAL/BSP Layer (`Core/`):** Contains STM32CubeMX auto-generated initialization code and standard HAL library drivers.

## Key Features & Milestones

* **Zero-Interrupt Precision Timing:** Implemented ARM Cortex-M DWT (Data Watchpoint and Trigger) for exact microsecond-level delays, immune to interrupt nesting and ideal for precise protocol simulation (e.g., 1-Wire).
* **System Performance Profiler:** Integrated DWT-based profiling tools into the CLI to measure code execution time (e.g., `printf` overhead) down to the exact CPU cycle.
* **Asynchronous Event Mailbox:** Features a non-blocking, timestamp-based debouncing mechanism (20ms) for hardware inputs, abstracting EXTI triggers into a unified event queue.
* **Non-Blocking CLI System:** Built a custom Command Line Interface utilizing UART RXNE single-byte interrupts, ring buffer management, and GCC `_write` printf redirection.

## Hardware & Toolchain

* **Target Board:** 野火指南者 (STM32F103VET6)
* **Toolchain:** arm-none-eabi-gcc
* **Build System:** CMake (configured with `GLOB_RECURSE` and `-u _printf_float`)
* **Recommended IDE:** CLion

## Directory Structure

```text
/Project_Root
├── Application/        # [Layer 1] Main tasks and state machines
├── Service/            # [Layer 2] Data processing and protocols
├── Hardware/           # [Layer 3] Peripheral drivers and abstraction
├── Core/               # [Layer 4] STM32CubeMX generated files
├── CMakeLists.txt      # Build configuration
└── .gitignore          # Version control rules
