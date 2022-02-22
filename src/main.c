/*
** Copyright (C) 2022 Arseny Vakhrushev <arseny.vakhrushev@me.com>
**
** This firmware is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This firmware is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this firmware. If not, see <http://www.gnu.org/licenses/>.
*/

#include <avr/io.h>

#define AVCC 5.000 // AVCC pin voltage (V)
#define VIN1 2.750 // Input high threshold voltage (V)
#define VIN2 2.250 // Input low threshold voltage (V)

// #define CLK_16MHZ // Use F_CPU=16Mhz instead of 8Mhz (VCC=5V is required)
// #define OUT_INV // Output active low
#define OUT_ODR PORTD
#define OUT_DDR DDRD
#define OUT_PIN 0x80 // D7
#ifndef PORTE // 32-pin device
#define LED_ODR PORTB
#define LED_DDR DDRB
#define LED_PIN 0x20 // B5
#else
#define LED_ODR PORTD
#define LED_DDR DDRD
#define LED_PIN 0x40 // D6
#endif

void main(void) {
	CLKPR = 0x80;
#ifdef CLK_16MHZ
	CLKPR = 0x00; // 16Mhz clock
#else
	CLKPR = 0x01; // 8Mhz clock
#endif

	OUT_DDR |= OUT_PIN;
	LED_DDR |= LED_PIN;
#ifndef PORTE // 32-pin device
	PORTB = 0xff;
	PORTC = 0xef; // C4 floating
	PORTD = 0xff;
#else
	PORTB = 0xff;
	PORTC = 0xff;
	PORTD = 0xff;
	PORTE = 0xff;
	PORTF = 0xef; // F4 floating
#endif
#ifndef OUT_INV
	OUT_ODR &= ~OUT_PIN;
#endif
	ADMUX = 0x44; // MUX=ADC4, REFS=01 (VCC)

	uint16_t buf[64] = {0};
	for (uint8_t i = 0;;) {
		ADCSRA = 0xc7; // ADPS=111, ADSC=1, ADEN=1 (start conversion, CLK/128)
		while (ADCSRA & 0x40); // ADSC=1 (conversion in progress)
		buf[i++] = ADC;
		if (i == 64) i = 0;
		uint16_t val = 0;
		uint8_t j = i;
		do {
			val += buf[j++];
			if (j == 64) j = 0;
		} while (i != j);
		val >>= 6;
		if (val > (VIN1 / AVCC) * 1024) {
#ifdef OUT_INV
			OUT_ODR &= ~OUT_PIN;
#else
			OUT_ODR |= OUT_PIN;
#endif
			LED_ODR |= LED_PIN;
		} else if (val < (VIN2 / AVCC) * 1024) {
#ifdef OUT_INV
			OUT_ODR |= OUT_PIN;
#else
			OUT_ODR &= ~OUT_PIN;
#endif
			LED_ODR &= ~LED_PIN;
		}
	}
}
