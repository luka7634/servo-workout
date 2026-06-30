/**
 * @file USART.c
 * @brief Implementation of USART serial communication functions
 *
 * This file contains the implementation for initializing and using
 * the USART peripheral on AVR microcontrollers. It provides functions
 * for byte transmission/reception, string output, and formatted printing.
 *
 * @author Roybel Carbonell Camejo
 * @date 2026-04-25
 *
 * @note Uses setbaud.h header for automatic baud rate calculation
 * @see USART.h For function interfaces
 */

#include "includes/CPU.h"
#include <avr/io.h>
#include "includes/USART.h"
#include <util/setbaud.h>

/**
 * @brief Initializes the USART peripheral
 *
 * Configures the USART with baud rate from setbaud.h calculations,
 * enables transmitter and receiver, and sets frame format to 8 data bits,
 * 1 stop bit, no parity.
 *
 * @note The setbaud.h header uses F_CPU and BAUD macros from CPU.h
 * @warning Baud rate error checking should be performed at compile time
 *
 * @see CPU.h For F_CPU and BAUD definitions
 * @see <util/setbaud.h> For baud rate calculation macros
 */
void initUSART(void)
{
#ifdef BAUD
#warning "BAUD IS DEFINED"
#else
#warning "BAUD ISNT DEFINED"
#endif

    /* Set baud rate registers */
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    /* Configure double speed mode if necessary */
#if USE_2X
    UCSR0A |= (1 << U2X0);
#else
    UCSR0A &= ~(1 << U2X0);
#endif

    /* Enable transmitter and receiver */
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    /* Set frame format: 8 data bits, 1 stop bit, no parity */
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

/**
 * @brief Transmits a single byte over USART
 *
 * Waits for the transmit data register to be empty, then loads the data
 * into the UDR register for transmission.
 *
 * @param data The byte to transmit (0-255)
 *
 * @note Blocking function - waits until transmission buffer is ready
 * @warning If called too frequently, may cause data loss
 */
void transmitByte(uint8_t data)
{
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = data;
}

/**
 * @brief Receives a single byte from USART
 *
 * Waits for a byte to be received (RXC flag set), then returns the
 * received data from the UDR register.
 *
 * @return uint8_t The received byte (0-255)
 *
 * @note Blocking function - waits indefinitely for incoming data
 * @warning Ensure data is available before calling to avoid infinite loop
 */
uint8_t receiveByte(void)
{
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}

/**
 * @brief Sends a null-terminated string over USART
 *
 * Iterates through each character in the string and transmits it
 * one by one until the null terminator is reached.
 *
 * @param String Pointer to the null-terminated string to transmit
 *
 * @note Blocking - each character waits for transmission to complete
 * @warning String must be properly null-terminated
 */
void printString(const char *String)
{
    while (*String)
        transmitByte(*String++);
}

/**
 * @brief Prints a byte as a 3-digit decimal number
 *
 * Converts the byte value to a fixed-width decimal format with
 * leading zeros (000-255) and transmits each digit.
 *
 * @param byte The byte value to print as decimal (0-255)
 *
 * @note Output is always exactly 3 characters
 * @example printByte(5) sends "005"
 * @example printByte(127) sends "127"
 *
 * @see printBinaryByte For binary representation
 */
void printByte(uint8_t byte)
{
    transmitByte('0' + (byte / 100));
    transmitByte('0' + ((byte / 10) % 10));
    transmitByte('0' + (byte % 10));
}

/**
 * @brief Prints a byte as an 8-bit binary string
 *
 * Iterates through each bit from MSB (bit 7) to LSB (bit 0) and
 * transmits '1' if the bit is set, '0' otherwise.
 *
 * @param byte The byte value to display in binary format
 *
 * @note Output is exactly 8 characters
 * @note MSB (bit 7) is printed first
 *
 * @example printBinaryByte(0x0F) sends "00001111"
 * @example printBinaryByte(0xAA) sends "10101010"
 *
 * @see printByte For decimal representation
 * @see bit_is_set Macro from avr/io.h
 */
void printBinaryByte(uint8_t byte)
{
    uint8_t bit;
    for (bit = 7; bit < 255; bit--)
    {
        if (bit_is_set(byte, bit))
            transmitByte('1');
        else
            transmitByte('0');
    }
}