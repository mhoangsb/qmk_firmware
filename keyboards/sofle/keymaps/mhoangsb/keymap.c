// Copyright 2019-2022 Manna Harbour
// Copyright 2026 mhoang
// SPDX-License-Identifier: GPL-2.0-or-later
// Adapted from this Miryoku QMK checkout: users/manna-harbour_miryoku/.
// Upstream: https://github.com/manna-harbour/miryoku
// All keymap data and behavior are local to this directory. See readme.md.

#include QMK_KEYBOARD_H

// 1. Layer numbers. Keep BASE first. LT() can address only layers 0..15.
enum layers {
    BASE,
    EXTRA,
    TAP,
    BUTTON,
    NAV,
    MOUSE,
    MEDIA,
    NUM,
    SYM,
    FUN,
};

// 2. Tap Dance indices are separate from layer numbers.
// TD(TD_BASE), etc. switch the default layer after exactly two taps.
enum tap_dances {
    TD_BOOT,
    TD_BASE,
    TD_EXTRA,
    TD_TAP,
    TD_BUTTON,
    TD_NAV,
    TD_MOUSE,
    TD_MEDIA,
    TD_NUM,
    TD_SYM,
    TD_FUN,
};

// 3. Clipboard shortcuts. Select the operating system in config.h.
#if MHOANGSB_CLIPBOARD == 1 // Windows / conventional Ctrl shortcuts on Linux
#    define CLIP_REDO  C(KC_Y)
#    define CLIP_PASTE C(KC_V)
#    define CLIP_COPY  C(KC_C)
#    define CLIP_CUT   C(KC_X)
#    define CLIP_UNDO  C(KC_Z)
#elif MHOANGSB_CLIPBOARD == 2 // macOS
#    define CLIP_REDO  S(G(KC_Z))
#    define CLIP_PASTE G(KC_V)
#    define CLIP_COPY  G(KC_C)
#    define CLIP_CUT   G(KC_X)
#    define CLIP_UNDO  G(KC_Z)
#elif MHOANGSB_CLIPBOARD == 0 // Original Miryoku default
#    define CLIP_REDO  KC_AGIN
#    define CLIP_PASTE S(KC_INS)
#    define CLIP_COPY  C(KC_INS)
#    define CLIP_CUT   S(KC_DEL)
#    define CLIP_UNDO  KC_UNDO
#else
#    error "MHOANGSB_CLIPBOARD must be 0, 1, or 2"
#endif

// 4. Current QMK has separate RGB Matrix and RGB Light keycodes.
// These five aliases preserve the original MEDIA layer's RGB controls.
// With neither lighting feature enabled, UG_* keycodes have no lighting effect.
#ifdef RGB_MATRIX_ENABLE
#    define RGB_TOGGLE RM_TOGG
#    define RGB_NEXT   RM_NEXT
#    define RGB_HUE_UP RM_HUEU
#    define RGB_SAT_UP RM_SATU
#    define RGB_VAL_UP RM_VALU
#else
#    define RGB_TOGGLE UG_TOGG
#    define RGB_NEXT   UG_NEXT
#    define RGB_HUE_UP UG_HUEU
#    define RGB_SAT_UP UG_SATU
#    define RGB_VAL_UP UG_VALU
#endif

