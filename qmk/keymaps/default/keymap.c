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
    TORPILLE,
};

enum tap_dance {
    START,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    FORWARD, DOWN, UP,
    ORANGE, TORPILLE, TD(START)
   )
};

int gauge = 50;

void leds_on(void) {
    writePin(PIN_LED_GREEN,  LED_ON);
    writePin(PIN_LED_ORANGE, LED_ON);
    writePin(PIN_LED_RED,    LED_ON);
}

void leds_off(void) {
    writePin(PIN_LED_GREEN , LED_OFF);
    writePin(PIN_LED_ORANGE, LED_OFF);
    writePin(PIN_LED_RED   , LED_OFF);
}

bool orange = false;
uint32_t orange_led_off(uint32_t trigger_time, void *cb_arg) {
    if (orange) {
        writePin(PIN_LED_ORANGE, LED_OFF);
        orange = false;
    }
    return 0;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case ORANGE:
            if (record->event.pressed && !orange) {
                orange = true;
                tap_code(FR_L);
                writePin(PIN_LED_ORANGE, LED_ON);
                defer_exec(3000, orange_led_off, NULL);
            }
            break;
        case FORWARD:
            if (record->event.pressed) {
                writePin(PIN_LED_GREEN, LED_ON);
                register_code(KC_SPACE);
            } else {
                writePin(PIN_LED_GREEN, LED_OFF);
                unregister_code(KC_SPACE);
            }
            break;
        case UP:
            if (record->event.pressed) {
                writePin(PIN_METER , LED_ON);
                gauge = MAX(gauge - GAUGE_INCREMENT, 0);
                register_code(KC_UP);
            } else {
                unregister_code(KC_UP);
            }
            break;
        case DOWN:
            if (record->event.pressed) {
                writePin(PIN_METER , LED_OFF);
                gauge = MIN(gauge + GAUGE_INCREMENT, 100);
                register_code(KC_DOWN);
            } else {
                unregister_code(KC_DOWN);
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
        case 3:
            if (active) {
                tap_code(KC_S);
                leds_off();
                writePin(PIN_LED_RED, LED_ON);
                wait_ms(400);
                writePin(PIN_LED_ORANGE, LED_ON);
                wait_ms(400);
                writePin(PIN_LED_GREEN, LED_ON);
                wait_ms(400);
                leds_off();
            } else {
                tap_code(KC_R);
            }
            break;
    }
    return true;
}

void start(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        gauge = 50;
        leds_on();
        wait_ms(200);
        leds_off();
        wait_ms(200);
        leds_on();
        wait_ms(200);
        leds_off();
        wait_ms(200);
        leds_on();
        wait_ms(200);
        leds_off();
        tap_code(KC_P);
        reset_tap_dance(state);
    } else if (state->count >= 2) {
        leds_off();
        writePin(PIN_LED_ORANGE, LED_ON);
        wait_ms(150);
        writePin(PIN_LED_ORANGE, LED_OFF);
        wait_ms(150);
        writePin(PIN_LED_ORANGE, LED_ON);
        wait_ms(150);
        writePin(PIN_LED_ORANGE, LED_OFF);
        wait_ms(150);
        tap_code(KC_X);
        reset_tap_dance(state);
    }
}

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    [START] = ACTION_TAP_DANCE_FN(start)
};

void keyboard_pre_init_user(void) {
    // Set our LED pins as output
    setPinOutput(PIN_LED_GREEN);
    setPinOutput(PIN_LED_ORANGE);
    setPinOutput(PIN_LED_RED);

    setPinOutput(PIN_METER);

    leds_off();
}

int cpt = 0;
void matrix_scan_user(void) {
    cpt++;
    if (cpt >= gauge && cpt < 100) {
        writePin(PIN_METER, LED_ON);
    } else {
        writePin(PIN_METER, LED_OFF);
    }
    if (cpt >= 100) {
      cpt = 0;
    }
//    wait_ms(10);
}
