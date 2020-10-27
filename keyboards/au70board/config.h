#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFACD
#define PRODUCT_ID      0x0000
#define DEVICE_VER      0x0001
#define MANUFACTURER    Au70
#define PRODUCT         Au70Board
#define DESCRIPTION     A 60% Preonic Clone

/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 12

// ROWS: Top to bottom, COLS: Left to right

#define MATRIX_ROW_PINS {D4,D7,A13,A12,D0}
#define MATRIX_COL_PINS {C2,C1,D6,D5,B2,B3,B1,B0,C0,D1,C7,C6}
#define UNUSED_PINS {}

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION ROW2COL

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
// #define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
// #define LOCKING_RESYNC_ENABLE

/* Backlight configuration
 */
#define BACKLIGHT_PIN C4
#define BACKLIGHT_LEVELS 3

// #define QMK_ESC_OUTPUT D0 // usually COL
// #define QMK_ESC_INPUT D4 // usually ROW