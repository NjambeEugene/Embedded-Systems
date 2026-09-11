# UART Communication Protocol (STM32F103C8)

A register-level UART communication protocol built in Embedded C, developed
from first principles — no HAL, no vendor abstraction libraries.

## What it does

- Direct memory-mapped I/O access to USART1 registers (`SR`, `DR`, `CR1`)
- Byte-level `uart_send_byte()` / `uart_receive_byte()` functions using polling
  (TXE / RXNE status flags)
- A custom lightweight packet protocol to solve UART's framing problem:

```
[START_BYTE] [LENGTH] [PAYLOAD...] [CHECKSUM]
```

- XOR-based checksum for basic corruption detection

## Why register-level, not HAL

The goal of this project is to understand what's actually happening at the
hardware level — clock enables, status flags, memory-mapped registers — rather
than relying on vendor abstraction libraries that hide those details.

## Toolchain

- **Target:** STM32F103C8 ("Blue Pill", ARM Cortex-M3)
- **IDE / Simulator:** Keil MDK-ARM (uVision), simulated (no physical hardware)

## Status

Work in progress — currently implemented:

- [x] Register macros for USART1 (SR, DR, CR1)
- [x] `uart_send_byte()` / `uart_receive_byte()`
- [x] Packet struct definition
- [x] Checksum calculation function
- [x] Packet builder
- [x] Receiver state machine
- [x] UART peripheral configuration (clocks, GPIO, baud rate)
- [x] Full simulated send/receive test between two device instances
