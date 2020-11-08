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

typedef struct {
    uint16_t keycode;
    keyrecord_t *record;
    uint16_t pressed_time;
    bool is_pressed;
} FakeLayerTapKey;

// Used with hacky layer switching
uint16_t L_Pressed_Time = false;
bool L_Pressed = false;
uint16_t L_Keycode = 0;
uint16_t R_Pressed_Time = false;
bool R_Pressed = false;
uint16_t R_Keycode = 0;
bool L_and_R_were_pressed = false;
const uint16_t HOLD_TIME = 250;
uint8_t layer = 0;

// Digital dice bookkeeping
bool accumulate = false;
bool last_is_animating = false;
uint16_t total_rolled = 0;

#define NUM_LAYERS 3
char LAYER_NAMES[NUM_LAYERS][4] = {
    "ROLL",
    "TYPE",
    "SETT"
};

const uint16_t PROGMEM keymaps[NUM_LAYERS][MATRIX_ROWS][MATRIX_COLS] = {

/* Digital Dice
 * .--------------------------------------------.
 * |        |  D4    |  D6    |  D8    |        |
 * |  clr   +--------+--------+--------+  d20   |
 * |        |  D10   |  D12   |  D100  |        |
 * '--------------------------------------------'
 */

    [0] = LAYOUT(
        DD_CLEAR, DD_D4, DD_D6, DD_D8, DD_D20,
        _______, DD_D10, DD_D12, DD_D100, _______
    ),

/* Macro Pad
 * .--------------------------------------------.
 * |        |   2    |   3    |   4    |        |
 * |   1    +--------+--------+--------+   5    |
 * |        |   6    |   7    |   8    |        |
 * '--------------------------------------------'
 */

    [1] = LAYOUT(
        KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_4, KC_KP_5,
        _______, KC_KP_6, KC_KP_7, KC_KP_8, _______
    ),

/* Adjust
 * .--------------------------------------------.
 * |        | ClkOn  | Accum  | Bcklt+ |        |
 * |        +--------+--------+--------+        |
 * |        | ClkOff |        | Bcklt- |        |
 * '--------------------------------------------'
 */

    [2] = LAYOUT(
        _______, CK_ON, DD_ACC, BL_INC, KC_NO,
        _______, CK_OFF, KC_NO, BL_DEC, _______
    ),

};

bool Roll(uint16_t keycode);

// HandleLayerSwitch
// Return true if the keypress was handled
bool HandleLayerSwitch(uint16_t keycode, keyrecord_t *record) {
    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;
    uint16_t now = timer_read(); // caching because I assume this is expensive
    if (record->event.pressed)
    {
        if (row == 0 && col == 0)
        {
            L_Pressed = true;
            L_Pressed_Time = now;
            L_Keycode = keycode;
            return true;
        }
        else if (row == 0 && col == 4)
        {
            R_Pressed = true;
            R_Pressed_Time = now;
            R_Keycode = keycode;
            return true;
        }
        else if (L_Pressed && R_Pressed)
        {
            L_and_R_were_pressed = true;
            if (col == 2) // Use the middle column to jump layers
            {
                if (row == 0)
                {
                    layer ++;
                }
                else
                {
                    layer --;
                }

                // lazy min/max
                if (layer == 255) // Did it overflow?
                    layer = NUM_LAYERS - 1;
                if (layer >= NUM_LAYERS) {
                    layer = 0;
                }

                Segments__SetValueWithString(segments, LAYER_NAMES[layer]);
                layer_move(layer);
                return true;
            }
        }
    }
    else // record->event.pressed == false
    {
        if (row == 0 && col == 0) {
            L_Pressed = false;
            // Treat it like a regular keypress if it wasn't held long enough
            if (L_Pressed_Time + HOLD_TIME > now || L_and_R_were_pressed == false) {
                bool is_dice = Roll(keycode);
                if (is_dice == false)
                    tap_code(L_Keycode);
                if (R_Pressed == false)
                    L_and_R_were_pressed = false;
                return false;
            }
        }
        else if (row == 0 && col == 4) {
            R_Pressed = false;
            if (R_Pressed_Time + HOLD_TIME > now || L_and_R_were_pressed == false) {
                bool is_dice = Roll(keycode);
                if (is_dice == false)
                    tap_code(R_Keycode);
                if (L_Pressed == false)
                    L_and_R_were_pressed = false;
                return false;
            }
        }
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
}

bool Roll(uint16_t keycode) {
    uint16_t rolled = 0;
    bool identified = true;
    bool was_roll = true;
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
        case DD_CLEAR:
            was_roll = false;
            Segments__Clear(segments);
            total_rolled = 0;
            break;
        // case DD_ACC:
        //     was_roll = false;
        //     accumulate = !accumulate;
        //     if (accumulate)
        //         Segments__SetValueWithString(segments, "ACON");
        //     else
        //         Segments__SetValueWithString(segments, "ACOF");
        default:
            identified = false;
            was_roll = false;

    }
    if (accumulate)
        total_rolled += rolled;
    else
        total_rolled = rolled;

    if (identified) {
        if (was_roll) {
            Segments__SetAnimationWithAdditive(segments, rolling_animation, 32, true);
        }
        UpdateDisplay();
    }
    return identified;
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
    bool curr_is_animating = Segments__IsAnimating(segments);
    if (curr_is_animating == false)
        last_frame = timer_read();
    if (curr_is_animating && timer_read() > last_frame + 15) {
        Segments__StepAnimation(segments);
        last_frame = timer_read();
        UpdateDisplay();
    }
    // If the animation just stopped, show the roll (need to generalize this to work with more than rolls)
    if (last_is_animating && curr_is_animating == false) {
        Segments__SetValue(segments, total_rolled);
        UpdateDisplay();
    }

    // Remember the current state
    last_is_animating = curr_is_animating;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // put your per-action keyboard code here
  // runs for every action, just before processing by the firmware
    bool press_handled = HandleLayerSwitch(keycode, record);
    if (press_handled == true) {
        UpdateDisplay();
        return false;
    }
    bool display_needs_update = true;
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
            case DD_D4:
            case DD_D6:
            case DD_D8:
            case DD_D10:
            case DD_D12:
            case DD_D20:
            case DD_D100:
                Roll(keycode);
                display_needs_update = false;
                break;
            case DD_ACC:
                accumulate = !accumulate;
                if (accumulate)
                    Segments__SetValueWithString(segments, "ACON");
                else
                    Segments__SetValueWithString(segments, "ACOF");
                break;
            case CK_ON:
                Segments__SetValueWithString(segments, "BEEP");
                break;
            case CK_OFF:
                Segments__SetValueWithString(segments, "MUTE");
                break;
            default:
                Segments__SetValue(segments, keycode);
                break;
        }
        if (display_needs_update)
            UpdateDisplay();
    }
    return true;
}