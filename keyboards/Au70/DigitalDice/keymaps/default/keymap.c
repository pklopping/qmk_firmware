
#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Single 4x4 board only
 * .--------------------------------------------.
 * |   1    |   2    |   3    |   4    |   5    |
 * |--------+--------+--------+--------+--------|
 * |        |   6    |   7    |   8    |        |
 * '--------------------------------------------'
 */

  [0] = LAYOUT(
  KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_4, KC_KP_5,
  _______, KC_KP_6, KC_KP_7, KC_KP_8, _______
  ),

  // [1] = LAYOUT(
  // DD_CLEAR, DD_D4, DD_D6, DD_D8, DD_D20,
  // _______, DD_D10, DD_D12, DD_D100, _______
  // ),

  // [2] = LAYOUT(
  // KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_4, KC_KP_5,
  // _______, KC_KP_6, KC_KP_7, KC_KP_8, _______
  // ),

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
