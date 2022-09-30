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

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define GAUGE_INCREMENT 10

#define LED_ON  1
#define LED_OFF 0
// Macro Declarations
enum custom_keycodes {
    ORANGE = SAFE_RANGE,
    UP, DOWN, FORWARD,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    FORWARD, DOWN, UP,
    ORANGE, FR_T, FR_P
   )
};

int gauge = 50;

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

uint32_t orange_led_off(uint32_t trigger_time, void *cb_arg) {
    writePin(PIN_LED_ORANGE, LED_OFF);
    tap_code(FR_L);
    return 0;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case ORANGE:
            if (record->event.pressed) {
                led_orange();
                defer_exec(3000, orange_led_off, NULL);
            }
            break;
        case FR_P:
            gauge = 50;
            break;
        case FR_T:
            if (record->event.pressed) {
                writePin(PIN_LED_RED, LED_ON);
            } else {
                writePin(PIN_LED_RED, LED_OFF);
            }
            break;
        case FORWARD:
            if (record->event.pressed) {
                led_green();
                tap_code(KC_SPACE);
            } else {
                led_off();
            }
            break;
        case UP:
            if (record->event.pressed) {
                led_off();
                writePin(PIN_METER , LED_ON);
                gauge = MAX(gauge - GAUGE_INCREMENT, 0);
                tap_code(KC_UP);
            }
            break;
        case DOWN:
            if (record->event.pressed) {
                writePin(PIN_METER , LED_OFF);
                gauge = MIN(gauge + GAUGE_INCREMENT, 100);
                tap_code(KC_DOWN);
            }
            break;
    }
    return true;
}

int previous = 0;

uint32_t dip_fuck(uint32_t trigger_time, void *cb_arg) {
    if (previous == 1) {
        tap_code(KC_P1);
    }
    return 0;
}

bool dip_switch_update_user(uint8_t index, bool active) {
    // debug
    /*SEND_STRING("index: ");
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
                previous = 3;
            }
            break;
        case 2:
            if (active) {
                tap_code(KC_P2);
                previous = 2;
            } else {
                defer_exec(200, dip_fuck, NULL);
                previous = 1;
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

int cpt = 0;
void matrix_scan_user(void) {
    cpt++;
    if (cpt >= gauge && cpt < 100) {
      writePin(PIN_METER , LED_ON);
    } else {
      writePin(PIN_METER , LED_OFF);
    }
    if (cpt >= 100) {
      cpt = 0;
    }
//    wait_ms(10);
}
