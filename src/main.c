#include "includes/CPU.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "includes/pinDefines.h"
#include "includes/USART.h"

#define PULSE_MIN 250 // 1 ms
#define PULSE_MID 375 // 1.5 ms
#define PULSE_MAX 500 // 2 ms

void initTimer1Servo();
void showOff();
static inline void initADC();
uint16_t readADC(uint8_t channel);

int main(void)
{
    initTimer1Servo();
    OCR1A = 375;
    initADC();
    initUSART();

    printString("\r\nControl Servo con Potenciometro\r\n");

    DDRB |= (1 << SERVO_OUT); // Habilita la salida del servo permanentemente

    showOff();

    for (uint16_t i = 250; i <= 500; i++)
    {
        OCR1A = i;
        _delay_ms(150);
    }

    while (1)
    {
        uint16_t adcValue = readADC(0);
        uint16_t pulse = 250 + (adcValue * 250 / 1023);

        OCR1A = pulse;

        _delay_ms(15);

        char buffer[6];
        sprintf(buffer, "%u", adcValue);
        printString(buffer);
        printString("\r\n");
    }

    return 0;
}

void initTimer1Servo()
{
    DDRB |= (1 << SERVO_OUT);
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10);
    ICR1 = 4999;
}

static inline void initADC()
{
    ADMUX |= (1 << REFS0);

    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t readADC(uint8_t channel)
{

    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA |= (1 << ADSC);

    while (ADCSRA & (1 << ADSC))
        ;

    return ADC;
}

uint16_t mapADCtoServo(uint16_t adc)
{
    return 250 + (adc * 250 / 1023);
}

void showOff()
{
    OCR1A = PULSE_MID;
    _delay_ms(1500);
    OCR1A = PULSE_MAX;
    _delay_ms(1500);
    OCR1A = PULSE_MIN;
    _delay_ms(1500);
    OCR1A = PULSE_MID;
    _delay_ms(1500);
}