// 5. Complete Sofle LAYOUT arrays: 60 positions, including encoder buttons.
// No LAYOUT_miryoku wrapper, generated layer macros, or shared userspace.
// KC_NO blocks a position. KC_TRNS inherits from lower active/default layers.
// The 24 extra positions add numbers, editing keys, direct modifiers, and audio.
// Functional layers inherit these extras, with NAV/MEDIA button and FUN row overrides.
// Intentional KC_NO entries inside the Miryoku core remain blocked.
//
// Argument order, viewed from above (left/right labels are physical positions):
// L00 L01 L02 L03 L04 L05                    R00 R01 R02 R03 R04 R05
// L10 L11 L12 L13 L14 L15                    R10 R11 R12 R13 R14 R15
// L20 L21 L22 L23 L24 L25                    R20 R21 R22 R23 R24 R25
// L30 L31 L32 L33 L34 L35 [LENC]      [RENC] R30 R31 R32 R33 R34 R35
//         L40 L41 L42 L43 L44          R41 R42 R43 R44 R45
// Left active thumbs: L42 Esc, L43 Space, L44 Tab.
// Right active thumbs: R41 Enter, R42 Backspace, R43 Delete.

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // BASE: QWERTY. Home-row modifiers, AltGr on X/dot, Button on Z/slash.
    [BASE] = LAYOUT(
        KC_GRV,   KC_1,             KC_2,          KC_3,          KC_4,          KC_5,                        KC_6,  KC_7,          KC_8,          KC_9,            KC_0,                KC_EQL,
        KC_ESC,   KC_Q,             KC_W,          KC_E,          KC_R,          KC_T,                        KC_Y,  KC_U,          KC_I,          KC_O,            KC_P,                KC_BSPC,
        KC_TAB,   LGUI_T(KC_A),     LALT_T(KC_S),  LCTL_T(KC_D),  LSFT_T(KC_F),  KC_G,                        KC_H,  LSFT_T(KC_J),  LCTL_T(KC_K),  LALT_T(KC_L),    LGUI_T(KC_QUOT),     KC_BSLS,
        KC_LSFT,  LT(BUTTON,KC_Z),  ALGR_T(KC_X),  KC_C,          KC_V,          KC_B,    KC_MUTE, KC_MPLY,  KC_N,  KC_M,          KC_COMM,       ALGR_T(KC_DOT),  LT(BUTTON,KC_SLSH),  KC_ENT,
            KC_LALT, KC_LCTL, LT(MEDIA,KC_ESC), LT(NAV,KC_SPC), LT(MOUSE,KC_TAB),    LT(SYM,KC_ENT), LT(NUM,KC_BSPC), LT(FUN,KC_DEL), KC_LSFT, KC_LGUI
    ),

    // EXTRA: Colemak DH with the same modifier and thumb-layer behavior as BASE.
    [EXTRA] = LAYOUT(
        KC_GRV,   KC_1,             KC_2,          KC_3,          KC_4,          KC_5,                        KC_6,  KC_7,          KC_8,          KC_9,            KC_0,                KC_EQL,
        KC_ESC,   KC_Q,             KC_W,          KC_F,          KC_P,          KC_B,                        KC_J,  KC_L,          KC_U,          KC_Y,            KC_QUOT,             KC_BSPC,
        KC_TAB,   LGUI_T(KC_A),     LALT_T(KC_R),  LCTL_T(KC_S),  LSFT_T(KC_T),  KC_G,                        KC_M,  LSFT_T(KC_N),  LCTL_T(KC_E),  LALT_T(KC_I),    LGUI_T(KC_O),        KC_BSLS,
        KC_LSFT,  LT(BUTTON,KC_Z),  ALGR_T(KC_X),  KC_C,          KC_D,          KC_V,    KC_MUTE, KC_MPLY,  KC_K,  KC_H,          KC_COMM,       ALGR_T(KC_DOT),  LT(BUTTON,KC_SLSH),  KC_ENT,
            KC_LALT, KC_LCTL, LT(MEDIA,KC_ESC), LT(NAV,KC_SPC), LT(MOUSE,KC_TAB),    LT(SYM,KC_ENT), LT(NUM,KC_BSPC), LT(FUN,KC_DEL), KC_LSFT, KC_LGUI
    ),

    // TAP: QWERTY with plain tap keys. Power cycle to return to BASE.
    [TAP] = LAYOUT(
        KC_GRV,   KC_1,  KC_2,  KC_3,  KC_4,  KC_5,                        KC_6,  KC_7,  KC_8,     KC_9,    KC_0,     KC_EQL,
        KC_ESC,   KC_Q,  KC_W,  KC_E,  KC_R,  KC_T,                        KC_Y,  KC_U,  KC_I,     KC_O,    KC_P,     KC_BSPC,
        KC_TAB,   KC_A,  KC_S,  KC_D,  KC_F,  KC_G,                        KC_H,  KC_J,  KC_K,     KC_L,    KC_QUOT,  KC_BSLS,
        KC_LSFT,  KC_Z,  KC_X,  KC_C,  KC_V,  KC_B,    KC_MUTE, KC_MPLY,  KC_N,  KC_M,  KC_COMM,  KC_DOT,  KC_SLSH,  KC_ENT,
            KC_LALT, KC_LCTL, KC_ESC, KC_SPC, KC_TAB,    KC_ENT, KC_BSPC, KC_DEL, KC_LSFT, KC_LGUI
    ),

    // BUTTON: Mouse buttons on thumbs; clipboard shortcuts and plain modifiers on fingers.
    [BUTTON] = LAYOUT(
        KC_TRNS,  KC_TRNS,    KC_TRNS,   KC_TRNS,    KC_TRNS,     KC_TRNS,                          KC_TRNS,    KC_TRNS,     KC_TRNS,    KC_TRNS,   KC_TRNS,    KC_TRNS,
        KC_TRNS,  CLIP_UNDO,  CLIP_CUT,  CLIP_COPY,  CLIP_PASTE,  CLIP_REDO,                        CLIP_REDO,  CLIP_PASTE,  CLIP_COPY,  CLIP_CUT,  CLIP_UNDO,  KC_TRNS,
        KC_TRNS,  KC_LGUI,    KC_LALT,   KC_LCTL,    KC_LSFT,     KC_NO,                            KC_NO,      KC_LSFT,     KC_LCTL,    KC_LALT,   KC_LGUI,    KC_TRNS,
        KC_TRNS,  CLIP_UNDO,  CLIP_CUT,  CLIP_COPY,  CLIP_PASTE,  CLIP_REDO,    KC_TRNS, KC_TRNS,  CLIP_REDO,  CLIP_PASTE,  CLIP_COPY,  CLIP_CUT,  CLIP_UNDO,  KC_TRNS,
            KC_TRNS, KC_TRNS, MS_BTN3, MS_BTN1, MS_BTN2,    MS_BTN2, MS_BTN1, MS_BTN3, KC_TRNS, KC_TRNS
    ),

    // NAV: Hold left Space. VI arrows on QWERTY H/J/K/L; left hand: modifiers/selectors.
    [NAV] = LAYOUT(
        KC_TRNS,  KC_TRNS,      KC_TRNS,     KC_TRNS,       KC_TRNS,      KC_TRNS,                        KC_TRNS,    KC_TRNS,     KC_TRNS,    KC_TRNS,   KC_TRNS,    KC_TRNS,
        KC_TRNS,  TD(TD_BOOT),  TD(TD_TAP),  TD(TD_EXTRA),  TD(TD_BASE),  KC_NO,                          CLIP_REDO,  CLIP_PASTE,  CLIP_COPY,  CLIP_CUT,  CLIP_UNDO,  KC_TRNS,
        KC_TRNS,  KC_LGUI,      KC_LALT,     KC_LCTL,       KC_LSFT,      KC_NO,                          KC_LEFT,    KC_DOWN,     KC_UP,      KC_RGHT,   CW_TOGG,    KC_TRNS,
        KC_TRNS,  KC_NO,        KC_ALGR,     TD(TD_NUM),    TD(TD_NAV),   KC_NO,      C(KC_O), C(KC_I),  KC_HOME,    KC_PGDN,     KC_PGUP,    KC_END,    KC_INS,     KC_TRNS,
            KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO,    KC_ENT, KC_BSPC, KC_DEL, KC_TRNS, KC_TRNS
    ),

    // MOUSE: Hold left Tab. VI pointer/wheel arrangement; left hand: modifiers/selectors.
    [MOUSE] = LAYOUT(
        KC_TRNS,  KC_TRNS,      KC_TRNS,     KC_TRNS,       KC_TRNS,       KC_TRNS,                        KC_TRNS,    KC_TRNS,     KC_TRNS,    KC_TRNS,   KC_TRNS,    KC_TRNS,
        KC_TRNS,  TD(TD_BOOT),  TD(TD_TAP),  TD(TD_EXTRA),  TD(TD_BASE),   KC_NO,                          CLIP_REDO,  CLIP_PASTE,  CLIP_COPY,  CLIP_CUT,  CLIP_UNDO,  KC_TRNS,
        KC_TRNS,  KC_LGUI,      KC_LALT,     KC_LCTL,       KC_LSFT,       KC_NO,                          MS_LEFT,    MS_DOWN,     MS_UP,      MS_RGHT,   KC_NO,      KC_TRNS,
        KC_TRNS,  KC_NO,        KC_ALGR,     TD(TD_SYM),    TD(TD_MOUSE),  KC_NO,      KC_TRNS, KC_TRNS,  MS_WHLL,    MS_WHLD,     MS_WHLU,    MS_WHLR,   KC_NO,      KC_TRNS,
            KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO,    MS_BTN2, MS_BTN1, MS_BTN3, KC_TRNS, KC_TRNS
    ),

    // MEDIA: Hold left Escape. VI RGB/media/output arrangement; left hand: modifiers/selectors.
    [MEDIA] = LAYOUT(
        KC_TRNS,  KC_TRNS,      KC_TRNS,     KC_TRNS,       KC_TRNS,       KC_TRNS,                        KC_TRNS,   KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,
        KC_TRNS,  TD(TD_BOOT),  TD(TD_TAP),  TD(TD_EXTRA),  TD(TD_BASE),   KC_NO,                          RGB_NEXT,  RGB_HUE_UP,  RGB_SAT_UP,  RGB_VAL_UP,  RGB_TOGGLE,  KC_TRNS,
        KC_TRNS,  KC_LGUI,      KC_LALT,     KC_LCTL,       KC_LSFT,       KC_NO,                          KC_MPRV,   KC_VOLD,     KC_VOLU,     KC_MNXT,     KC_NO,       KC_TRNS,
        KC_TRNS,  KC_NO,        KC_ALGR,     TD(TD_FUN),    TD(TD_MEDIA),  KC_NO,      KC_VOLD, KC_VOLU,  KC_NO,     KC_NO,       KC_NO,       KC_NO,       OU_AUTO,     KC_TRNS,
            KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO,    KC_MSTP, KC_MPLY, KC_MUTE, KC_TRNS, KC_TRNS
    ),

    // NUM: Hold right Backspace. Left hand: numbers/punctuation; right hand: modifiers/selectors.
    [NUM] = LAYOUT(
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,                        KC_TRNS,  KC_TRNS,      KC_TRNS,       KC_TRNS,     KC_TRNS,      KC_TRNS,
        KC_TRNS,  KC_LBRC,  KC_7,     KC_8,     KC_9,     KC_RBRC,                        KC_NO,    TD(TD_BASE),  TD(TD_EXTRA),  TD(TD_TAP),  TD(TD_BOOT),  KC_TRNS,
        KC_TRNS,  KC_SCLN,  KC_4,     KC_5,     KC_6,     KC_EQL,                         KC_NO,    KC_LSFT,      KC_LCTL,       KC_LALT,     KC_LGUI,      KC_TRNS,
        KC_TRNS,  KC_GRV,   KC_1,     KC_2,     KC_3,     KC_BSLS,    KC_TRNS, KC_TRNS,  KC_NO,    TD(TD_NUM),   TD(TD_NAV),    KC_ALGR,     KC_NO,        KC_TRNS,
            KC_TRNS, KC_TRNS, KC_DOT, KC_0, KC_MINS,    KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS
    ),

    // SYM: Hold right Enter. Left hand: shifted number/punctuation symbols.
    [SYM] = LAYOUT(
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,                        KC_TRNS,  KC_TRNS,      KC_TRNS,       KC_TRNS,     KC_TRNS,      KC_TRNS,
        KC_TRNS,  KC_LCBR,  KC_AMPR,  KC_ASTR,  KC_LPRN,  KC_RCBR,                        KC_NO,    TD(TD_BASE),  TD(TD_EXTRA),  TD(TD_TAP),  TD(TD_BOOT),  KC_TRNS,
        KC_TRNS,  KC_COLN,  KC_DLR,   KC_PERC,  KC_CIRC,  KC_PLUS,                        KC_NO,    KC_LSFT,      KC_LCTL,       KC_LALT,     KC_LGUI,      KC_TRNS,
        KC_TRNS,  KC_TILD,  KC_EXLM,  KC_AT,    KC_HASH,  KC_PIPE,    KC_TRNS, KC_TRNS,  KC_NO,    TD(TD_SYM),   TD(TD_MOUSE),  KC_ALGR,     KC_NO,        KC_TRNS,
            KC_TRNS, KC_TRNS, KC_LPRN, KC_RPRN, KC_UNDS,    KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS
    ),

    // FUN: Hold right Delete. Left hand: F1-F12 and system keys.
    [FUN] = LAYOUT(
        KC_F1,    KC_F2,   KC_F3,  KC_F4,  KC_F5,  KC_F6,                          KC_F7,  KC_F8,        KC_F9,         KC_F10,      KC_F11,       KC_F12,
        KC_TRNS,  KC_F12,  KC_F7,  KC_F8,  KC_F9,  KC_PSCR,                        KC_NO,  TD(TD_BASE),  TD(TD_EXTRA),  TD(TD_TAP),  TD(TD_BOOT),  KC_TRNS,
        KC_TRNS,  KC_F11,  KC_F4,  KC_F5,  KC_F6,  KC_SCRL,                        KC_NO,  KC_LSFT,      KC_LCTL,       KC_LALT,     KC_LGUI,      KC_TRNS,
        KC_TRNS,  KC_F10,  KC_F1,  KC_F2,  KC_F3,  KC_PAUS,    KC_TRNS, KC_TRNS,  KC_NO,  TD(TD_FUN),   TD(TD_MEDIA),  KC_ALGR,     KC_NO,        KC_TRNS,
            KC_TRNS, KC_TRNS, KC_APP, KC_SPC, KC_TAB,    KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS
    ),

};

