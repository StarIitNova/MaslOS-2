#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
"$SCRIPT_DIR/cDisk.sh"
qemu-system-x86_64 -machine q35 -m 1G -cpu qemu64 -serial stdio -drive file=disk.img -boot d -cdrom MaslOS2.iso
