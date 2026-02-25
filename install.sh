#!/bin/bash

set -e

APP_NAME="phone_contract_app"
INSTALL_DIR="/usr/local/bin"
MAN_DIR="/usr/local/share/man/man1"

if [ "$(id -u)" -ne 0 ]; then
    echo "This script must be run as sudo."
    exit 1
fi


apt-get update
apt-get install -y build-essential libncurses-dev

make -C ~/phone_contract_app

mkdir -p $INSTALL_DIR
cp ~/phone_contract_app/$APP_NAME $INSTALL_DIR

echo "OK"
