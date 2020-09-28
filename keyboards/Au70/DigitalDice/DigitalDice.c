#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "DigitalDice.h"
#include "Segments.h"
#include "spi_master.h"

#define SEVEN_RCK C7
#define SEVEN_CLR D7
int a = 0;
Segments* segments;

void UpdateDisplay(void) {
	spi_start(B4, false, 0, 2);
	for (int i = 0; i < 4; i++) {
		spi_write(Segments__GetByte(segments, i));
	}
	a++;
	spi_stop();
	writePinHigh(SEVEN_RCK);
	writePinLow(SEVEN_RCK);
	backlight_level(8);
}

void matrix_init_kb(void) {
	segments = Segments__Create();
	// put your keyboard start-up code here
	// runs once when the firmware starts up
	setPinOutput(SEVEN_RCK);
	setPinOutput(SEVEN_CLR);
	writePinLow(SEVEN_RCK);
	writePinHigh(SEVEN_CLR);
	matrix_init_user();
    spi_init();


	Segments__SetValue(segments, a);
    UpdateDisplay();
}

void matrix_scan_kb(void) {
	// put your looping keyboard code here
	// runs every cycle (a lot)

	matrix_scan_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
	// put your per-action keyboard code here
	// runs for every action, just before processing by the firmware
	if (record->event.pressed) {
		if (keycode == KC_KP_1)
			Segments__SetValueWithString(segments, "Zero");
		else
			Segments__SetValue(segments, a);
		UpdateDisplay();
	}

	return process_record_user(keycode, record);
}

void led_set_kb(uint8_t usb_led) {
	// put your keyboard LED indicator (ex: Caps Lock LED) toggling code here

	led_set_user(usb_led);
}
