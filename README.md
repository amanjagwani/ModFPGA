# ModFPGA

## Overview

A modular synthesizer system made with HLS for FPGAs. Each IP core can be treated as a module of a modular synthesizer and can be interconnected in a block design to generate a range of sonic possibilities.

ModFPGA is a FPGA-based audio processing project that leverages Xilinx’s Vitis/Vivado flow on the Digilent Zybo Z7020 SOC board. The project is organized as a collection of custom HLS IP cores (e.g. ADSR, Butterworth, FMOsc, FourPole, Mixer, OscBank, PhaseGen, Resonator, ReverbSC, Tone), complete hardware platforms (block designs for subtractive and FM synthesis), and accompanying Vitis application code. The repository also includes build scripts for automating the creation of IP cores, building the Vitis projects, and programming the Zybo Z7020 board.
The csound sub-directory contains a cross-compile file to build csound to run on the ARM Cortex A9 of the Zynq-7000

## Prerequisites

- **Xilinx Tools:**

  - Vitis HLS
  - Vivado
  - Vitis

- **Hardware:**

  - Zybo Z7020 board
  - JTAG cable
  - UART Midi Break-out board
  - MIDI Interface or controller

- **Operating System:**
  - Linux
  - Ensure that your environment variables are set for Xilinx tools (e.g., `XILINX_ROOT` and `XILINX_VERSION`)

## Hardware Set-up

The provided synthesizer designs respond to midi messages via uart on MIO 14 and 15 (Pins 9 and 10 on the JF PMOD header on the Zybo Z7020). Make sure to connect the RX signal to pin 9 and TX signal to pin 10 along with the appropriate VCC and GND connections.

## Building the Vitis Projects

Two complete designs are provided: **subtractive synthesis** and **FM synthesis**.  
Build these projects by running the provided build scripts in the `scripts/` directory:

1. **Subtractive Synthesis:**

   ```bash
   cd scripts
   ./build_subtractive.sh
   ```

2. **FM Synthesis:**

   ```bash
   cd scripts
   ./build_fm.sh
   ```

These scripts set up the Vitis environment, build the project, and place the final ELF file (and bitstream) in the build folder.

The build scripts use an XSCT TCL script (build_common.tcl) to create the application project, import sources, configure the linker, and build the project.

## Programming the Board

A programming script is provided for each design to program both the FPGA and the PS. This script:

- Connects to the target via the hardware server.
- Programs the FPGA with the bitstream.
- Runs the PS initialization.
- Downloads the ARM ELF file.
- Starts execution.

To program the board, run:

```bash
cd scripts
./program_subtractive.sh
```

or

```bash
cd scripts
./program_fm.sh
```

These shell scripts call the XSCT TCL script program_common.tcl and passes it the paths to:

- The ELF file.
- The FPGA bitstream.
- The PS initialization script (ps7_init.tcl).

Make sure the board is connected before executing this script.
