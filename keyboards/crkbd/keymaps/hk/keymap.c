#include QMK_KEYBOARD_H

#include "holykeebs.h"

enum layers {
    _BASE,
    _SYM,
    _NAV,
    _HK,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x6_3(
        KC_ESC,  KC_Q,           KC_W,           KC_E,           KC_R,           KC_T,         KC_Y,    KC_U,           KC_I,          KC_O,           KC_P,            KC_BSPC,
        KC_TAB,  LCTL_T(KC_A),   LALT_T(KC_S),   LGUI_T(KC_D),   LSFT_T(KC_F),   KC_G,         KC_H,    RSFT_T(KC_J),   RGUI_T(KC_K),  RALT_T(KC_L),   RCTL_T(KC_SCLN), KC_QUOT,
        KC_LSFT, KC_Z,           KC_X,           KC_C,           KC_V,           KC_B,         KC_N,    KC_M,           KC_COMM,       KC_DOT,         KC_SLSH,         KC_RSFT,
                                                  KC_LGUI,        MO(_SYM),       KC_SPC,       KC_ENT,  MO(_NAV),       KC_RALT),

    [_SYM] = LAYOUT_split_3x6_3(
        KC_TRNS, KC_TILD,        KC_PIPE,        KC_LCBR,        KC_RCBR,        KC_HASH,      KC_UNDS, KC_7,           KC_8,          KC_9,           KC_PPLS,         KC_TRNS,
        KC_CIRC, LCTL_T(KC_GRV), LALT_T(KC_BSLS),LGUI_T(KC_LBRC),LSFT_T(KC_RBRC),LALT(KC_SPC), KC_MINS, LSFT_T(KC_4),   LGUI_T(KC_5),  LALT_T(KC_6),   LCTL_T(KC_PEQL), KC_NO,
        KC_PERC, KC_EXLM,        KC_AMPR,        KC_LPRN,        KC_RPRN,        KC_AT,        KC_DLR,  KC_1,           KC_2,          KC_3,           KC_PAST,         KC_TRNS,
                                                  KC_TRNS,        KC_TRNS,        KC_NO,        KC_ENT,  MO(_HK),        KC_0),

    [_NAV] = LAYOUT_split_3x6_3(
        KC_F1,   KC_F2,          KC_F3,          KC_F4,          KC_F5,          KC_F6,        KC_PGUP, KC_LSFT,        KC_LGUI,       KC_LALT,        KC_LCTL,         KC_NO,
        KC_F7,   LCTL_T(KC_F8),  LALT_T(KC_F9),  LGUI_T(KC_F10), LSFT_T(KC_F11), KC_F12,       KC_PGDN, KC_LEFT,        KC_DOWN,       KC_UP,          KC_RGHT,         KC_NO,
        KC_NO,   KC_NO,          KC_NO,          KC_BTN1,        KC_BTN2,        KC_BTN3,      KC_HOME, LGUI(KC_LEFT),  LALT(KC_LEFT), LALT(KC_RGHT),  LGUI(KC_RGHT),   KC_END,
                                                  KC_LGUI,        MO(_HK),        KC_BSPC,      KC_NO,   KC_TRNS,        KC_RALT),

    [_HK] = LAYOUT_split_3x6_3(
        HK_DUMP, HK_SAVE, HK_RESET, HK_P_SET_D,  HK_P_SET_S,  HK_P_SET_BUF,  HK_S_MODE_T, HK_D_MODE_T, HK_C_SCROLL, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, MS_BTN1, MS_BTN2,  MS_BTN3,     MS_BTN4,     MS_BTN5,       XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, MS_UP,   MS_DOWN,  MS_LEFT,     MS_RGHT,     XXXXXXX,       XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX,
                                    _______,     _______,     _______,       _______,     _______,     _______)
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_PGDN, KC_PGUP), ENCODER_CCW_CW(KC_RGHT, KC_LEFT)},
    [_SYM]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_PGDN, KC_PGUP), ENCODER_CCW_CW(KC_RGHT, KC_LEFT)},
    [_NAV]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_PGDN, KC_PGUP), ENCODER_CCW_CW(KC_RGHT, KC_LEFT)},
    [_HK]   = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_UP,   KC_DOWN), ENCODER_CCW_CW(KC_RGHT, KC_LEFT)},
};
#endif
