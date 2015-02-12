
F_CPU = 4000000UL
MCU = attiny25

TARGET = schrecklicht
OBJS = schrecklicht.o

PROGRAMMER = -c buspirate -P /dev/ttyUSB0
#PROGRAMMER = -c usbasp

FORMAT = ihex
CC = avr-gcc
CFLAGS += -Wall -g -Os -mmcu=$(MCU) -D F_CPU=$(F_CPU) -std=gnu99 -pedantic-errors -Werror
LDFLAGS +=
OBJCOPY = avr-objcopy

all: $(TARGET).hex


program: $(TARGET).hex
	avrdude -p $(MCU) $(PROGRAMMER) -U flash:w:$(TARGET).hex

fuses:
	avrdude -p $(MCU) $(PROGRAMMER) -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $<

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<
