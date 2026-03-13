# UART Brightness Sensor Serial Shell

## Overview
This project provides a **UART-based communication shell** to interact with brightness sensors over **I²C**.  
It captures real-time brightness data, visualizes it dynamically, and logs results into Excel for further analysis.

## Features
- Command-line interface for **USB-UART devices**  
- **I²C integration** with brightness sensor (PU7020/PU8020)  
- Real-time plotting with **Matplotlib**  
- Automated **Excel data logging** for calibration and reporting  

## Tech Stack
- **Languages:** Python, C++  
- **Libraries:** `pyserial`, `matplotlib`, `openpyxl`  
- **Hardware:** FT232R USB-UART adapter, PU7020/PU8020 brightness sensor

## Getting Started

### Prerequisites
- Python 3.7+
- An FT232R USB-UART adapter connected to your machine
- Arduino flashed with `Arduino Main.ino` and the C++ source files

### Installation
```bash
git clone https://github.com/HongminAn03/UART-Brightness-Sensor.git
cd UART-Brightness-Sensor
pip install -r requirements.txt
```

### Arduino Setup
1. Open `Arduino Main.ino` in the Arduino IDE along with the C++ source files.
2. Flash the sketch to your Arduino board.
3. Connect the FT232R adapter to the Arduino's UART pins.

### Running the Shell
```bash
python Command_Shell.py
```

The script auto-detects the FT232R USB UART port and connects at 115200 baud.  
Type `h` at the `serial>` prompt to list available commands. Press `e` to exit and save the Excel log.

## File Reference
- `Arduino Main.ino` + C++ files (`i2c`, `serial`, `gpio`, etc.) — Firmware for the Arduino side
- `Command_Shell.py` — Python host shell for sending commands and logging data
- `alsensor_ctrl.txt` — I²C register initialization data for the PU7020/PU8020 sensor (reference only, not read at runtime)

## Example Output
- **Dynamic graphs** displaying ambient brightness changes  
- **Excel tables** with timestamped sensor readings (`brightness_log_<timestamp>.xlsx`)

## Use Cases
- Sensor calibration and validation  
- Hardware/software integration for display driver IC systems  
- Automated data collection for R&D experiments  

---