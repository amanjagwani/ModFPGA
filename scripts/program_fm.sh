#!/bin/bash

# Get script directory and root directory
SCRIPT_DIR=$(dirname "$(realpath "$0")")
ROOT_DIR=$(realpath "$SCRIPT_DIR/..")

# Project configuration
PROJECT_NAME="fm"
ELF_FILE="$ROOT_DIR/build/fm/fm/Debug/fm.elf"
BITSTREAM_FILE="$ROOT_DIR/build/fm/fm/_ide/bitstream/eight_voice_fm_v1.bit"
PS_INIT_TCL="$ROOT_DIR/build/fm/fm/_ide/psinit/ps7_init.tcl"

# Source common setup
source "$SCRIPT_DIR/build_common.sh"

# Validate files
if [ ! -f "$ELF_FILE" ]; then
    echo "Error: ELF file not found at $ELF_FILE"
    exit 1
fi

if [ ! -f "$BITSTREAM_FILE" ]; then
    echo "Error: Bitstream file not found at $BITSTREAM_FILE"
    exit 1
fi

if [ ! -f "$PS_INIT_TCL" ]; then
    echo "Error: PS init TCL script not found at $PS_INIT_TCL"
    exit 1
fi

echo "Programming project '$PROJECT_NAME'..."
echo "ELF: $ELF_FILE"
echo "Bitstream: $BITSTREAM_FILE"
echo "PS init TCL: $PS_INIT_TCL"

# Call the TCL script via xsct, passing the three arguments.
xsct "$SCRIPT_DIR/program_common.tcl" "$ELF_FILE" "$BITSTREAM_FILE" "$PS_INIT_TCL"

echo "Programming complete."
