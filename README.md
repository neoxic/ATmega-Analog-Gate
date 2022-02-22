Simple analog gate firmware for ATmega MCUs
===========================================

Dependencies
------------

+ cmake
+ avr-gcc
+ avr-libc
+ avrdude


Configuration
-------------

There are a couple of _#define_ values at the beginning of _src/main.c_ to set up the gate.

| Value | Description                                                                         |
|-------|-------------------------------------------------------------------------------------|
| AVCC  | Voltage on the AVCC pin (must be measured with a multimeter)                        |
| VIN1  | The output pin goes high if voltage on the input pin becomes higher than this value |
| VIN2  | The output pin goes low if voltage on the input pin becomes lower than this value   |

* The difference between VIN1 and VIN2 (VIN1 > VIN2) defines the depth of hysteresis between output transitions.

* The output pin can be inverted via the OUT_INV option.


Installation
------------

To build, run (with your own MCU):

    cmake -B build -D MCU=ATMEGA32U4
	cd build
    make

To install on the device using AVRDUDE, run (with your own programmer):

	make PROG=usbasp flash

or for Teensy 2.0:

    make flash-teensy


Pinout
------

| Pin       | I/O | Description    |
|-----------|-----|----------------|
| PF4 (PC4) | AIN | Input          |
| PD7       | OUT | Output         |
| PD6 (PB5) | OUT | Status LED     |

Pins in brackets pertain to 32-pin devices, i.e. ATmega88/168/328/etc.
