// Copyright 2024 Ryan Neff (@JellyTitan)
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdint.h>
#include "action.h"
#include "action_layer.h"
#include "action_tapping.h"
#include "action_util.h"
#include "color.h"
#include "keyboard.h"
#include "keycodes.h"
#include "modifiers.h"
#include "oled_driver.h"
#include "quantum.h"
#include "rgb_matrix.h"
#include QMK_KEYBOARD_H

#ifdef OLED_ENABLE
    static void render_logo(void);
    static void render_right(void);
    static void render_left(void);
    static void print_base(void);
    static void print_layer(void);
    static void print_mods(void);
    static void print_adjust(void);
#endif

enum sofle_layers {
    _COLEMAK_DH,
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST,
};

enum custom_keycodes {
    KC_COLE = SAFE_RANGE,
    KC_QWER,
    KC_ADJST,
    KC_UM_A,
    KC_UM_O,
    KC_UM_U,
    KC_SS,
};

enum tapdance_keys {
    TD_LALT_RALT,
};

tap_dance_action_t tap_dance_actions[] = {
    [TD_LALT_RALT] = ACTION_TAP_DANCE_DOUBLE(KC_LALT, KC_RALT),
};

// define mod keys
#define MO_LOW MO(_LOWER)
#define MO_HIGH MO(_RAISE)
#define TD_ALT TD(TD_LALT_RALT)
#define SPC_GUI LGUI_T(KC_SPC)
#define ESC_CTL CTL_T(KC_ESC)
#define ENT_GUI LGUI_T(KC_ENT)
#define BSC_SFT RSFT_T(KC_BSPC)

// home row mods for ColemakDH
#define A_GUI LGUI_T(KC_A)
#define R_ALT LALT_T(KC_R)
#define S_CTL LCTL_T(KC_S)
#define T_SFT LSFT_T(KC_T)
#define N_SFT RSFT_T(KC_N)
#define E_CTL RCTL_T(KC_E)
#define I_ALT LALT_T(KC_I)
#define O_GUI RGUI_T(KC_O)

