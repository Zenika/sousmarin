/* Copyright 2020 coliss86
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "keymap_french.h"
#include "sendstring_french.h"

#define LED_ON  1
#define LED_OFF 0

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    FR_A, FR_B, FR_C,
    FR_L, FR_T, FR_P
   )
};

void led_green(void) {
    writePin(PIN_LED_GREEN , LED_ON);
    writePin(PIN_LED_ORANGE, LED_OFF);
    writePin(PIN_LED_RED   , LED_OFF);
}

void led_orange(void) {
    writePin(PIN_LED_GREEN , LED_OFF);
    writePin(PIN_LED_ORANGE, LED_ON);
    writePin(PIN_LED_RED   , LED_OFF);
}

void led_red(void) {
    writePin(PIN_LED_GREEN , LED_OFF);
    writePin(PIN_LED_ORANGE, LED_OFF);
    writePin(PIN_LED_RED   , LED_ON);
}

void led_off(void) {
    writePin(PIN_LED_GREEN , LED_OFF);
    writePin(PIN_LED_ORANGE, LED_OFF);
    writePin(PIN_LED_RED   , LED_OFF);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case FR_L:
            led_orange();
            break;
        case FR_T:
            led_red();
            break;
        case FR_A:
            if (record->event.pressed) {
                led_green();
            } else {
                led_off();
            }
            break;
        case FR_C:
            led_off();
            writePin(PIN_METER , LED_ON);
            break;
        case FR_B:
            writePin(PIN_METER , LED_OFF);
            break;
    }
    return true;
}

bool dip_switch_update_user(uint8_t index, bool active) {
    // debug
/*    SEND_STRING("index: ");
    char characterToSend[2];
    characterToSend[0] = '0' + index;
    characterToSend[1] = '\0';
    send_string(characterToSend);
    SEND_STRING(", active: ");
    send_string(active ? "1" : "0");
    SEND_STRING(SS_TAP(X_ENTER));*/

    switch (index) {
        case 0:
            if (active) {
                tap_code(KC_LEFT);
            } else {
                tap_code(KC_RIGHT);
            }
            break;
        case 1:
            if (active) {
                tap_code(KC_P3);
            }
            break;
        case 2:
            if (active) {
                tap_code(KC_P2);
            } else {
                tap_code(KC_P1);
            }
            break;
    }
    return true;
}

void keyboard_pre_init_user(void) {
    // Set our LED pins as output
    setPinOutput(PIN_LED_GREEN);
    setPinOutput(PIN_LED_ORANGE);
    setPinOutput(PIN_LED_RED);

    setPinOutput(PIN_METER);

    led_off();
}
