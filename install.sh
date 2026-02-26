#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e

APP_NAME="phone_contract_app"
INSTALL_DIR="/usr/local/bin"
MAN_DIR="/usr/local/share/man/man1"

# Check for root privileges
if [ "$(id -u)" -ne 0 ]; then
    echo "This script must be run as root. Please use sudo."
    exit 1
fi

# Get the directory where the script is located
SCRIPT_DIR="$( cd "$( dirname "$(BASH_SOURCE[0])" )" &> /dev/null && pwd )"

echo "Updating package list and installing dependencis..."
apt-get update
apt-get install -y build-essential libncurses-dev libsqlite3-dev sqlite3

echo "Compiling the application..."
# Use the script's directory to find the Makefile
make -C "$SCRIPT_DIR"

echo "Installing the application..."
mkdir -p $INSTALL_DIR
cp "$SCRIPT_DIR/$APP_NAME" $INSTALL_DIR

# Optional:

echo "Installation complete. You can now run the application by typing '$APP_NAME'."
