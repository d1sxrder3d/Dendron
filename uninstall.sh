#!/bin/bash

set -e

TARGET="dendron"
INSTALL_DIR="/usr/local/bin"
INSTALL_PATH="${INSTALL_DIR}/${TARGET}"
MAKEFILE="Makefile"

echo "Starting uninstallation of ${TARGET}..."

if [ -f "$INSTALL_PATH" ]; then
    echo "Removing ${TARGET} from ${INSTALL_DIR}..."
    echo "This may require administrator privileges."
    sudo rm "$INSTALL_PATH"
    echo "✅ ${TARGET} has been uninstalled."
else
    echo "Info: ${TARGET} is not installed in ${INSTALL_DIR}. Nothing to remove."
fi

if [ -f "$MAKEFILE" ]; then
    echo "Cleaning up build files..."
    make clean
fi

# Determine config directory based on OS
if [[ "$(uname)" == "Darwin" ]]; then
    CONFIG_DIR="$HOME/Library/Application Support/dendron"
else # Assume Linux/other Unix-like
    CONFIG_DIR="$HOME/.config/dendron"
fi

CONFIG_FILE="${CONFIG_DIR}/config.ini"
if [ -f "$CONFIG_FILE" ]; then
    echo "Removing default configuration file..."
    rm "$CONFIG_FILE"
    echo "✅ Default configuration file removed."
    # Attempt to remove the configuration directory if it is empty
    rmdir "$CONFIG_DIR" 2>/dev/null && echo "✅ Removed empty configuration directory." || true
fi

echo "Uninstallation process finished."
