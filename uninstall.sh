#!/bin/bash

set -e

TARGET="dendron"
INSTALL_PATH="/usr/local/bin/${TARGET}"
MAKEFILE="Makefile"

echo "Starting uninstallation of ${TARGET}..."

if [ -f "$INSTALL_PATH" ]; then
    echo "Removing ${TARGET} from ${INSTALL_PATH}..."
    echo "This may require administrator privileges."
    sudo rm "$INSTALL_PATH"
    echo "✅ ${TARGET} has been uninstalled."
else
    echo "Info: ${TARGET} is not installed in ${INSTALL_PATH}. Nothing to remove."
fi

if [ -f "$MAKEFILE" ]; then
    echo "Cleaning up build files..."
    make -f "$MAKEFILE" clean
fi

echo "Uninstallation process finished."

