#!/bin/bash

set -e

TARGET="dendron"
INSTALL_PATH="/usr/local/bin"
MAKEFILE="Makefile"

echo "Starting the installation of ${TARGET}..."

if [ ! -f "$MAKEFILE" ]; then
    echo "Error: ${MAKEFILE} not found. Please run this script from the project root directory."
    exit 1
fi

echo "Building the project..."
make -f "$MAKEFILE" clean
make -f "$MAKEFILE"

echo "Installing ${TARGET} to ${INSTALL_PATH}..."
echo "This may require administrator privileges."
sudo install -m 755 "$TARGET" "$INSTALL_PATH"

echo ""
echo "✅ Installation successful!"
echo "You can now run the utility from anywhere by typing: ${TARGET}"
echo "Example: ${TARGET} . -r 2"