// 6. Double-tap guard: one tap does nothing; exactly two taps run the action.
// Default-layer changes are session-only (no EEPROM writes).
// No layer_clear(): releasing the thumb that opened the layer restores typing.
static void select_layer_on_double_tap(tap_dance_state_t *state, uint8_t layer) {
    if (state->count == 2) {
        default_layer_set((layer_state_t)1 << layer);
    }
}

static void dance_boot(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) {
        reset_keyboard(); // Enter the bootloader, matching upstream Miryoku.
    }
}

static void dance_base(tap_dance_state_t *state, void *user_data) {
    select_layer_on_double_tap(state, BASE);
}

static void dance_extra(tap_dance_state_t *state, void *user_data) {
    select_layer_on_double_tap(state, EXTRA);
}

static void dance_tap(tap_dance_state_t *state, void *user_data) {
    select_layer_on_double_tap(state, TAP);
}

static void dance_button(tap_dance_state_t *state, void *user_data) {
    select_layer_on_double_tap(state, BUTTON);
}

static void dance_nav(tap_dance_state_t *state, void *user_data) {
    select_layer_on_double_tap(state, NAV);
}

static void dance_mouse(tap_dance_state_t *state, void *user_data) {
    select_layer_on_double_tap(state, MOUSE);
}

static void dance_media(tap_dance_state_t *state, void *user_data) {
    select_layer_on_double_tap(state, MEDIA);
}

