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

echo "Updating package list and installing dependencis..."
apt-get update
apt-get install -y build-essential libncurses-dev libsqlite3-dev sqlite3

echo "Compiling the application..."
make -C ~/phone_contract_app

echo "Installing the application..."
mkdir -p $INSTALL_DIR
cp ~/phone_contract_app/$APP_NAME $INSTALL_DIR

# Optional:

echo "Installation complete. You can now run the application by typing '$APP_NAME'."
