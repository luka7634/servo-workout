# Servo Control with Potentiometer (AVR Project)

This project demonstrates how to control a servo motor using a potentiometer with an AVR microcontroller (e.g., ATmega328P). It reads the analog voltage from a potentiometer via ADC, maps it to a servo pulse width, and outputs the current ADC value over USART (serial) for monitoring.

## Features

- **Real-time servo control** – Adjust the servo angle by turning the potentiometer.
- **Serial output** – Sends the raw ADC value (0–1023) over UART for debugging or external logging.
- **Startup sequence** – Performs a "show off" routine to demonstrate servo movement (mid → max → min → mid).
- **Efficient PWM** – Uses Timer1 in Fast PWM mode with a 50 Hz frequency (20 ms period) for stable servo operation.
- **Configurable pulse range** – Easily adjust `PULSE_MIN`, `PULSE_MID`, and `PULSE_MAX` (in timer ticks) to match your servo’s specifications.

## Hardware Requirements

- AVR microcontroller (e.g., ATmega328P, ATmega8, etc.)
- Servo motor (standard 50 Hz PWM)
- Potentiometer (e.g., 10 kΩ linear)
- Power supply (5V for servo and microcontroller, or external for servo)
- USB-to-serial adapter (if using USART for output)
- (Optional) LEDs or other indicators

## Pin Connections

| Component     | AVR Pin        | Description                         |
|---------------|----------------|-------------------------------------|
| Servo signal  | PB1 (OC1A)     | PWM output from Timer1 channel A    |
| Potentiometer | PC0 (ADC0)     | Analog input channel 0              |
| USART TX      | PD1 (TXD)      | Serial output (optional)            |
| USART RX      | PD0 (RXD)      | Not used in this example            |
| VCC           | 5V             | Power for servo and microcontroller |
| GND           | GND            | Common ground                       |

> **Note:** The pin names correspond to the ATmega168. If you use a different AVR, adjust the pin definitions in `src/includes/pinDefines.h` accordingly.

## How It Works

1. **Timer1 Configuration**  
   Timer1 is set to Fast PWM mode (mode 14) with ICR1 as the TOP value (4999). The prescaler is 64, and the system clock is assumed to be 16 MHz. This yields a PWM frequency of 50 Hz (period = 20 ms).  
   - `ICR1 = 4999` → PWM period = (4999+1) * 64 / 16 MHz = 20 ms.  
   - The output compare register `OCR1A` controls the pulse width. Values from 250 (1 ms) to 500 (2 ms) are typical for servo control.

2. **ADC Reading**  
   The ADC is configured with AVcc as reference and a prescaler of 128 (for 16 MHz clock). The `readADC()` function reads channel 0 (potentiometer) and returns a 10‑bit value (0–1023).

3. **Mapping**  
   The ADC value is linearly mapped to the pulse width range [250, 500] using the formula:  
   `pulse = 250 + (adc * 250 / 1023)`.  
   This provides a smooth transition from minimum to maximum servo position.

4. **Serial Output**  
   Every loop iteration, the raw ADC value is sent as a decimal string followed by a newline via USART (configured at 9600 baud by default). This allows you to monitor the potentiometer position in a serial terminal.

5. **Startup Routine**  
   The `showOff()` function runs once at power‑up, moving the servo to mid, max, min, and back to mid with 1.5‑second delays between positions. This is useful for testing and calibration.

## Building and Flashing

### Prerequisites
- AVR-GCC toolchain (avr-gcc, avr-libc, avr-binutils)
- Make
- AVR programmer (e.g., AVRISP, USBasp, or Arduino as ISP)

### Steps
1. Clone or download the project files.
2. Open a terminal in the project root directory (where `Makefile` is located).
3. Edit the `Makefile` if necessary (e.g., set the correct MCU type, programmer, and port).
4. Build the firmware:
   ```bash
   make
   ```
   This will compile the source files and produce a `.hex` file.
5. Connect your programmer and microcontroller.
6. Flash the firmware:
   ```bash
   make flash
   ```
7. (Optional) Set fuse bits (if needed) – check the `Makefile` for any fuse settings.

## Usage

1. Power up the circuit.
2. The servo will perform the startup routine.
3. Turn the potentiometer knob; the servo angle should follow proportionally.
4. Connect a serial terminal (e.g., PuTTY, screen, or Arduino Serial Monitor) to the USART pins (TX) at 9600 baud, 8N1 to see the ADC values printed.

## Customization

- **Adjust pulse range** – Change `PULSE_MIN`, `PULSE_MID`, and `PULSE_MAX` in `main.c` to suit your servo’s required pulse widths.
- **Change ADC channel** – Modify the argument in `readADC(0)` to use a different analog input.
- **Baud rate** – Modify the `initUSART()` call (set in `USART.c`) to change the serial speed.
- **PWM frequency** – Adjust `ICR1` and/or the prescaler to change the PWM period.

## Project Structure

```
.
├── Makefile
└── src/
    ├── main.c
    ├── USART.c
    └── includes/
        ├── CPU.h
        ├── USART.h
        └── pinDefines.h
```

- **main.c** – Contains the main loop, Timer1 initialization, ADC, and servo logic.
- **USART.c/h** – Provides functions for serial communication (initialization, string printing).
- **pinDefines.h** – Defines pin names for the specific microcontroller (e.g., `SERVO_OUT`, `LED`).
- **CPU.h** – Probably contains CPU‑specific settings (like F_CPU definition) – ensure it matches your clock frequency.

## Dependencies

- [AVR Libc](https://www.nongnu.org/avr-libc/) – Standard C library for AVR.
- The project assumes a 16 MHz system clock (set `F_CPU` accordingly in `CPU.h` or via compiler flags).

## Troubleshooting

- **Servo jitter** – Ensure stable power supply; the servo can draw significant current.
- **No serial output** – Check baud rate, connections, and that `initUSART()` is called.
- **Servo not moving** – Verify the PWM pin and that the timer is configured correctly. Use an oscilloscope or logic analyzer to check the PWM signal.
- **ADC values erratic** – Add decoupling capacitors near the potentiometer and ensure the reference voltage is stable.

## License

This project is open‑source and provided as‑is. Feel free to modify and distribute it.