static void dance_num(tap_dance_state_t *state, void *user_data) {
    select_layer_on_double_tap(state, NUM);
}

static void dance_sym(tap_dance_state_t *state, void *user_data) {
    select_layer_on_double_tap(state, SYM);
}

static void dance_fun(tap_dance_state_t *state, void *user_data) {
    select_layer_on_double_tap(state, FUN);
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_BOOT]   = ACTION_TAP_DANCE_FN(dance_boot),
    [TD_BASE]    = ACTION_TAP_DANCE_FN(dance_base),
    [TD_EXTRA]   = ACTION_TAP_DANCE_FN(dance_extra),
    [TD_TAP]     = ACTION_TAP_DANCE_FN(dance_tap),
    [TD_BUTTON]  = ACTION_TAP_DANCE_FN(dance_button),
    [TD_NAV]     = ACTION_TAP_DANCE_FN(dance_nav),
    [TD_MOUSE]   = ACTION_TAP_DANCE_FN(dance_mouse),
    [TD_MEDIA]   = ACTION_TAP_DANCE_FN(dance_media),
    [TD_NUM]     = ACTION_TAP_DANCE_FN(dance_num),
    [TD_SYM]     = ACTION_TAP_DANCE_FN(dance_sym),
    [TD_FUN]     = ACTION_TAP_DANCE_FN(dance_fun),
};

