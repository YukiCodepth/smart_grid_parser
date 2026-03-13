# ⚡ Smart Grid Telemetry Parser Engine

A high-performance, memory-safe middleware protocol parser designed for embedded gateways in power distribution networks. 

This engine is built to ingest raw, high-speed telemetry payloads from remote smart meters, validate data integrity against electromagnetic interference (EMI), and route critical hardware fault events (like over-voltage spikes or blackouts) to dedicated dispatch handlers in real-time.

## 🧠 Core Architecture & Technical Features

This project serves as a demonstration of industrial-grade, bare-metal C programming principles:

* **Zero-Copy Payload Unpacking (Unions & Bit-Fields):** Extracts 32-bit dense hexadecimal network payloads into usable metrics instantly without relying on heavy bit-shifting arithmetic at runtime.
* **Asynchronous Data Ingestion (Circular Buffer):** Implements a fast First-In-First-Out (FIFO) Ring Buffer to safely queue burst-transmissions from multiple meters, ensuring zero packet drops during high-traffic network events.
* **Data Integrity Validation (CRC/LRC):** Utilizes bitwise XOR Longitudinal Redundancy Checks to detect and discard packets corrupted by RF interference or noisy substation environments.
* **Event-Driven Architecture (Function Pointers):** Bypasses blocking `switch/case` polling by utilizing a Dispatch Table of function pointers. Critical faults instantly trigger decoupled callback functions for immediate grid protection responses.
* **Dynamic Memory Safety (Heap & Linked Lists):** Implements a dynamically allocated Linked List to log fault histories. Built with strict `malloc()` validation and complete `free()` garbage collection, guaranteeing zero memory leaks over long uptime cycles.

---

## 🚀 How to Build and Run

This project uses a standard `Makefile` build system and is designed to compile natively on any operating system using the GNU Compiler Collection (`gcc`).

### 🐧 Linux (Ubuntu/Debian)
Linux is the native environment for this project.
1. Open your terminal.
2. Ensure you have the build tools installed: `sudo apt update && sudo apt install build-essential`
3. Compile the project: `make`
4. Run the executable: `./build/telemetry_engine`

### 🍏 macOS
macOS requires the Xcode command-line tools.
1. Open the Terminal app.
2. Install the compiler tools (if not already installed): `xcode-select --install`
3. Compile the project: `make`
4. Run the executable: `./build/telemetry_engine`

### 🪟 Windows
For the best embedded development experience on Windows, using **WSL (Windows Subsystem for Linux)** is highly recommended.
**Option A: Using WSL (Recommended)**
1. Open PowerShell and install WSL: `wsl --install`
2. Open your WSL Ubuntu terminal and follow the **Linux** instructions above.

**Option B: Using MinGW / Git Bash**
1. Install [MinGW-w64](https://www.mingw-w64.org/) and ensure `gcc` and `make` are added to your Windows PATH.
2. Open Git Bash or Command Prompt in the project folder.
3. Compile the project: `make`
4. Run the executable: `.\build\telemetry_engine.exe`

---

## 🔮 Expected Improvements & Roadmap

This software is currently in the simulation phase and operates in a terminal environment. The next phases of development will port this logic directly to silicon and cloud infrastructure:

* **[Phase 2] Bare-Metal Hardware Integration:** Port the engine to an ARM Cortex-M architecture (STM32). Replace the simulated `Generate_Mock_Packet()` producer with a physical UART interrupt (UART-RX via DMA) to ingest real electrical pulses.
* **[Phase 3] RTOS Concurrency:** Wrap the parser inside a FreeRTOS task. The Circular Buffer will be upgraded to an RTOS Queue, and the Event Callbacks will trigger distinct, priority-based RTOS tasks to manage hardware relays concurrently.
* **[Phase 4] Cloud Telemetry Bridge:** Route the finalized, validated telemetry data out of the microcontroller via SPI to an ESP32, which will publish the data over MQTT/5G to a live Node-RED substation dashboard.

---
*Built with strict adherence to `-Wall -Wextra` compiler flags. Zero warnings. Zero memory leaks.*