// home row mods for qwerty
#define A_GUI LGUI_T(KC_A)
#define S_ALT LALT_T(KC_S)
#define D_CTL LCTL_T(KC_D)
#define F_SFT LSFT_T(KC_F)
#define J_SFT RSFT_T(KC_J)
#define K_CTL RCTL_T(KC_K)
#define L_ALT LALT_T(KC_L)
#define SCLN_GUI LGUI_T(KC_SCLN)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // clang-format off
    /*
    * COLEMAK_DH
    * ,-----------------------------------------.                    ,-----------------------------------------.
    * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | Del  |
    * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
    * | Esc  |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   U  |   Y  |   ;  | Bspc |
    * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
    * | Tab  | A/GUI| R/ALT| S/CTL| T/SFT|   G  |-------.    ,-------|   M  | N/SFT| E/CTL| I/ALT| O/GUI|  '   |
    * |------+------+------+------+------+------|  MUTE |    | ADJST |------+------+------+------+------+------|
    * |CW_TOG|   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,  |   .  |   /  |      |
    * `-----------------------------------------/       /     \      \-----------------------------------------'
    *            | LGUI | LAlt | Esc  |Space | / LOWER /       \RAISE \  |Enter | Bspc | LAlt-| RGUI |
    *            |      |      | LSFT |GUI   |/       /         \       \|GUI   | RSFT | RAlt |      |
    *            `----------------------------------'           '------''---------------------------'
    */
    [_COLEMAK_DH] = LAYOUT(
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                            KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
    KC_ESC,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                            KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_BSPC,
    KC_TAB,  A_GUI,   R_ALT,   S_CTL,   T_SFT,   KC_G,                            KC_M,    N_SFT,   E_CTL,   I_ALT,   O_GUI,   KC_QUOT,
    CW_TOGG, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,    KC_MUTE,       KC_ADJST,KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH, XXXXXXX,
                      KC_LGUI, KC_LALT, ESC_CTL, SPC_GUI, MO_LOW,        MO_HIGH, ENT_GUI, BSC_SFT, TD_ALT,  KC_RGUI
    ),

    /*
    * QWERTY
    * ,-----------------------------------------.                    ,-----------------------------------------.
    * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | Del  |
    * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
    * | Esc  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | Bspc |
    * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
    * | Tab  | A/GUI| S/ALT| D/CTL| F/SFT|   G  |-------.    ,-------|   H  | J/SFT| K/CTL| L/ALT| ;/GUI|  '   |
    * |------+------+------+------+------+------|  MUTE |    | ADJST |------+------+------+------+------+------|
    * |CW_TOG|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |      |
    * `-----------------------------------------/       /     \      \-----------------------------------------'
    *            | LGUI | LAlt | Esc  |Space | / LOWER /       \RAISE \  |Enter | Bspc | LAlt-| RGUI |
    *            |      |      | LSFT |GUI   |/       /         \      \ |GUI   | RSFT | RAlt |      |
    *            `----------------------------------'           '------''---------------------------'
    */
    [_QWERTY] = LAYOUT(
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                            KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
    KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                            KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
    KC_TAB,  A_GUI,   S_ALT,   D_CTL,   F_SFT,   KC_G,                            KC_H,    J_SFT,   K_CTL,   L_ALT,   SCLN_GUI,KC_QUOT,
    CW_TOGG, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_MUTE,       KC_ADJST,KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, XXXXXXX,
                      KC_LGUI, KC_LALT, ESC_CTL, SPC_GUI, MO_LOW,        MO_HIGH, ENT_GUI, BSC_SFT, TD_ALT,  KC_RGUI
    ),

    /* LOWER
    * ,-----------------------------------------.                    ,-----------------------------------------.
    * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |                    |  F7  |  F8  |  F9  | F10  | F11  | F12  |
    * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
    * |  `   |   !  |   @  |   #  |   $  |   %  |                    |   ^  |   &  |   *  |   (  |   )  |      |
    * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
    * |      |   |  |   /  |   \  |   +  |   ?  |-------.    ,-------|   _  |   -  |   =  |   {  |   }  |      |
    * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
    * |   ß  |   ä  |   ö  |   ü  |   ~  |      |-------|    |-------|      |   <  |   >  |   [  |   ]  |      |
    * `-----------------------------------------/       /     \      \-----------------------------------------'
    *            |      |      |      |      | /       /       \ LOCK \  |      |      |      |      |
    *            |      |      |      |      |/       /         \      \ |      |      |      |      |
    *            `----------------------------------'           '------''---------------------------'
    */
    [_LOWER] = LAYOUT(
    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                           KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
    KC_GRV,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                         KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
    _______, KC_PIPE, KC_SLSH, KC_BSLS, KC_PLUS, KC_QUES,                         KC_UNDS, KC_MINS, KC_EQL,  KC_LCBR, KC_RCBR, _______,
    KC_SS,   KC_UM_A, KC_UM_O, KC_UM_U, KC_TILD, _______, _______,       _______, _______, KC_LT,   KC_GT,   KC_LBRC, KC_RBRC, _______,
                      _______, _______, _______, _______, _______,       QK_LLCK, _______, _______, _______, _______
    ),

    /* RAISE
    * ,----------------------------------------.                    ,-----------------------------------------.
    * | PrtSc| Calc |      |      |      |      |                    | Play |      |      |      |      |      |
    * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
    * | Esc  |      |      | Menu |      |      |                    | PgUp | Home |      |      |      | Ins  |
    * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
    * | Tab  | LGui | LAlt | LCtl |LShift|      |-------.    ,-------| Left |  Up  | Down | Right| Bspc | Del  |
    * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
    * | Caps | Undo |  Cut | Copy | Paste|      |-------|    |-------| PgDwn| End  |      |      |      |      |
    * `-----------------------------------------/       /     \      \-----------------------------------------'
    *            |      |      |      |      | / LOCK  /       \      \  |      |      |      |      |
    *            |      |      |      |      |/       /         \      \ |      |      |      |      |
    *            `----------------------------------'           '------''---------------------------'
    */
    [_RAISE] = LAYOUT(
    KC_PSCR, KC_CALC, _______, _______, _______, _______,                         KC_MPLY, _______, _______, _______, _______, _______,
    _______, XXXXXXX, XXXXXXX, KC_APP,  XXXXXXX, XXXXXXX,                         KC_PGUP, KC_HOME, XXXXXXX, XXXXXXX, XXXXXXX, KC_INS,
    _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                         KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_BSPC, KC_DEL,
    KC_CAPS, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), XXXXXXX, _______,       _______, KC_PGDN, KC_END,  XXXXXXX, XXXXXXX, XXXXXXX, _______,
                      _______, _______, _______, _______, QK_LLCK,       _______, _______, _______, _______, _______
    ),

    /* ADJUST
    * ,-----------------------------------------.                    ,-----------------------------------------.
    * |COLEMK|QWERTY|      |      |      |QK_BOT|                    |      |      |      |      |      |      |
    * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
    * |RGB_P |RGB_N |RGB_T |      |      |      |                    |      |      |      |      |      |      |
    * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
    * |      |      |      |      |      |      |-------.    ,-------|      |      |      |      |      |      |
    * |------+------+------+------+------+------| ADJST |    | ADJST |------+------+------+------+------+------|
    * |      |      |      |      |      |      |-------|    |-------|      |      |      |      |      |      |
    * `-----------------------------------------/       /     \      \-----------------------------------------'
    *            |      |      |      |      | /       /       \      \  |      |      |      |      |
    *            |      |      |      |      |/       /         \      \ |      |      |      |      |
    *            `----------------------------------'           '------''---------------------------'
    */
    [_ADJUST] = LAYOUT(
    KC_COLE, KC_QWER, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,                         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    RM_PREV, RM_NEXT, RM_TOGG, XXXXXXX, XXXXXXX, XXXXXXX,                         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_ADJST,      KC_ADJST,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                      _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
    ),
    // clang-format on
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_COLEMAK_DH] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGDN, KC_PGUP) },
    [_QWERTY] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGDN, KC_PGUP) },
    [_LOWER] = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______) },
    [_RAISE] = { ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(_______, _______) },
    [_ADJUST] = { ENCODER_CCW_CW(DT_DOWN, DT_UP), ENCODER_CCW_CW(KC_BRIU, KC_BRID) },
};
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_COLE:
            if (record->event.pressed) {
                oled_clear();
                set_single_default_layer(_COLEMAK_DH);
            }
            return false;
        case KC_QWER:
            if (record->event.pressed) {
                oled_clear();
                set_single_default_layer(_QWERTY);
            }
            return false;
        case KC_ADJST:
            if (record->event.pressed) {
                oled_clear();
                layer_invert(_ADJUST);
            }
            return false;
        case KC_UM_A:
            if (record->event.pressed) {
                SEND_STRING(SS_RALT("a"));
            }
            return false;
        case KC_UM_O:
            if (record->event.pressed) {
                SEND_STRING(SS_RALT("o"));
            }
            return false;
        case KC_UM_U:
            if (record->event.pressed) {
                SEND_STRING(SS_RALT("u"));
            }
            return false;
        case KC_SS:
            if (record->event.pressed) {
                SEND_STRING(SS_RALT("s"));
            }
            return false;
    }

    return true;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state);

    for (uint8_t i = led_min; i < led_max; i++) {
        switch (layer) {
            case _QWERTY:
                rgb_matrix_set_color(i, RGB_GOLD);
                break;
            case _LOWER:
                if (is_layer_locked(_LOWER)) {
                    rgb_matrix_set_color(i, 0x00, 0x2B, 0x2B); // third of RGB_TEAL
                } else {
                    rgb_matrix_set_color(i, RGB_TEAL);
                }
                break;
            case _RAISE:
                if (get_mods() & MOD_BIT_LGUI){
                    rgb_matrix_set_color(i, RGB_PURPLE);
                } else {
                    if (is_layer_locked(_RAISE)) {
                        rgb_matrix_set_color(i, 0x00, 0x55, 0x2B); // third of RGB_SPRINGGREEN
                    } else {
                        rgb_matrix_set_color(i, RGB_SPRINGGREEN);
                    }
                }
                break;
            case _ADJUST:
                rgb_matrix_set_color(i, RGB_RED);
                break;
        }
    }

    return false;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        default:
            return g_tapping_term;
    }
}

