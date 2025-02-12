#!/bin/bash
# Xilinx Environment Setup

# Set fail-on-error
set -e

# Configure paths (customizable via environment variables)
export XILINX_ROOT=${XILINX_ROOT:-"/opt/Xilinx"}
export XILINX_VERSION=${XILINX_VERSION:-"2022.2"}

# Verify Xilinx installation
VITIS_PATH="$XILINX_ROOT/Vitis/$XILINX_VERSION"
if [ ! -f "$VITIS_PATH/settings64.sh" ]; then
    echo "ERROR: Xilinx tools not found!"
    echo "1. Confirm installation exists at: $VITIS_PATH"
    echo "2. Or set custom path:"
    echo "   export XILINX_ROOT=/your/install/path"
    echo "   export XILINX_VERSION=your_version"
    exit 1
fi

# Setup environment
echo "Initializing Xilinx Vitis $XILINX_VERSION..."
source "$VITIS_PATH/settings64.sh"