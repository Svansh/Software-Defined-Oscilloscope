Software-Defined Oscilloscope

A lightweight oscilloscope implementation built on STM32F103C8T6 ("Blue Pill") microcontroller with Qt-based PC interface. This project demonstrates real-time signal capture using STM32 peripherals (ADC + DMA + USART) and visualization on a host PC.

✨ Features

Real-time signal capture using STM32 ADC

DMA for efficient data transfer to USART

PC interface built with Qt for visualization

Configurable parameters (sampling rate, buffer size, etc.)

Lightweight tiny_printf implementation for debugging

📂 Project Structure
Software-Defined-Oscilloscope/
├── .settings/                # IDE-specific configuration files
├── Debug/                    # Build output (binaries, object files, etc.)
├── Libraries/                # External libraries and drivers
├── Qt/                       # Qt-based PC interface source code
├── src/                      # Core STM32 firmware
│   ├── capture.c / capture.h       # Signal capture (ADC + DMA handling)
│   ├── frame.c / frame.h           # Frame formatting for communication
│   ├── main.c / main.h             # Application entry point
│   ├── parameters.h                # Global parameter definitions
│   ├── peripheral.c / peripheral.h # Peripheral initialization (GPIO, USART, etc.)
│   ├── stm32f10x_conf.h            # STM32 configuration header
│   ├── stm32f10x_it.c / it.h       # Interrupt service routines
│   ├── system_stm32f10x.c          # System clock configuration
│   ├── startup_stm32f10x.ld        # Linker script
│   ├── tiny_printf.c               # Minimal printf implementation
│   └── ...
└── README.md

🛠️ Getting Started
Requirements

STM32F103C8T6 (Blue Pill)

ST-Link / USB-TTL programmer

STM32CubeIDE or arm-none-eabi-gcc toolchain

Qt 5.x/6.x for PC visualization

Build & Flash (STM32CubeIDE)

Import the project into STM32CubeIDE

Build the firmware

Flash to the STM32 board via ST-Link

Run (Qt PC Interface)

Open Qt/ project in Qt Creator

Build and run the application

Connect the STM32 via USB/Serial

📊 Usage

Connect your signal to the ADC pin (PA0).

Launch the Qt GUI to visualize incoming waveform.

Adjust parameters (sampling rate, buffer size) in parameters.h.
