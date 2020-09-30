#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "DigitalDice.h"
#include "Segments.h"
#include "spi_master.h"

#include QMK_KEYBOARD_H

#define SEVEN_RCK C7
#define SEVEN_CLR D7
int a = 0;
Segments* segments;

enum custom_keycodes {
  DD_CLEAR = SAFE_RANGE,
  DD_D4,
  DD_D6,
  DD_D8,
  DD_D10,
  DD_D12,
  DD_D20,
  DD_D100,
  DD_ACC
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Macro Pad
 * .--------------------------------------------.
 * |        |   2    |   3    |   4    |        |
 * |   1    +--------+--------+--------+   5    |
 * |        |   6    |   7    |   8    |        |
 * '--------------------------------------------'
 */

    [0] = LAYOUT(
    LT(2,KC_KP_1), KC_KP_2, KC_KP_3, KC_KP_4, LT(3,KC_KP_5),
    _______, KC_KP_6, KC_KP_7, KC_KP_8, _______
    ),

/* Digital Dice
 * .--------------------------------------------.
 * |        |  D4    |  D6    |  D8    |        |
 * |  clr   +--------+--------+--------+  d20   |
 * |        |  D10   |  D12   |  D100  |        |
 * '--------------------------------------------'
 */

    [1] = LAYOUT(
    LT(2,DD_CLEAR), DD_D4, DD_D6, DD_D8, LT(3,DD_D20),
    _______, DD_D10, DD_D12, DD_D100, _______
    ),

/* Keyboard Adjust
 * .--------------------------------------------.
 * |        | ClkOn  |  Macro | Bcklt+ |        |
 * |        +--------+--------+--------+        |
 * |        | ClkOff |  Dice  | Bcklt- |        |
 * '--------------------------------------------'
 */

    [2] = LAYOUT(
    _______, CK_ON, DF(0), BL_INC, BL_STEP,
    _______, CK_OFF, DF(1), BL_DEC, _______
    ),

/* Dice Adjust
 * .--------------------------------------------.
 * |        |        |        |        |        |
 * | Accum  +--------+--------+--------+        |
 * |        |        |        |        |        |
 * '--------------------------------------------'
 */
    [3] = LAYOUT(
    _______, DD_ACC, _______, _______, _______,
    _______, _______, _______, _______, _______
    ),
};

void led_set_user(uint8_t usb_led) {

}

void UpdateDisplay(void) {
  spi_start(B4, false, 0, 2);
  for (int i = 0; i < 4; i++) {
    spi_write(Segments__GetByte(segments, i));
  }
  spi_stop();
  writePinHigh(SEVEN_RCK);
  writePinLow(SEVEN_RCK);
  backlight_level(8);
}

void matrix_init_user(void) {
    segments = Segments__Create();
    // put your keyboard start-up code here
    // runs once when the firmware starts up
    setPinOutput(SEVEN_RCK);
    setPinOutput(SEVEN_CLR);
    writePinLow(SEVEN_RCK);
    writePinHigh(SEVEN_CLR);
    spi_init();

    Segments__SetValue(segments, a);
    UpdateDisplay();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // put your per-action keyboard code here
  // runs for every action, just before processing by the firmware
  if (record->event.pressed) {
    switch (keycode) {
      case KC_KP_1:
      case KC_KP_2:
      case KC_KP_3:
      case KC_KP_4:
      case KC_KP_5:
      case KC_KP_6:
      case KC_KP_7:
      case KC_KP_8:
        a++;
        Segments__SetValue(segments, a);
        break;
      case DD_D4:
        Segments__SetValueWithString(segments, "  D4");
        break;
      case DD_D6:
        Segments__SetValueWithString(segments, "  D6");
        break;
      case DD_D8:
        Segments__SetValueWithString(segments, "  D8");
        break;
      case DD_D10:
        Segments__SetValueWithString(segments, " D10");
        break;
      case DD_D12:
        Segments__SetValueWithString(segments, " D12");
        break;
      case DD_D20:
        Segments__SetValueWithString(segments, " D20");
        break;
      case DD_D100:
        Segments__SetValueWithString(segments, "D100");
        break;
    case DD_ACC:
        Segments__SetValueWithString(segments, "ACC ");
        break;
      default:
        Segments__SetValue(segments, keycode);
        break;
    }
    UpdateDisplay();
  }
  return true;
}