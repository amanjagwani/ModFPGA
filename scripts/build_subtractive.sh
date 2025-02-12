#!/bin/bash
# Subtractive Synth Builder

# Get script directory
SCRIPT_DIR=$(dirname "$(realpath "$0")")
ROOT_DIR=$(realpath "$SCRIPT_DIR/..")

# Source common setup
source "$SCRIPT_DIR/build_common.sh"

# Project configuration
PROJECT_NAME="subtractive"
XSA_FILE="$ROOT_DIR/hardware/platforms/subtractive/eight_voice_uart_harms.xsa"
SRC_DIR="$ROOT_DIR/software/vitis"
BUILD_DIR="$ROOT_DIR/build/$PROJECT_NAME"

# Validate hardware
if [ ! -f "$XSA_FILE" ]; then
    echo "Error: XSA file not found at $XSA_FILE"
    echo "Run Vivado export first or check hardware/platforms"
    exit 1
fi


# Clear the previous build directory
if [ -d "$BUILD_DIR" ]; then
    echo "Clearing build directory: $BUILD_DIR"
    rm -rf "$BUILD_DIR"
fi

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

# Run build
echo "Building $PROJECT_NAME..."
xsct -quiet "$SCRIPT_DIR/build_common.tcl" \
    "$PROJECT_NAME" \
    "$XSA_FILE" \
    "$SRC_DIR"

# Result check
if [ -f "$BUILD_DIR/$PROJECT_NAME/Debug/$PROJECT_NAME.elf" ]; then
    echo "Success! Output: $BUILD_DIR/$PROJECT_NAME/Debug/$PROJECT_NAME.elf"
else
    echo "Build failed! Check logs in $BUILD_DIR"
    exit 1
fi
