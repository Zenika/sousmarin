/*
Copyright 2020 coliss86

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xBEAF
#define PRODUCT_ID      0x9876
#define DEVICE_VER      0x0001
#define MANUFACTURER    Zenika
#define PRODUCT         Sousmarin

/* key matrix size */
#define MATRIX_ROWS 2
#define MATRIX_COLS 3

/*
 * Keyboard Matrix Assignments
 *
*/
#define MATRIX_ROW_PINS { C6, D7 }
#define MATRIX_COL_PINS { E6, B4, B5 }
#define UNUSED_PINS

#define PIN_LED_GREEN   F6
#define PIN_LED_ORANGE  F5
#define PIN_LED_RED     F4

#define PIN_METER B6

// D4 : switch 1 position
// D0, D1 : switch 3 positions
#define DIP_SWITCH_PINS { D4, D0, D1, F7 }

/* COL2ROW, ROW2COL*/
#define DIODE_DIRECTION COL2ROW

#define TAPPING_TERM 600

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 20

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE
