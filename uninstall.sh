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

CONFIG_DIR="$HOME/.config/dendron"
CONFIG_FILE="${CONFIG_DIR}/config.ini"
if [ -f "$CONFIG_FILE" ]; then
    echo "Removing default configuration file..."
    rm "$CONFIG_FILE"
    echo "✅ Default configuration file removed."
    # Попытка удалить директорию конфигурации, если она пуста
    rmdir "$CONFIG_DIR" 2>/dev/null && echo "✅ Removed empty configuration directory." || true
fi

echo "Uninstallation process finished."