// 7. Shift + Caps Word key sends ordinary Caps Lock, as in Miryoku.
// The array form below is required by the target checkout's current QMK API.
const key_override_t caps_word_to_caps_lock = ko_make_basic(MOD_MASK_SHIFT, CW_TOGG, KC_CAPS);
const key_override_t *key_overrides[] = {
    &caps_word_to_caps_lock,
};

// 8. Start each boot on BASE, even if a previous keymap stored a different
// default layer in EEPROM. Tap Dance changes remain active until reboot.
// BASE starts in QWERTY. To boot in Colemak DH, change BASE here to EXTRA.
void keyboard_post_init_user(void) {
    default_layer_set((layer_state_t)1 << BASE);
}

// 9. Optional upstream two-thumb combos (disabled by default in rules.mk).
// Sofle has all six thumbs, so these are unnecessary unless you want them.
// These match full keycodes, including the LT() hold layer on BASE/EXTRA.
#ifdef COMBO_ENABLE
const uint16_t PROGMEM combo_base_right[] = {LT(SYM, KC_ENT), LT(NUM, KC_BSPC), COMBO_END};
const uint16_t PROGMEM combo_base_left[]  = {LT(NAV, KC_SPC), LT(MOUSE, KC_TAB), COMBO_END};
const uint16_t PROGMEM combo_nav[]        = {KC_ENT, KC_BSPC, COMBO_END};
const uint16_t PROGMEM combo_mouse[]      = {MS_BTN2, MS_BTN1, COMBO_END};
const uint16_t PROGMEM combo_media[]      = {KC_MSTP, KC_MPLY, COMBO_END};
const uint16_t PROGMEM combo_num[]        = {KC_0, KC_MINS, COMBO_END};
const uint16_t PROGMEM combo_sym[]        = {KC_RPRN, KC_UNDS, COMBO_END};
const uint16_t PROGMEM combo_fun[]        = {KC_SPC, KC_TAB, COMBO_END};

