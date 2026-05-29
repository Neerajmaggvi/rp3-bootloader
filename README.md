# RP3 Custom Bootloader

A bare-metal custom bootloader for Raspberry Pi 3 written in AArch64 assembly and C.
Initializes the PL011 UART peripheral and displays the boot message with current Exception Level on the serial console.

---

## What it does

- Runs directly on hardware with no OS underneath (bare metal)
- Detects and drops from Exception Level 2 (EL2) to EL1
- Initializes PL011 UART at 115200 baud via MMIO
- Prints boot message and current Exception Level over UART
- Parks CPU cores 1, 2, 3 — only core 0 executes

---

## Output

\`\`\`
========================================
  RP3 Custom Bootloader — v1.0
  AArch64 Bare Metal
========================================
  Exception Level : EL1
  Status          : Boot successful
========================================
  Halting — extend main() to load OS
========================================
\`\`\`

---

## Project Structure

```text
rp3-bootloader/
├── Makefile          # Build system
├── linker.ld         # Linker script
├── config.txt        # RPi3 firmware configuration
└── src/
    ├── boot.S
    ├── uart.c
    ├── uart.h
    └── main.c
```

---

## How it works

## Boot Flow

```text
Power ON
   ↓
GPU firmware loads kernel8.img to RAM at 0x80000
   ↓
CPU starts executing boot.S at EL2
   ↓
boot.S detects EL2 → drops to EL1 via ERET
   ↓
Stack pointer set, BSS section zeroed
   ↓
main() called
   ↓
UART initialized
   ↓
Boot message + EL1 printed over UART
   ↓
CPU halts (WFE loop)
```

### Exception Levels (ARM AArch64)

| Level | Purpose |
|-------|---------|
| EL3 | Secure monitor |
| EL2 | Hypervisor (RPi3 starts here) |
| EL1 | Operating system (we drop here) |
| EL0 | User applications |

### UART Configuration

| Setting | Value |
|---------|-------|
| Peripheral | PL011 (UART0) |
| Base address | 0x3F201000 |
| Baud rate | 115200 |
| GPIO TX | Pin 14 |
| GPIO RX | Pin 15 |
| Data bits | 8 |
| Stop bits | 1 |
| Parity | None |

---

## Build and Run

### Build

```bash
make
```

### Run on QEMU

```bash
make run
```

### Clean

```bash
make clean
```

## Running on Real Hardware

| Item | Specification |
|------|--------------|
| Board | Raspberry Pi 3 Model B / B+ |
| SD card | MicroSD 8GB+, FAT32 formatted |
| UART adapter | USB-to-UART 3.3V (CP2102 or CH340) |
| Power supply | 5V 2.5A Micro-USB |

### UART Wiring

| RPi3 Pin | Signal | UART Adapter |
|----------|--------|--------------|
| Pin 6 | GND | GND |
| Pin 8 (GPIO14) | TX | RX |
| Pin 10 (GPIO15) | RX | TX |

> Use a 3.3V adapter only — 5V will damage GPIO pins!

---

## Development Environment

- OS: Ubuntu 22.04 LTS (VMware)
- Editor: VS Code
- Emulator: QEMU 9.0.2
- Compiler: aarch64-linux-gnu-gcc 11.4.0
- Target: Raspberry Pi 3 (AArch64 / ARMv8)

---

## Output 
![image alt](https://github.com/Neerajmaggvi/rp3-bootloader/blob/e3fb27227b890d9e8f0cafa47fcd4564cbe870d5/Output%20screenshot.png)

## Author

Neeraj
README
