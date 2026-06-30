# Makefile for AVR ATmega168 USART project

MCU = atmega168
FREQ = 16000000UL

CC = avr-gcc
OBJCOPY = avr-objcopy
SIZE = avr-size

CFLAGS = -Os -Wall -Wextra -mmcu=$(MCU) -DF_CPU=$(FREQ)
CFLAGS += -I./includes

SRCS = src/main.c src/USART.c
OBJS = $(SRCS:.c=.o)

TARGET = main
ELF = $(TARGET).elf
HEX = $(TARGET).hex

PROGRAMMER = usbasp
PORT = usb

all: $(HEX)

$(ELF): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	$(SIZE) --mcu=$(MCU) $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(HEX)
	avrdude -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -U flash:w:$(HEX)

clean:
	rm -f $(OBJS) $(ELF) $(HEX)

help:
	@echo "Targets: all, clean, flash"

.PHONY: all clean flash help