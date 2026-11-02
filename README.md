# Raspberry Pi 4B (Broadcom BCM2711) – Hobby Kernel

Bare-Metal-64-Bit-Multicore-Betriebssystemkernel für den Raspberry Pi 4 Model B.

Dieses Projekt ist während meines Präsenzdienstes entstanden. Der Kernel wurde bereits auf echter Hardware ausgeführt und mit selbst entwickelten Userspace-Testprogrammen getestet.

Aus Zeitgründen wurde die Entwicklung seitdem nicht weitergeführt. Insbesondere die Multi-Core-Architektur befindet sich noch in einem experimentellen Entwicklungsstand und ist hinsichtlich Prozessverwaltung und Synchronisierung nicht vollständig ausgebaut.

**CPU:** ARM Cortex-A72 (4 Kerne)  
**Architektur:** AArch64 / ARMv8-A

## Funktionen

- Multiprocessing-Unterstützung für alle vier CPU-Kerne
- Program-Loader für ELF64-Binaries
- Ausführung eigener Userspace-Programme
- Prioritätsbasierter Scheduler mit dynamischer Counter-Berechnung
- Präemptives Scheduling und Context Switching
- System Calls:
  - `fork`, `exit`, `spawn`
  - `alloc_heap`, `yield`
  - `get_pid`, `set_prio`, `print`
- Memory Management:
  - Virtual Memory mit Paging und Memory Protection
  - Verwaltung und Mapping virtueller Adressräume (`vmem`)
  - Copy-on-Write (CoW) zur effizienten Duplizierung von Adressräumen bei `fork`
  - Verwaltung physischer Page Frames (`pmem`)
  - Mehrere eigens entwickelte Heap-Allocator
- Eigenes Logging-Framework

## Hardware-Treiber

- **SD/eMMC:** Zugriff auf die Micro-SD-Karte
- **FAT32:** Unterstützung mehrerer Partitionen und Dateizugriff
- **Mailbox:** Kommunikation mit der VideoCore-GPU-Firmware
- **DMA:** Direct Memory Access zur hardwarebeschleunigten Datenübertragung
- **Framebuffer:** Grafische Ausgabe mit Zeichenfunktionen, Textformatierung und DMA-beschleunigtem Rendering
- **GPIO:** Zugriff auf die General Purpose Input/Output Pins
- **UART:** Serielle Kommunikation und Kernel-Debugging
- **MMU:** Verwaltung virtueller Speicheradressen und Speicherschutz
- **ARM GIC:** Generic Interrupt Controller zur Verwaltung von Hardware-Interrupts
- **ARM Generic Timer:** Zeitmessung und Timer-Interrupts

## Software / Entwicklungsumgebung

- GNU Toolchain und AArch64 Cross-Compiler
- QEMU zur Emulation und zum Testen
- GDB für Kernel-Debugging
- GNU Make für den Build-Prozess
