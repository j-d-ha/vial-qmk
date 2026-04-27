#include QMK_KEYBOARD_H

#include "dynamic_keymap.h"
#include "eeconfig.h"
#include "holykeebs.h"

enum layers {
    _BASE,
    _SYM,
    _NAV,
    _HK,
    _EXT1,
    _EXT2,
};

#ifdef VIAL_COMBO_ENABLE
#define HK_COMBO_MIGRATION_MAGIC   0x484B434DUL
#define HK_COMBO_MIGRATION_VERSION 1

typedef struct {
    uint32_t magic;
    uint8_t  version;
    uint8_t  reserved[3];
} hk_keymap_eeconfig_t;

static const vial_combo_entry_t hk_default_combos[] = {
    {{KC_C, KC_V, KC_NO, KC_NO}, LGUI(KC_V)},
    {{KC_X, KC_C, KC_NO, KC_NO}, LGUI(KC_C)},
    {{KC_X, KC_C, KC_V, KC_NO}, SGUI(KC_C)},
    {{KC_Z, KC_X, KC_NO, KC_NO}, LGUI(KC_Z)},
    {{KC_Z, KC_X, KC_C, KC_NO}, SGUI(KC_Z)},
    {{KC_Z, KC_C, KC_NO, KC_NO}, LGUI(KC_X)},
};

static void hk_reset_vial_combos(void) {
    const size_t combo_count = sizeof(hk_default_combos) / sizeof(hk_default_combos[0]);
    vial_combo_entry_t blank = {0};

    for (uint8_t i = 0; i < VIAL_COMBO_ENTRIES; ++i) {
        dynamic_keymap_set_combo(i, &blank);
    }

    for (uint8_t i = 0; i < combo_count && i < VIAL_COMBO_ENTRIES; ++i) {
        dynamic_keymap_set_combo(i, &hk_default_combos[i]);
    }
}

static bool hk_combo_entry_is_blank(const vial_combo_entry_t *entry) {
    if (entry->output != KC_NO) {
        return false;
    }

    for (uint8_t i = 0; i < ARRAY_SIZE(entry->input); ++i) {
        if (entry->input[i] != KC_NO) {
            return false;
        }
    }

    return true;
}

static bool hk_any_vial_combos_configured(void) {
    for (uint8_t i = 0; i < VIAL_COMBO_ENTRIES; ++i) {
        vial_combo_entry_t entry = {0};

        if (dynamic_keymap_get_combo(i, &entry) == 0 && !hk_combo_entry_is_blank(&entry)) {
            return true;
        }
    }

    return false;
}

