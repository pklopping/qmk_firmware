#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "DigitalDice.h"
#include "Segments.h"
#include "spi_master.h"

#include QMK_KEYBOARD_H

#define SEVEN_RCK C7
#define SEVEN_CLR D7
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

// Used with hacky layer switching
bool L_Held = false;
bool R_Held = false;
uint8_t layer = 0;

#define NUM_LAYERS 3
char LAYER_NAMES[NUM_LAYERS][4] = {
  "HOME",
  "ROLL",
  "SETT"
};

const uint16_t PROGMEM keymaps[NUM_LAYERS][MATRIX_ROWS][MATRIX_COLS] = {

/* Macro Pad
 * .--------------------------------------------.
 * |        |   2    |   3    |   4    |        |
 * |   1    +--------+--------+--------+   5    |
 * |        |   6    |   7    |   8    |        |
 * '--------------------------------------------'
 */

    [0] = LAYOUT(
    KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_4, KC_KP_5,
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
    DD_CLEAR, DD_D4, DD_D6, DD_D8, DD_D20,
    _______, DD_D10, DD_D12, DD_D100, _______
    ),

/* Keyboard Adjust
 * .--------------------------------------------.
 * |        | ClkOn  |  Macro | Bcklt+ |        |
 * | Accum  +--------+--------+--------+        |
 * |        | ClkOff |  Dice  | Bcklt- |        |
 * '--------------------------------------------'
 */

    [2] = LAYOUT(
    DD_ACC, CK_ON, _______, BL_INC, BL_STEP,
    _______, CK_OFF, _______, BL_DEC, _______
    ),

};

void led_set_user(uint8_t usb_led) {

}

bool HandleLayerSwitch( keyrecord_t *record) {
  uint8_t row = record->event.key.row;
  uint8_t col = record->event.key.col;

  if (record->event.pressed) {
    if (row == 0 && col == 0)
      L_Held = true;
    else if (row == 0 && col == 4)
      R_Held = true;
    else if (L_Held && R_Held && col == 2) {
      if (row == 0) {
        layer ++;
      } else {
        layer --;
      }
      // lazy min/max
      layer = layer > NUM_LAYERS ? NUM_LAYERS : layer;
      layer = layer < 0 ? 0 : layer;

      Segments__SetValueWithString(segments, LAYER_NAMES[layer]);
      layer_move(layer);
      return true;
    }
  } else {
    if (row == 0 && col == 0)
      L_Held = false;
    else if (row == 0 && col == 4)
      R_Held = false;
  }
  return false;
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

    Segments__SetValueWithString(segments, " hi ");
    UpdateDisplay();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // put your per-action keyboard code here
  // runs for every action, just before processing by the firmware
  bool handled = HandleLayerSwitch(record);
  if (handled) {
    UpdateDisplay();
    return true;
  }

  if (record->event.pressed) {
    switch (keycode) {
      case KC_KP_1:
        Segments__SetValue(segments, 1);
        break;
      case KC_KP_2:
        Segments__SetValue(segments, 2);
        break;
      case KC_KP_3:
        Segments__SetValue(segments, 3);
        break;
      case KC_KP_4:
        Segments__SetValue(segments, 4);
        break;
      case KC_KP_5:
        Segments__SetValue(segments, 5);
        break;
      case KC_KP_6:
        Segments__SetValue(segments, 6);
        break;
      case KC_KP_7:
        Segments__SetValue(segments, 7);
        break;
      case KC_KP_8:
        Segments__SetValue(segments, 8);
        break;
      case DD_CLEAR:
        Segments__SetValueWithString(segments, "CLER");
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