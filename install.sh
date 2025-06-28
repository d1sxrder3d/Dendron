#!/bin/bash

set -e

TARGET="dendron"
INSTALL_PATH="/usr/local/bin"
MAKEFILE="Makefile"
VERSION="1.0.0"

echo "Starting the installation of ${TARGET} version ${VERSION}..."

if [ ! -f "$MAKEFILE" ]; then
    echo "Error: ${MAKEFILE} not found. Please run this script from the project root directory."
    exit 1
fi

echo "Building the project..."
make clean
make

echo "Installing ${TARGET} to ${INSTALL_PATH}..."
echo "This may require administrator privileges."
sudo install -m 755 "$TARGET" "$INSTALL_PATH"

echo "Installing configuration files..."
SOURCE_CONFIG="configs/config.ini"
CONFIG_DIR="$HOME/.config/dendron"

if [ -f "$SOURCE_CONFIG" ]; then
    mkdir -p "$CONFIG_DIR"
    if [ ! -f "$CONFIG_DIR/config.ini" ]; then
        cp "$SOURCE_CONFIG" "$CONFIG_DIR/"
        echo "Default config created at $CONFIG_DIR/config.ini"
    else
        echo "Config already exists at $CONFIG_DIR/config.ini. Not overwriting."
    fi
else
    echo "Warning: Default config file not found at ${SOURCE_CONFIG}. Skipping."
fi

echo "Cleaning up build files..."
make clean

echo ""
echo "✅ Installation successful!"
echo "You can now run the utility from anywhere by typing: ${TARGET}"
echo "Example: ${TARGET} . -r 2"
