#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <unistd.h>
#include "DigitalDice.h"
#include "Segments.h"
#include "spi_master.h"
#include "Animation.h"

#include QMK_KEYBOARD_H

#define SEVEN_RCK C7
#define SEVEN_CLR D7

Segments* segments;

uint16_t last_frame = 0;

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
bool accumulate = false;
uint16_t total_rolled = 0;

#define NUM_LAYERS 3
char LAYER_NAMES[NUM_LAYERS][4] = {
    "TYPE",
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

/* Adjust
 * .--------------------------------------------.
 * |        | ClkOn  |        | Bcklt+ |        |
 * | Accum  +--------+--------+--------+        |
 * |        | ClkOff |        | Bcklt- |        |
 * '--------------------------------------------'
 */

    [2] = LAYOUT(
        DD_ACC, CK_ON, KC_NO, BL_INC, KC_NO,
        _______, CK_OFF, KC_NO, BL_DEC, _______
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
        if (layer >= NUM_LAYERS) layer = 0;

        Segments__SetValueWithString(segments, LAYER_NAMES[layer]);
        layer_move(layer);
        return false;
    }
    if (R_Held && L_Held)
          return false; // Don't count the second press as a key
    } else {
        if (row == 0 && col == 0)
            L_Held = false;
        else if (row == 0 && col == 4)
            R_Held = false;
    }
    return true;
}

void UpdateDisplay(void) {
    spi_start(B4, false, 0, 2);
    for (int i = 0; i < 4; i++) {
        spi_write(Segments__GetByte(segments, i));
    }
    spi_stop();
    writePinHigh(SEVEN_RCK);
    writePinLow(SEVEN_RCK);
}

void Roll(uint16_t keycode) {
    Segments__SetAnimationWithAdditive(segments, rolling_animation, 32, true);
    uint16_t rolled = 0;
    switch (keycode) {
    case DD_D4:
        rolled = (rand() % 4 ) + 1;
        break;
    case DD_D6:
        rolled = (rand() % 6) + 1;
        break;
    case DD_D8:
        rolled = (rand() % 8) + 1;
        break;
    case DD_D10:
        rolled = (rand() % 10) + 1;
        break;
    case DD_D12:
        rolled = (rand() % 12) + 1;
        break;
    case DD_D20:
        rolled = (rand() % 20) + 1;
        break;
    case DD_D100:
        rolled = (rand() % 100) + 1;
        break;
    }
    if (accumulate)
        total_rolled += rolled;
    else
        total_rolled = rolled;

}

void matrix_init_user(void) {
    srand(time(0));
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

void matrix_scan_user(void) {
    // Animate the display if there are things to animate
    if (Segments__IsAnimating(segments) && timer_read() > last_frame + 25) {
        Segments__StepAnimation(segments);
        last_frame = timer_read();
    }
    UpdateDisplay();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // put your per-action keyboard code here
  // runs for every action, just before processing by the firmware
    bool should_continue = HandleLayerSwitch(record);
    if (should_continue == false) {
        UpdateDisplay();
        return false;
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
                Segments__ClearAnimation(segments);
                total_rolled = 0;
                break;
            case DD_D4:
                Roll(keycode);
                break;
            case DD_D6:
                Roll(keycode);
                break;
            case DD_D8:
                Roll(keycode);
                break;
            case DD_D10:
                Roll(keycode);
                break;
            case DD_D12:
                Roll(keycode);
                break;
            case DD_D20:
                Roll(keycode);
                break;
            case DD_D100:
                Roll(keycode);
                break;
            case CK_ON:
                Segments__SetValueWithString(segments, "BEEP");
                break;
            case CK_OFF:
                Segments__SetValueWithString(segments, "MUTE");
                break;
            case DD_ACC:
                accumulate = !accumulate;
                if (accumulate)
                    Segments__SetValueWithString(segments, "ACON");
                else
                    Segments__SetValueWithString(segments, "ACOF");
                break;
            default:
                Segments__SetValue(segments, keycode);
                break;
        }
        UpdateDisplay();
    }
    return true;
}