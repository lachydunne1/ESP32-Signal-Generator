# ESP32 Signal Generator

This repository contains code for an ESP32-based signal generator. The project demonstrates how to control the DAC and ADC of the ESP32 to generate and read signals.

## Files

- **main.c**: Contains the main application code.
- **dac.c**: Contains functions for controlling the DAC, including setting up the DAC, changing frequencies, and inverting DAC output.
- **adc.c**: Contains functions for reading ADC values and calculating average voltage.
- **CMakeLists.txt**: CMake configuration for building the project.

## Features

- **DAC Control**: Functions to initialize the DAC, change output frequency, and invert output signals.
- **Frequency Response**: Bode function sweeps user defined frequency range.

## Getting Started

1. **Clone the Repository**:
   ```sh
   git clone https://github.com/yourusername/ESP32_Signal_Generator.git
   cd ESP32_Signal_Generator