combo_t key_combos[] = {
    COMBO(combo_base_right, LT(FUN, KC_DEL)),
    COMBO(combo_base_left, LT(MEDIA, KC_ESC)),
    COMBO(combo_nav, KC_DEL),
    COMBO(combo_mouse, MS_BTN3),
    COMBO(combo_media, KC_MUTE),
    COMBO(combo_num, KC_DOT),
    COMBO(combo_sym, KC_LPRN),
    COMBO(combo_fun, KC_APP),
};
#endif

// 10. Sofle conveniences. Rotation keeps the existing default keymap behavior:
// left encoder = volume; right encoder = page up/down. Buttons are independent
// LAYOUT positions with audio/NAV actions. Rotation is disabled in rules.mk
// for this keyboard's replacement switches; enable it only with real encoders.
#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        tap_code16(clockwise ? KC_VOLU : KC_VOLD);
    } else if (index == 1) {
        tap_code16(clockwise ? KC_PGDN : KC_PGUP);
    }
    return false; // The event was handled here.
}
#endif

// Simple local OLED status. No external images, fonts, or layer-list macros.
// On the secondary half, only show a static label; live status is on the master.
#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

bool oled_task_user(void) {
    if (!is_keyboard_master()) {
        oled_write_ln_P(PSTR("SOFLE"), false);
        oled_write_ln_P(PSTR("MIRYU"), false);
        return false;
    }

    oled_write_ln_P(PSTR("MH SB"), false);
    oled_write_ln_P(PSTR("Layer"), false);
    switch (get_highest_layer(layer_state | default_layer_state)) {
        case BASE:   oled_write_ln_P(PSTR("BASE "), false); break;
        case EXTRA:  oled_write_ln_P(PSTR("EXTRA"), false); break;
        case TAP:    oled_write_ln_P(PSTR("TAP  "), false); break;
        case BUTTON: oled_write_ln_P(PSTR("BTN  "), false); break;
        case NAV:    oled_write_ln_P(PSTR("NAV  "), false); break;
        case MOUSE:  oled_write_ln_P(PSTR("MOUSE"), false); break;
        case MEDIA:  oled_write_ln_P(PSTR("MEDIA"), false); break;
        case NUM:    oled_write_ln_P(PSTR("NUM  "), false); break;
        case SYM:    oled_write_ln_P(PSTR("SYM  "), false); break;
        case FUN:    oled_write_ln_P(PSTR("FUN  "), false); break;
        default:     oled_write_ln_P(PSTR("?    "), false); break;
    }
    oled_write_ln_P(host_keyboard_led_state().caps_lock ? PSTR("CAPS ") : PSTR("     "), false);
    oled_write_ln_P(is_caps_word_on() ? PSTR("WORD ") : PSTR("     "), false);
    return false;
}
#endif
