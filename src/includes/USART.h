/**
 * @file USART.h
 * @brief USART serial communication interface
 * 
 * This file provides functions for initializing and using the USART
 * (Universal Synchronous/Asynchronous Receiver/Transmitter) peripheral
 * for serial communication. It supports byte transmission/reception,
 * string printing, and formatted output.
 * 
 * @author Roybel Carbonell Camejo
 * @date 2026-04-25
 * 
 * @note All functions are blocking by design for simplicity
 * @see CPU.h For F_CPU and BAUD configuration
 */

#ifndef USART_H
#define USART_H

/**
 * @brief Initializes the USART peripheral for serial communication
 * 
 * Configures the USART with the following settings:
 * - Asynchronous mode
 * - Baud rate defined by BAUD macro (from CPU.h)
 * - 8 data bits
 * - 1 stop bit
 * - No parity
 * - Transmitter and receiver enabled
 * 
 * @note Must be called before any other USART functions
 * @warning Baud rate calculation depends on F_CPU macro from CPU.h
 * 
 * @see CPU.h For F_CPU and BAUD definitions
 */
void initUSART(void);

/**
 * @brief Sends a null-terminated string over USART
 * 
 * Transmits each character of the string sequentially until the null
 * terminator ('\0') is reached.
 * 
 * @param String Pointer to the null-terminated string to transmit
 * 
 * @note Function is blocking - waits for each byte to be transmitted
 * @warning String must be properly null-terminated to avoid buffer overrun
 * 
 * @see transmitByte For single byte transmission
 */
void printString(const char String[]);

/**
 * @brief Receives a single byte from USART
 * 
 * Waits for a byte to be received on the USART RX line and returns it.
 * Function blocks until data is available.
 * 
 * @return uint8_t The received byte (0-255)
 * 
 * @note Blocking function - will wait indefinitely for incoming data
 * @warning Ensure the transmitter is sending data to avoid infinite wait
 */
uint8_t receiveByte(void);

/**
 * @brief Transmits a single byte over USART
 * 
 * Sends one byte of data through the USART TX line. Function blocks
 * until the transmission is complete.
 * 
 * @param data The byte (0-255) to transmit
 * 
 * @note Useful for sending raw binary data
 * @see printString For sending strings
 * @see printByte For sending bytes as formatted text
 */
void transmitByte(uint8_t data);

/**
 * @brief Prints a byte as a formatted decimal number
 * 
 * Converts the byte value to a 3-digit decimal string (000-255) and
 * sends it over USART. Leading zeros are included for fixed width.
 * 
 * @param byte The byte value (0-255) to print as decimal
 * 
 * @note Output format: Always 3 digits (e.g., "000", "001", ..., "255")
 * @see printBinaryByte For binary representation
 */
void printByte(uint8_t byte);

/**
 * @brief Prints a byte as an 8-bit binary string
 * 
 * Converts the byte to an 8-character binary representation (each bit
 * shown as '0' or '1') and sends it over USART.
 * 
 * @param byte The byte value to display in binary format
 * 
 * @note Output format: 8 bits (e.g., "10101010")
 * @note MSB (bit 7) is printed first
 * 
 * @see printByte For decimal representation
 */
void printBinaryByte(uint8_t byte);

#endif /* USART_H */