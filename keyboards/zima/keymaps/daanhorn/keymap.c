/* Copyright 2019 Thomas Baart
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

// Defines the keycodes used by our macros in process_record_user
// enum custom_keycodes { QMKBEST = SAFE_RANGE, QMKURL };

enum custom_keycodes {
    HS_PCB = SAFE_RANGE,
    HS_CASE
};

enum layers {
    _NUMBER = 0,
    _FUNCTION,
    _INTELLIJ,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_NUMBER] = LAYOUT(/* Base */
                 KC_7,  KC_8,  KC_9,
                 KC_4,  KC_5,  KC_6,
                 KC_1,  KC_2,  KC_3,
                 KC_0,  KC_DOT,  TG(_FUNCTION)),
    [_FUNCTION] = LAYOUT(/* Function keys */
                 KC_F7,  KC_F8,  KC_F9,
                 KC_F4,  KC_F5,  KC_F6,
                 KC_F1,  KC_F2,  KC_F3,
                 KC_F10,  MT(KC_F12, KC_F11),  TG(_INTELLIJ)),
    [_INTELLIJ] = LAYOUT(/* IntelliJ commands */
                 KC_F7,    KC_F8,       KC_F9,
                 C(KC_T),  C(KC_N),     C(S(KC_UP)),
                 C(KC_K),  C(S(KC_N)),  C(S(KC_DOWN)),
                 C(KC_F9), C(KC_F5),    TO(_NUMBER))
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HS_PCB:
            if (record->event.pressed) {
                SEND_STRING("853400");
            } else {
            }
            break;
        case HS_CASE:
            if (record->event.pressed) {
                SEND_STRING("847160");
            } else {
            }
            break;
    }
    return true;
}

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	return OLED_ROTATION_180;
}

//static void render_qmk_logo(void) {
//  static const char PROGMEM qmk_logo[] = {
//    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
//    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
///    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};

//  oled_write_P(qmk_logo, false);
//}

//void oled_task_user(void) {
//    render_qmk_logo();
//}

void oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case _NUMBER:
            oled_write_P(PSTR("Number\n"), false);
            break;
        case _FUNCTION:
            oled_write_P(PSTR("Function\n"), false);
            break;
        case _INTELLIJ:
            oled_write_P(PSTR("IntelliJ\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
}
#endif

void matrix_init_user(void) {}

void matrix_scan_user(void) {}

void led_set_user(uint8_t usb_led) {}


bool is_ctl_tab_active = false;
uint16_t ctl_tab_timer = 0;

#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
    switch (biton32(layer_state)) {
        case _INTELLIJ:
            // History scrubbing.
            if (clockwise) {
                tap_code16(C(KC_Z));
            } else {
                tap_code16(C(S(KC_Z)));
            }
            break;
        default:
            // Switch between tabs with ctrl tab.
            if (clockwise) {
                if (!is_ctl_tab_active) {
                    is_ctl_tab_active = true;
                    register_code(KC_LCTL);
                } 
                ctl_tab_timer = timer_read();
                tap_code16(KC_TAB);
            } else {
                tap_code16(S(KC_TAB));
            }
            break;
    }
}
#endif
