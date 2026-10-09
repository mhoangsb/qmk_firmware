// Copyright 2026 mhoang
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

// Clipboard profile: 0 = upstream Miryoku default, 1 = Windows/Linux, 2 = macOS.
// Edit this number; keymap.c exposes the actual shortcuts for each profile.
#define MHOANGSB_CLIPBOARD 0

// Miryoku tap/hold timing, in milliseconds. Larger = more time to produce a tap.
#undef TAPPING_TERM
#define TAPPING_TERM 200

// A quick tap followed by a hold becomes the hold action rather than repeat.
#define QUICK_TAP_TERM 0

// Current QMK already ignores interruptions for mod-taps by default.
// Do not add the obsolete IGNORE_MOD_TAP_INTERRUPT define from old Miryoku.
// HOLD_ON_OTHER_KEY_PRESS and PERMISSIVE_HOLD are intentionally left undefined.

// Auto Shift applies to eligible numbers/punctuation, not letters.
// Holding past this timeout sends the shifted version of an eligible key.
#define NO_AUTO_SHIFT_ALPHA
#define AUTO_SHIFT_TIMEOUT TAPPING_TERM
#define AUTO_SHIFT_NO_SETUP

// Mouse keys: delay and interval are milliseconds; other values control speed.
#undef MOUSEKEY_DELAY
#define MOUSEKEY_DELAY 0
#undef MOUSEKEY_INTERVAL
#define MOUSEKEY_INTERVAL 16
#undef MOUSEKEY_WHEEL_DELAY
#define MOUSEKEY_WHEEL_DELAY 0
#undef MOUSEKEY_MAX_SPEED
#define MOUSEKEY_MAX_SPEED 6
#undef MOUSEKEY_TIME_TO_MAX
#define MOUSEKEY_TIME_TO_MAX 64

// Only used when COMBO_ENABLE = yes in rules.mk. No fixed COMBO_COUNT is needed
// in this checkout: QMK derives it from key_combos[].
#ifdef COMBO_ENABLE
#    define COMBO_TERM 200
#    define EXTRA_SHORT_COMBOS
#endif

// Split handedness: QMK defaults to USB on the physical left half.
// If you always plug USB into the right half, uncomment:
// #define MASTER_RIGHT
// For USB on either half, use QMK's EE_HANDS setup; see readme.md.