#ifndef OLED_LOGO_TIMEOUT
    #define OLED_LOGO_TIMEOUT 3000
#endif

#ifdef OLED_ENABLE
    uint16_t startup_timer;

    oled_rotation_t oled_init_user(oled_rotation_t rotation) {
        startup_timer = timer_read();
        return OLED_ROTATION_90;
    }

    bool oled_task_user(void) {
        static bool finished_logo = false;
        if (!finished_logo) {
            /* Display the logo on startup */
            if (is_keyboard_master()) {
                render_logo();
                /* Adding text below the partial screen logo requires oled_set_cursor. There are 10 chars per line. Subsequent oled_set_cursor simplifies line wraps. */
                oled_set_cursor(0, 12);
                oled_write_ln_P(PSTR("v3.5.4"), false);
                oled_set_cursor(0, 13);
                oled_write_ln_P(PSTR("03-22-2024"), false);
            } else {
                render_logo();
            }

            if (timer_elapsed(startup_timer) > OLED_LOGO_TIMEOUT) {
                finished_logo = TRUE;
                oled_clear();
            }
        } else {
            /* Display the current keyboard state */
            if (is_keyboard_master()) {
                render_left();
            } else {
                render_right();
            }
        }

        return false;
    }

    static void render_left(void) {
        print_base();
        oled_write_P(PSTR("\n"), false);
        print_layer();
        oled_write_P(PSTR("\n"), false);

        if (IS_LAYER_OFF(_ADJUST)) {
            print_mods();
        } else {
            print_adjust();
        }
    }

    static void render_right(void) {
        if (IS_LAYER_ON(_ADJUST)) {
            oled_clear();
            return;
        }

        print_base();
        oled_write_P(PSTR("\n"), false);
        print_layer();
        oled_write_P(PSTR("\n"), false);
        print_mods();

    }

    static void print_base(void) {
        oled_write_ln_P(PSTR("Base"), false);

        switch (get_highest_layer(default_layer_state)) {
            case _COLEMAK_DH:
                oled_write_P(PSTR("COLEMAK_DH"), false); //no newline as the line is full with 10 chars
                break;
            case _QWERTY:
                oled_write_ln_P(PSTR("QWERTY"), false);
                break;
            default:
                oled_write_ln_P(PSTR("Undefined"), false);
                break;
        }
    }

    static void print_layer(void) {
        oled_write_ln_P(PSTR("Layer"), false);

        switch (get_highest_layer(layer_state)) {
            case _COLEMAK_DH:
            case _QWERTY:
                oled_write_ln_P(PSTR("BASE"), false);
                break;
            case _RAISE:
                oled_write_ln_P(PSTR("RAISE"), is_layer_locked(_RAISE));
                break;
            case _LOWER:
                oled_write_ln_P(PSTR("LOWER"), is_layer_locked(_LOWER));
                break;
            case _ADJUST:
                oled_write_ln_P(PSTR("ADJUST"), true);
                break;
            default:
                oled_write_ln_P(PSTR("UNDEFINED"), false);
        }
    }

    static void print_mods(void) {
        uint8_t current_mods = get_mods();
        oled_write_ln_P(PSTR("Caps-Word"), is_caps_word_on());

        oled_write("SFT", (current_mods & MOD_BIT_LSHIFT));
        oled_write("    ", false);
        oled_write("SFT", (current_mods & MOD_BIT_RSHIFT));
        oled_write("CTL", (current_mods & MOD_BIT_LCTRL));
        oled_write("    ", false);
        oled_write("CTL", (current_mods & MOD_BIT_RCTRL));
        oled_write("ALT", (current_mods & MOD_BIT_LALT));
        oled_write("    ", false);
        oled_write("ALT", (current_mods & MOD_BIT_RALT));
        oled_write("GUI", (current_mods & MOD_BIT_LGUI));
        oled_write("    ", false);
        oled_write("GUI", (current_mods & MOD_BIT_RGUI));
    }

    static void print_adjust(void) {
            char buffer[12];
            sprintf(buffer, "DT: %d", g_tapping_term);
            oled_write_ln_P(PSTR(buffer), false);
    }

    static void render_logo(void) {
        static const char PROGMEM sofle_pico_logo[] = {
            0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0xe0, 0xf0, 0xf0, 0xf0, 0x70, 0xf0, 0xf0, 0xe0, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xf0, 0xf8, 0xfc, 0x3c, 0x3c, 0x3c, 0x7c, 0xf8, 0xf8, 0xf0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x1f, 0x3f, 0x3f, 0x3f, 0x7c, 0x78, 0xf8, 0xf8, 0xf0, 0xe0, 0x00, 0x00, 0xc0, 0xf0, 0xf8, 0xfc, 0x3c, 0x1c, 0x1e, 0x1e, 0x3c, 0xfc, 0xf8, 0xf0, 0xc0, 0x00, 0x1c, 0x1c, 0xff, 0xff, 0xff, 0xff, 0x1c, 0x1c, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xc0, 0xf0, 0xf8, 0xfc, 0xfc, 0xce, 0xce, 0xce, 0xdc, 0xfc, 0xfc, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1e, 0x1e, 0x1e, 0x3c, 0x3c, 0x3c, 0x3e, 0x1f, 0x1f, 0x0f, 0x07, 0x01, 0x00, 0x01, 0x07,
            0x0f, 0x1f, 0x1e, 0x3c, 0x3c, 0x3c, 0x1e, 0x1f, 0x0f, 0x07, 0x03, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x00, 0x00, 0x01, 0x07, 0x0f, 0x1f, 0x1f, 0x3c, 0x38, 0x38, 0x3c, 0x1c, 0x1c, 0x0c, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xfe, 0x06, 0x06, 0x06, 0x0e, 0x9e, 0xfc, 0xf8, 0x00, 0x00, 0x8e, 0x8e, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x07, 0x07, 0x07, 0x07, 0x03, 0x03, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x7c, 0xfe, 0xff, 0x83, 0x01, 0x01, 0x01, 0x83, 0x83, 0x00, 0x00, 0x7e, 0xff, 0xc7, 0x83, 0x01, 0x01, 0x01,
            0x83, 0xff, 0xff, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc3, 0xe3, 0xe0, 0xe0, 0xe0, 0xf0, 0x70, 0x70, 0x70, 0x70, 0x30, 0x33, 0x73, 0xe0, 0x60, 0xe0, 0xe0, 0xc0, 0xc1, 0x83, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x80, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0x70, 0x30, 0x70, 0x70, 0x70, 0x70, 0x70, 0xf0, 0xe0, 0xe0, 0xe0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1f, 0xff, 0xf8, 0xd0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x20, 0x40, 0xc0, 0x80, 0x00, 0x01, 0x87, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xff, 0xff, 0x03, 0x01, 0x00, 0x80, 0xc0, 0x40, 0x20, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
            0xf0, 0xf8, 0x7f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0f, 0x1c, 0x1c, 0x38, 0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x70, 0x38, 0x39, 0xdf, 0xef, 0xf7, 0x7b, 0x39, 0x1c, 0x1c, 0x0e, 0x0e, 0x06, 0x06, 0x06, 0x06, 0x06, 0x0e, 0x0e, 0x1e, 0x1c, 0x3c, 0x79, 0xfb, 0xf7, 0xef, 0x9f, 0x38, 0x30, 0x70, 0x60, 0x60, 0x60, 0x60, 0x70, 0x38, 0x3c, 0x1c, 0x0f, 0x0f, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xe0, 0xf8, 0xfc, 0x7e, 0x7f, 0xff, 0xff, 0xe3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc1, 0xff, 0x7f, 0x7f, 0x3f, 0x7c, 0xf8, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x7f, 0xff, 0x81, 0x00, 0x00, 0x00, 0x00, 0x01, 0xcf, 0xff, 0xfe, 0x7c, 0x78, 0x78, 0x38, 0x30, 0x30, 0x30, 0x30, 0x30, 0x38, 0x78, 0x78, 0x7c, 0xfe, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0f, 0x0e, 0x1e, 0x3f, 0x7f, 0xf1, 0xe0, 0xc0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xc0, 0xe1, 0xf3, 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x03, 0x03, 0x03,
            0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        oled_write_raw_P(sofle_pico_logo, sizeof(sofle_pico_logo));
    }
#endif
