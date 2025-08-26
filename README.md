# Software-Defined Oscilloscope

A software-defined oscilloscope built on **STM32F103C8T6** with a **Qt-based PC interface**.  
The project captures analog signals via STM32 peripherals, transfers sampled data over UART, and visualizes waveforms on a computer.

---

## ✨ Features
- Signal acquisition using **STM32 ADC**  
- UART data transfer to PC  
- **Qt application** for real-time waveform visualization  
- Modular source code for easy extension  

---

## 📂 Project Structure
```text
Software-Defined-Oscilloscope
│
├── .settings/                # IDE-specific configuration files
├── Debug/                    # Build output (binaries, object files, etc.)
├── Libraries/                # External libraries and drivers
├── Qt/                       # Qt-based PC interface source code
├── src/                      # Core STM32 firmware
│   ├── capture.c
│   ├── capture.h
│   ├── frame.c
│   ├── frame.h
│   ├── main.c
│   ├── main.h
│   ├── parameters.h
│   ├── peripheral.c
│   ├── peripheral.h
│   ├── stm32f10x_conf.h
│   ├── stm32f10x_it.c
│   ├── stm32f10x_it.h
│   ├── system_stm32f10x.c
│   ├── startup_stm32f10x.ld
│   └── tiny_printf.c
│
└── README.md
```

## ⚙️ Getting Started
Prerequisites
1. STM32F103C8T6 board
2. ST-Link or equivalent debugger
3. Qt 5.x or 6.x for PC GUI
4. arm-none-eabi-gcc toolchain (or STM32CubeIDE)

Build & Flash Firmware
- cd src
- make flash

Run Qt Application
- cd Qt
- qmake && make
- ./oscilloscope

🖥️ Usage
- Flash STM32 firmware.
- Connect board via UART/USB.
- Run the Qt application.
- Select the correct COM port.
- View real-time waveforms.

🤝 Contributing
 - Pull requests and suggestions are welcome!

👤 Author
 - Developed by Svansh Gaba, Anant Raj
