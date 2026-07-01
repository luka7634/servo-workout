#include "includes/CPU.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "includes/pinDefines.h"
#include "includes/USART.h"

#define PULSE_MIN 1000
#define PULSE_MAX 2000
#define PULSE_MID 1500

static inline uint16_t getNumber16(void);
static inline void initTimer1Servo(void);
static inline void showOff(void);

int main()
{
    // inits
    uint16_t servoPulseLength;
    OCR1A = 2000; /**< set it to de mid position initially */
    initTimer1Servo();
    initUSART();
    printString("\r\nServo Demo\r\n");
    showOff();

    while (1)
    {
        printString("\r\nEnter a 4-digit pulse length:\r\n");
        servoPulseLength = getNumber16();

        printString("Give me a sec...\r\n");
        OCR1A = servoPulseLength;
        DDRB |= (1 << SERVO_OUT); /**< re-enable output pin */

        _delay_ms(1000);
        printString("Releasing...\r\n");
        while (TCNT1 < 3000)
        {
        }                          /**< delay until pulse part of cycle done */
        DDRB &= ~(1 << SERVO_OUT); /**< disable output pin */
    }
}

static inline uint16_t getNumber16(void) /**< Gets pwm value from serial port, reads characters and turns them into numbers */
{
    char thousands = '0';
    char hundreds = '0';
    char tens = '0';
    char ones = '0';
    char thisChar = '0';

    do
    {
        thousands = hundreds; /**< shift number over */
        hundreds = tens;
        tens = ones;
        ones = thisChar;
        thisChar = receiveByte(); /**< get a new character */
        transmitByte(thisChar);   /**< echo */
    } while (thisChar != '\r');

    transmitByte('\n');
    return (1000 * (thousands - '0') + 100 * (hundreds - '0') + 10 * (tens - '0') + (ones - '0'));
}

static inline void initTimer1Servo(void)
{
    /**< set up timer1 (16bit) to give a pulse every 20ms */
    /**< use fast pwm mode, counter max in ICR1 */
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12) | (1 << WGM13);
    TCCR1B |= (1 << CS10);    /**< /1 prescaling -> counting in microseconds */
    ICR1 = 20000;             /**< top value 20ms */
    TCCR1A |= (1 << COM1A1);  /**< direct output on PB1 / OC1A */
    DDRB |= (1 << SERVO_OUT); /**< set pin for output */
}

static inline void showOff(void)
{
    printString("Center\r\n");
    OCR1A = PULSE_MID;
    _delay_ms(1500);
    printString("Clockwise Max\r\n");
    OCR1A = PULSE_MIN;
    _delay_ms(1500);
    printString("Counterclockwise Max\r\n");
    OCR1A = PULSE_MAX;
    _delay_ms(1500);
    printString("Center\r\n");
    OCR1A = PULSE_MID;
    _delay_ms(1500);
}