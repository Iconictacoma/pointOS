#!/bin/bash

# Setup script for pointOS development environment

echo "Setting up pointOS development environment"

# Check for required tools
check_tool() {
    if ! command -v $1 &> /dev/null; then
        echo "$1 is not installed"
        return 1
    else
        echo "$1 found"
        return 0
    fi
}

echo ""
echo "Checking for required tools:"
check_tool "nasm"
check_tool "gcc"
check_tool "qemu-system-i386"

echo ""
echo "Setup is now complete. You can now run:"
echo "  make build    - Build the OS"
echo "  make run      - Run in QEMU"
echo "  make debug    - Debug with GDB"
echo "  make clean    - Clean build artifacts"