static void hk_migrate_vial_combos_if_needed(void) {
    hk_keymap_eeconfig_t config = {0};

    if (eeconfig_is_kb_datablock_valid()) {
        eeconfig_read_kb_datablock(&config, 0, sizeof(config));
    }

    if (config.magic == HK_COMBO_MIGRATION_MAGIC && config.version >= HK_COMBO_MIGRATION_VERSION) {
        return;
    }

    if (!hk_any_vial_combos_configured()) {
        hk_reset_vial_combos();
    }

    config.magic = HK_COMBO_MIGRATION_MAGIC;
    config.version = HK_COMBO_MIGRATION_VERSION;
    eeconfig_update_kb_datablock(&config, 0, sizeof(config));
}
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x6_3(
        KC_ESC,  KC_Q,             KC_W,             KC_E,             KC_R,             KC_T,              KC_Y,             KC_U,             KC_I,             KC_O,             KC_P,              KC_BSPC,
        KC_TAB,  LCTL_T(KC_A),     LALT_T(KC_S),     LGUI_T(KC_D),     LSFT_T(KC_F),     KC_G,              KC_H,             RSFT_T(KC_J),     RGUI_T(KC_K),     RALT_T(KC_L),     RCTL_T(KC_SCLN),  KC_QUOT,
        KC_LSFT, KC_Z,             KC_X,             KC_C,             KC_V,             KC_B,              KC_N,             KC_M,             KC_COMM,          KC_DOT,           KC_SLSH,           KC_RSFT,
                                                      KC_LGUI,          MO(_SYM),         KC_SPC,            KC_ENT,           MO(_NAV),         KC_RALT),

    [_SYM] = LAYOUT_split_3x6_3(
        KC_TRNS, KC_TILD,          KC_PIPE,          KC_LCBR,          KC_RCBR,          KC_HASH,           KC_UNDS,          KC_7,             KC_8,             KC_9,             KC_PPLS,           KC_NO,
        KC_CIRC, LCTL_T(KC_GRV),   LALT_T(KC_BSLS),  LGUI_T(KC_LBRC),  LSFT_T(KC_RBRC),  LALT(KC_SPC),      KC_MINS,          LSFT_T(KC_4),     LGUI_T(KC_5),     LALT_T(KC_6),     LCTL_T(KC_PEQL),   KC_NO,
        KC_PERC, KC_EXLM,          KC_AMPR,          KC_LPRN,          KC_RPRN,          KC_AT,             KC_DLR,           KC_1,             KC_2,             KC_3,             KC_PAST,           KC_NO,
                                                      KC_TRNS,          KC_TRNS,         KC_NO,             KC_ENT,           MO(_HK),          KC_0),

    [_NAV] = LAYOUT_split_3x6_3(
        KC_TRNS, KC_F1,            KC_F2,            KC_F3,            KC_F4,            KC_BTN1,           KC_PGUP,          KC_LSFT,          KC_LGUI,          KC_LALT,          KC_LCTL,           KC_NO,
        KC_NO,   LCTL_T(KC_F5),    LALT_T(KC_F6),    LGUI_T(KC_F7),    LSFT_T(KC_F8),    KC_BTN2,           KC_PGDN,          KC_LEFT,          KC_DOWN,          KC_UP,            KC_RGHT,           KC_NO,
        KC_NO,   KC_F9,            KC_F10,           KC_F11,           KC_F12,           KC_BTN3,           KC_HOME,          LGUI(KC_LEFT),    LALT(KC_LEFT),    LALT(KC_RGHT),    LGUI(KC_RGHT),     KC_END,
                                                      KC_TRNS,          MO(_HK),         KC_BSPC,           KC_NO,            KC_TRNS,          KC_TRNS),

    [_HK] = LAYOUT_split_3x6_3(
        KC_TRNS, LCA(KC_UP),       LCA(KC_LEFT),     MEH(KC_DOWN),     LCA(KC_RGHT),     LCA(KC_ENT),       KC_NO,            QK_MACRO_3,       QK_MACRO_4,       QK_MACRO_5,       QK_MACRO_6,        KC_NO,
        KC_SLEP, LCA(KC_DOWN),     LCA(KC_D),        LCA(KC_F),        LCA(KC_G),        LCAG(KC_RGHT),     KC_NO,            QK_MACRO_2,       QK_MACRO_0,       QK_MACRO_1,       QK_MACRO_7,        KC_NO,
        TD(1),   MEH(KC_UP),       LCA(KC_E),        LCA(KC_R),        LCA(KC_T),        LCAG(KC_LEFT),     KC_NO,            QK_MACRO_8,       QK_MACRO_9,       KC_NO,            KC_NO,             KC_NO,
                                                      KC_TRNS,          KC_TRNS,         KC_NO,             KC_NO,            KC_TRNS,          KC_TRNS),

    [_EXT1] = LAYOUT_split_3x6_3(
        KC_TRNS, KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,           KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,           KC_TRNS,
        KC_TRNS, KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,           KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,           KC_TRNS,
        KC_TRNS, KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,           KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,           KC_TRNS,
                                                      KC_TRNS,          KC_TRNS,          KC_TRNS,           KC_TRNS,          KC_TRNS,          KC_TRNS),

    [_EXT2] = LAYOUT_split_3x6_3(
        KC_TRNS, KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,           KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,           KC_TRNS,
        KC_TRNS, KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,           KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,           KC_TRNS,
        KC_TRNS, KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,           KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,           KC_TRNS,
                                                      KC_TRNS,          KC_TRNS,          KC_TRNS,           KC_TRNS,          KC_TRNS,          KC_TRNS)
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_PGDN, KC_PGUP), ENCODER_CCW_CW(KC_RGHT, KC_LEFT)},
    [_SYM]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_PGDN, KC_PGUP), ENCODER_CCW_CW(KC_RGHT, KC_LEFT)},
    [_NAV]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_PGDN, KC_PGUP), ENCODER_CCW_CW(KC_RGHT, KC_LEFT)},
    [_HK]   = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_UP,   KC_DOWN), ENCODER_CCW_CW(KC_RGHT, KC_LEFT)},
    [_EXT1] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_PGDN, KC_PGUP), ENCODER_CCW_CW(KC_RGHT, KC_LEFT)},
    [_EXT2] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_PGDN, KC_PGUP), ENCODER_CCW_CW(KC_RGHT, KC_LEFT)},
};
#endif

void eeconfig_init_keymap(void) {
#ifdef VIAL_COMBO_ENABLE
    hk_reset_vial_combos();
#endif
}

void keyboard_post_init_keymap(void) {
#ifdef VIAL_COMBO_ENABLE
    if (is_keyboard_master()) {
        hk_migrate_vial_combos_if_needed();
    }
#endif

#ifdef VIAL_ENABLE
    vial_init();
#endif
}
