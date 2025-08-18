Software-Defined-Oscilloscope

A lightweight, open-source oscilloscope implementation using software-first architecture

Overview

This project implements a software-defined oscilloscope that uses microcontroller(s) and host-side software to acquire analog waveforms—via ADC or capture peripheral—and stream them to a controlling computer for real-time visualization and analysis.

Key Features:

Microcontroller-based data acquisition (ADC, DMA, timers)

Streaming capture via USB or serial port to host

Cross-platform GUI (or CLI) for waveform display and analysis

Modular and extensible design for adding features like triggering, FFT, or multi-channel support

Motivation

Traditional oscilloscopes are often large, expensive, and rely on embedded firmware + display interfaces. The software-defined approach allows you to:

Use a low-cost microcontroller alongside your PC for visualization

Leverage powerful host-side software (Python, Qt, etc.) for features

Rapidly prototype and extend the tool via software

Project Structure
.
├── firmware/           # Microcontroller firmware source
│   ├── main.c
│   ├── adc.c
│   └── dma.c
├── host/               # Host-side visualization (Python/Qt or CLI)
│   ├── scope.py
│   └── gui/
├── docs/               # Documentation and schematics
│   └── wiring.md
├── examples/           # Sample capture / test workflows
│   └── sine_test
└── CMakeLists.txt      # Build configuration

Getting Started
Prerequisites

Microcontroller development board (e.g. STM32, ESP32)

ARM GCC toolchain or platform SDK

USB-Serial driver (e.g., ST-Link or CDC)

On host: Python 3.x + dependencies (e.g., PyQt5, numpy, pyserial)

Flashing Firmware
cd firmware
make clean
make all
make flash

Running the GUI
cd host
pip install -r requirements.txt
python scope.py --port /dev/ttyUSB0 --baud 115200

Usage

Connect probe to ADC input pins.

Launch the host GUI.

Start acquisition—waveform should appear with real-time scrolling.

Use GUI controls to set timebase, volt/div, and trigger levels.

Roadmap & Features
Feature	Status
ADC sampling	✅ Completed
DMA buffering	In progress
Triggering (edge-level)	Planned
FFT display	Future
Multi-channel acquisition	Future
Contribution

Contributions are welcome! Please fork, make changes, and submit a pull request. For bugs and feature requests, please open an issue.

License
This project is released under the MIT License — feel free to use, modify, and share!

