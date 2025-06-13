#!/bin/bash
set -e

make

read -p "Press Enter to run..."

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
"$SCRIPT_DIR/cDisk.sh"
qemu-system-x86_64 -machine q35 -m 1G -cpu qemu64 -smp 4 -serial stdio -boot d -drive file=disk.img -cdrom MaslOS2.iso -no-reboot --no-shutdown
