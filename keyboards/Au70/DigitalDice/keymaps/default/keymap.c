
#include QMK_KEYBOARD_H

enum custom_keycodes {
  DD_CLEAR = SAFE_RANGE,
  DD_D4,
  DD_D6,
  DD_D8,
  DD_D10,
  DD_D12,
  DD_D20,
  DD_D100 
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
  LT(2,KC_KP_1), KC_KP_2, KC_KP_3, KC_KP_4, KC_KP_5,
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
  LT(2,DD_CLEAR), DD_D4, DD_D6, DD_D8, DD_D20,
  _______, DD_D10, DD_D12, DD_D100, _______
  ),

/* Adjust?
 * .--------------------------------------------.
 * |        | ClkOn  |  Macro | Bcklt+ |        |
 * |        +--------+--------+--------+        |
 * |        | ClkOff |  Dice  | Bcklt- |        |
 * '--------------------------------------------'
 */

  [2] = LAYOUT(
  _______, CK_ON, DF(0), BL_INC, _______,
  _______, CK_OFF, DF(1), BL_DEC, _______
  ),

};

void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  return true;
}

void led_set_user(uint8_t usb_led) {

}
