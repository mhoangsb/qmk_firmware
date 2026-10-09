# mhoangsb: self-contained Miryoku for Sofle

See the [visual layer guide](visual-guide.md) for full keyboard images, tap/hold
views, and instructions for reaching every layer.

This directory contains a complete, editable implementation of the
Miryoku layout from `/home/mhoang/dev/miryoku_qmk`. The starting configuration is
QWERTY on BASE, Colemak DH on EXTRA, QWERTY without hold actions on TAP, the
VI navigation arrangement, and the original clipboard profile. All ten
layers are included. Every Sofle position is written explicitly in `keymap.c`.

NAV, MOUSE, and MEDIA use the upstream `MIRYOKU_NAV=VI` arrangement, written
directly into the arrays. No build flag is needed to enable it.

Only the usual QMK framework and Sofle hardware definition are required. There
are no dependencies on `users/manna-harbour_miryoku`, Miryoku Babel, a generated
layer header, another keymap directory, images, or extra local fonts.

The original Miryoku Sofle mapping uses 36 keys. This version keeps those core
positions and assigns all 24 extra positions for coding: numbers, editing keys,
direct modifiers, and audio controls. Functional layers inherit the extra keys
with NAV, MEDIA, and FUN overrides described below. This keyboard has ordinary
switches in the encoder button positions, so rotary support is disabled.

## 1. Start here

1. Open `keymap.c` and find `const uint16_t PROGMEM keymaps`.
2. Find `[BASE] = LAYOUT(...)` for the main typing layer.
3. Change one keycode in that array. Keep the commas and the number of arguments.
4. Build using the commands below. Flash both halves when ready.
5. Adjust tapping timing or the clipboard profile in `config.h` as needed.

| File | What to edit |
| --- | --- |
| `keymap.c`, sections 1–2 | Layer names/numbers and Tap Dance indices |
| `keymap.c`, section 3 | Actual clipboard shortcuts for each OS profile |
| `keymap.c`, section 4 | RGB keycode aliases |
| `keymap.c`, section 5 | Every key on every layer |
| `keymap.c`, sections 6–7 | Double-tap actions and Shift + Caps Word override |
| `keymap.c`, section 8 | Layer selected at startup |
| `keymap.c`, section 9 | Optional two-thumb combos |
| `keymap.c`, section 10 | Encoder actions and OLED display |
| `config.h` | Tap/hold timing, Auto Shift, mouse speed, clipboard profile, handedness |
| `rules.mk` | Which QMK features are compiled |

## 2. Build and flash

Run from the target QMK checkout, not from the Miryoku source repository:

```sh
cd /home/mhoang/qmk_firmware
qmk compile -kb sofle/rev1 -km mhoangsb
```

The equivalent Make command is:

```sh
make sofle/rev1:mhoangsb
```

For a stock Pro Micro / ATmega32U4 with Caterina bootloader:

```sh
qmk flash -kb sofle/rev1 -km mhoangsb
# Or:
make sofle/rev1:mhoangsb:avrdude
```

Connect one half by USB, invoke flash, and press its reset button when prompted.
Repeat for the other half. Both halves should run the same keymap firmware.
The normal output is `sofle_rev1_mhoangsb.hex` in the QMK checkout.

These commands select the hardware definition present in this checkout. Check
your actual PCB/controller before flashing. A different Sofle variant or an
RP2040 replacement controller needs its matching keyboard/converter target;
changing a keymap does not change the pinout or bootloader.

Default split handedness assumes USB is connected to the physical left half.
For USB always on the right, uncomment `MASTER_RIGHT` in `config.h`. For USB on
either half, configure `EE_HANDS` and program each half's handedness using the
instructions in your checkout's `docs/features/split_keyboard.md`. Defining
`EE_HANDS` alone does not program the EEPROM handedness.

For a clean rebuild after changing features:

```sh
qmk compile -c -kb sofle/rev1 -km mhoangsb
```

## 3. Physical positions and LAYOUT argument order

Each layer supplies 60 positions: 58 ordinary keys and two replacement switches
at the encoder push-button positions. The fourth line has fourteen arguments
because it includes those two switches.
The final line has ten thumb-row arguments. Keep the order `12, 12, 12, 14, 10`.
Finger columns use compact alignment within each layer; thumb rows use their own
spacing so long `LT()` expressions do not widen the rest of the array. The gap
between hands on the upper rows lines up with the two encoder button positions.

The names below are labels for physical positions viewed from above. QMK's JSON
hardware definition handles the actual matrix coordinates, including the right
half's reversed column wiring.

```text
L00 L01 L02 L03 L04 L05                    R00 R01 R02 R03 R04 R05
L10 L11 L12 L13 L14 L15                    R10 R11 R12 R13 R14 R15
L20 L21 L22 L23 L24 L25                    R20 R21 R22 R23 R24 R25
L30 L31 L32 L33 L34 L35 [LENC]      [RENC] R30 R31 R32 R33 R34 R35
         L40 L41 L42 L43 L44          R41 R42 R43 R44 R45
```

Active Miryoku finger positions are `L11..L15`, `L21..L25`, `L31..L35` and
`R10..R14`, `R20..R24`, `R30..R34`. Active thumbs are `L42..L44` and `R41..R43`.

The 24 extra positions outside the Miryoku core are:

- `L00..L05`, `R00..R05`: number row.
- `L10`, `L20`, `L30`, `R15`, `R25`, `R35`: outer columns.
- `LENC`, `RENC`: normal switches at the encoder push-button positions.
- `L40`, `L41`, `R44`, `R45`: extra thumb keys.

`KC_NO` means the position does nothing and blocks fallback to lower layers.
`KC_TRNS` means use the assignment from a lower active/default layer. Aliases
`XXXXXXX` and `_______` mean `KC_NO` and `KC_TRNS`, respectively.

Extra positions use explicit assignments on BASE, EXTRA, and TAP. BUTTON, NAV,
MOUSE, MEDIA, NUM, SYM, and FUN use `KC_TRNS` there, except for the overrides
below. Intentional `KC_NO` entries inside the 36-key Miryoku core stay blocked;
do not replace them all with transparency.

### Extra keys for coding

BASE, EXTRA, and TAP have the same extra-key assignments:

| Physical positions | Assignments, left to right |
| --- | --- |
| `L00..L05` | Backtick, 1, 2, 3, 4, 5 |
| `R00..R05` | 6, 7, 8, 9, 0, Equals |
| `L10/L20/L30` | Escape, Tab, Left Shift |
| `R15/R25/R35` | Backspace, Backslash, Enter |
| `L40/L41` | Left Alt, Left Ctrl |
| `R44/R45` | Left Shift, Left GUI |
| `LENC/RENC` | Mute, Play/Pause |

These are ordinary keycodes with no tap/hold actions. Holding the extra Ctrl
or Shift activates that modifier immediately. GUI is Windows/Super/Command,
depending on the host. Shift provides the usual shifted number-row symbols,
tilde, plus, and pipe with a US host layout; eligible keys also retain Auto Shift.

```text
`  1 2 3 4 5                      6 7 8 9 0 =
Esc [Miryoku upper finger row]     [Miryoku upper finger row] Bspc
Tab [Miryoku home row]             [Miryoku home row]         \
Sft [Miryoku lower finger row] [Mute] [Play] [Miryoku lower finger row] Enter
    Alt Ctrl [Esc Space Tab]       [Enter Bspc Delete] Shift GUI
```

Bracketed three-key thumb groups above show BASE/EXTRA tap actions; their
existing layer holds are still available. On TAP, those six thumbs are plain keys.

| Layer | Extra-key override |
| --- | --- |
| FUN | Number row becomes F1..F6 on the left, F7..F12 on the right |
| NAV | `LENC` sends Ctrl+O; `RENC` sends Ctrl+I |
| MEDIA | `LENC` sends Volume Down; `RENC` sends Volume Up |

All other extra positions inherit the selected typing/default layer, including
the direct modifiers and editing keys. For example, holding left Space for NAV
still leaves the outer Escape and spare thumb Ctrl available.

### Neovim examples

- Press the outer Escape (`L10`) to leave Insert mode without tap/hold timing.
- Hold spare thumb Ctrl (`L41`) and press W, then a window command, for Neovim's
  `Ctrl-W` window operations. Use Ctrl+U/D to scroll or Ctrl+R to redo in Normal mode.
- In Normal mode, hold the left Space thumb for NAV, then press `LENC` for
  Ctrl+O (older jump) or `RENC` for Ctrl+I (newer jump). These use Neovim's
  default jump-list commands; your own mappings may change their behavior.
  Terminals often treat Ctrl+I and Tab as the same input.
- Use the extra Shift keys for coding symbols while retaining home-row modifiers.

Neovim command reference: <https://neovim.io/doc/user/vimindex/>.
The firmware sends these keys in every application; it does not detect Neovim
or its mode. File finding, formatting, diagnostics, and plugin shortcuts remain
configured in Neovim rather than being added as firmware command sequences.

## 4. Tap/hold behavior and the six thumb layers

| Physical position | Tap | Hold from BASE/EXTRA |
| --- | --- | --- |
| `L42` | Escape | MEDIA |
| `L43` | Space | NAV |
| `L44` | Tab | MOUSE |
| `R41` | Enter | SYM |
| `R42` | Backspace | NUM |
| `R43` | Delete | FUN |

`LT(NAV, KC_SPC)` means tap for Space, hold for NAV. A layer activated by `LT`
remains active until that physical key is released. These thumb-layer keycodes
are repeated explicitly on BASE and EXTRA. TAP has plain thumb keys instead.

On BASE, the home row is:

```text
tap:   A    S    D     F     G          H     J     K     L    '
hold: GUI  Alt Ctrl  Shift   -          -   Shift Ctrl   Alt  GUI
```

For example, `LCTL_T(KC_D)` taps D and holds Left Ctrl. Hold actions on both hands
use left-side GUI/Alt/Ctrl/Shift, matching upstream. `ALGR_T(KC_X)` and
`ALGR_T(KC_DOT)` produce Right Alt / AltGr when held. The outer bottom-row keys
`LT(BUTTON, KC_Z)` and `LT(BUTTON, KC_SLSH)` access BUTTON when held.

Colemak DH EXTRA has the same hold actions at the same physical positions; the
letters are different.

## 5. Complete layer reference

The following diagrams show the 36 Miryoku core positions only. Each finger row
has five keys per hand, and the thumb row has three per hand. The 24 extra Sofle
positions and their layer overrides are documented in section 3.

Legend:

- `--`: `KC_NO`, blocked.
- `Boot2`, `Base2`, etc.: exactly two taps perform the action; one does nothing.
- `GUI`: Windows/Super/Command modifier.
- `AltGr`: Right Alt, interpreted according to the host keyboard layout.
- `Btn1/2/3`: left/right/middle mouse buttons.
- `Word`: Caps Word toggle; Shift + this key sends Caps Lock instead.
- `Undo/Cut/Copy/Paste/Redo`: the clipboard profile's shortcuts.

### BASE: QWERTY

```text
Q   W   E   R   T                  Y   U   I   O   P
A   S   D   F   G                  H   J   K   L   '
Z   X   C   V   B                  N   M   ,   .   /
       Esc Space Tab          Enter Bspc Delete
```

All letter hold actions are described in section 4. The last right home-row key
is apostrophe, not semicolon, matching Miryoku's QWERTY alternative. Modifiers,
AltGr, BUTTON, and thumb holds remain available. QMK keycodes assume the host's
US keyboard mapping for these punctuation labels. Changing the host layout can
change the produced characters.

### EXTRA: Colemak DH

```text
Q   W   F   P   B                  J   L   U   Y   '
A   R   S   T   G                  M   N   E   I   O
Z   X   C   D   V                  K   H   ,   .   /
       Esc Space Tab          Enter Bspc Delete
```

Modifiers, AltGr, BUTTON, and thumb holds remain available at the same physical
positions as BASE.

### TAP: QWERTY with plain keys

Same tap characters as BASE, with no mod-tap or layer-tap assignments. Auto Shift
is still enabled for eligible nonalphabetic keys. This is not an all-QMK-features
off mode. Optional enabled thumb combos also still match any relevant plain
keycodes on TAP, as in the upstream combo implementation.

TAP's active keys provide no layer access. Disconnect/reconnect USB to return to
BASE, or assign `TD(TD_BASE)` to a spare TAP position for a direct escape route.

### BUTTON: hold Z or slash from BASE/EXTRA

```text
Undo Cut Copy Paste Redo           Redo Paste Copy Cut Undo
GUI  Alt Ctrl Shift --             --   Shift Ctrl Alt GUI
Undo Cut Copy Paste Redo           Redo Paste Copy Cut Undo
       Btn3 Btn1 Btn2              Btn2 Btn1 Btn3
```

### NAV: hold left Space

```text
Boot2 Tap2 Extra2 Base2 --         Redo Paste Copy Cut Undo
GUI   Alt  Ctrl   Shift --        Left Down Up Right Word
--    AltGr Num2  Nav2  --        Home PgDn PgUp End Ins
          -- -- --                Enter Bspc Delete
```

VI places Left/Down/Up/Right at `R20/R21/R22/R23`, the QWERTY H/J/K/L positions.
These keys send ordinary arrow keycodes in any application. Caps Word moves to
`R24`, the QWERTY apostrophe position. Home/Page Down/Page Up/End/Insert occupy
`R30..R34`. These physical positions stay the same on Colemak DH EXTRA.

To make an inverted T, edit the right-hand NAV positions directly; for example
place Up at `R12` and
Left/Down/Right at `R21/R22/R23`. The same choice can be made for MOUSE separately.

### MOUSE: hold left Tab

```text
Boot2 Tap2 Extra2 Base2 --         Redo Paste Copy Cut Undo
GUI   Alt  Ctrl   Shift --        MsLeft MsDown MsUp MsRight --
--    AltGr Sym2  Mouse2 --       WhLeft WhDown WhUp WhRight --
          -- -- --                Btn2 Btn1 Btn3
```

Pointer movement uses the same H/J/K/L positions as VI navigation. Wheel
movement uses the four positions directly below them (`R30..R33`).

### MEDIA: hold left Escape

```text
Boot2 Tap2 Extra2 Base2 --         RGBnext Hue+ Sat+ Value+ RGBtoggle
GUI   Alt  Ctrl   Shift --        Prev Vol- Vol+ Next --
--    AltGr Fun2  Media2 --       -- -- -- -- OutputAuto
          -- -- --                Stop Play/Pause Mute
```

RGB actions need RGB hardware and a matching enabled feature in `rules.mk`.
Lighting is disabled by default. `OU_AUTO` selects automatic output routing on
supported multi-output builds; stock wired Sofle has no Bluetooth output to
switch to. The key is retained from the original layout.

### NUM: hold right Backspace

```text
[    7    8    9    ]              -- Base2 Extra2 Tap2 Boot2
;    4    5    6    =              -- Shift Ctrl   Alt  GUI
`    1    2    3    \              -- Num2  Nav2   AltGr --
         .  0  -                  -- -- --
```

These are top-row digit keycodes, not keypad keycodes. Hold an eligible number
or punctuation key for its Auto Shift character.

### SYM: hold right Enter

```text
{    &    *    (    }              -- Base2 Extra2 Tap2 Boot2
:    $    %    ^    +              -- Shift Ctrl   Alt  GUI
~    !    @    #    |              -- Sym2  Mouse2 AltGr --
         (  )  _                  -- -- --
```

### FUN: hold right Delete

```text
F12  F7   F8   F9   PrintScreen    -- Base2 Extra2 Tap2 Boot2
F11  F4   F5   F6   ScrollLock     -- Shift Ctrl   Alt  GUI
F10  F1   F2   F3   Pause          -- Fun2  Media2 AltGr --
         Menu Space Tab           -- -- --
```

## 6. Switching layers and getting back

There are two distinct operations:

| Assignment | Behavior |
| --- | --- |
| `LT(NAV, KC_SPC)` | Tap Space; hold NAV; release to leave NAV |
| `MO(NAV)` | Hold NAV without a tap character |
| `TG(NAV)` | Toggle NAV on/off in the active layer state |
| `TO(BASE)` | Replace the active layer state; does not replace the default layer |
| `DF(BASE)` | Select BASE as the default layer for this session |
| `TD(TD_BASE)` | Select BASE as default after exactly two taps |

Miryoku's layer selectors use guarded Tap Dance actions, not `TG`. They select
the default layer, so a functional layer can become the resting layout. None
of these Tap Dance actions writes to EEPROM.

For example, while BASE is selected:

1. Hold left Space to open NAV.
2. Double-tap physical `L13` (BASE's E position) to select EXTRA.
3. Release Space. You are now typing Colemak DH.
4. Hold Space again and double-tap physical `L14` to select BASE.

From NAV, the top-row selector positions `L11/L12/L13/L14` are
Boot/TAP/EXTRA/BASE. From NUM/SYM/FUN, `R11/R12/R13/R14` are
BASE/EXTRA/TAP/Boot. These locations remain the same regardless of which letters
you put on the typing layer.

To enter the bootloader from the keymap, open a functional layer and double-tap
its `TD(TD_BOOT)` key. A single tap or three taps does not execute the action.
The action runs when QMK finishes the dance, possibly after a short timeout.

The local `keyboard_post_init_user()` deliberately selects BASE at every boot,
including when EEPROM contains a default-layer value from the previous keymap.
This gives a predictable escape from TAP and other resting layers. Change BASE
to EXTRA in this function to start with Colemak DH. This startup rule is a local
addition; upstream Miryoku's layer selection itself is otherwise preserved.

## 7. Common edits

### Change one key or remove a home-row modifier

In `[BASE]`, change `LCTL_T(KC_D)` to `KC_D` to make that position always D. To
keep the modifier but change the tap character, use e.g. `LCTL_T(KC_S)`.

Useful building blocks:

```c
KC_ESC                 // Plain Escape
LCTL_T(KC_A)           // Tap A, hold Left Ctrl
LSFT_T(KC_F)           // Tap F, hold Left Shift
LT(NAV, KC_SPC)        // Tap Space, hold NAV
MO(SYM)                // Hold-only SYM access
C(KC_C)                // Ctrl+C
G(KC_C)                // GUI/Command+C
S(KC_TAB)              // Shift+Tab
TD(TD_BASE)            // Guarded return to BASE
```

`LT`'s tap key must be a basic 8-bit keycode. For example, `LT(NAV, C(KC_C))`
cannot encode the Ctrl+C shortcut as its tap action. Use a custom event handler
or Tap Dance for more complex tap/hold actions.

BASE, EXTRA, and TAP are separate arrays. Editing BASE does not update the other
two. Update TAP yourself if you want its tap letters to track your new BASE.

### Choose the startup layout

BASE already starts in QWERTY. To start in Colemak DH, replace the startup `BASE`
with `EXTRA` in `keyboard_post_init_user()`. EXTRA has all Miryoku hold actions.
TAP remains QWERTY until you edit it.

### Customize the number row and extra keys

The first row in BASE, EXTRA, and TAP is already:

```c
KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5,
KC_6, KC_7, KC_8, KC_9, KC_0, KC_EQL,
```

Keep it as one twelve-argument row inside `LAYOUT`. Edit all three typing layers
if you want a shared assignment. Functional layers already inherit this row,
except FUN, whose row is F1..F12. The same inheritance applies to the outer
columns and extra thumbs. NAV and MEDIA have explicit replacement-button actions.

Other useful extra assignments are `KC_ESC`, `KC_LSFT`, `MO(NAV)`, `CW_TOGG`, or
`TD(TD_BASE)`. For example, replace the first thumb argument (`L40`) on TAP with
`TD(TD_BASE)` to allow returning from TAP without reconnecting USB. Replace
`LENC` is already `KC_MUTE` on all three typing layers.

### Add a layer

1. Add a name such as `GAME` at the end of `enum layers`.
2. Copy an entire sixty-position `LAYOUT` entry and name it `[GAME]`.
3. Assign `MO(GAME)`, `TG(GAME)`, or `DF(GAME)` to an accessible position.
4. Put an exit key on GAME if using a toggle/default layer.
5. Add an OLED switch case if you want its name displayed.

If you want a guarded selector too, add `TD_GAME` to `enum tap_dances`, a
`dance_game()` callback calling `select_layer_on_double_tap(state, GAME)`, and
a `[TD_GAME] = ACTION_TAP_DANCE_FN(dance_game)` table entry. Assign it with
`TD(TD_GAME)`. All existing examples are written out in section 6 of `keymap.c`.

Keep layers used by `LT()` below 16. You do not need a shared layer-list macro,
generator, or userspace change to add a layer here.

### Tune tap/hold timing

Start with `TAPPING_TERM 200` in `config.h`. Increase it if slow taps turn into
holds. Decrease it if holds take too long to become modifiers/layers. Timing
also interacts with key press/release order; it is not just a stopwatch rule.

`QUICK_TAP_TERM 0` allows tap-then-hold to invoke the hold action immediately
instead of repeating the tap character. For example, tap-then-hold Backspace
opens NUM instead of repeating deletion. To favor repetition, try a nonzero
`QUICK_TAP_TERM` no larger than `TAPPING_TERM`.

Current QMK already uses the mod-tap interruption behavior requested by old
Miryoku. `IGNORE_MOD_TAP_INTERRUPT` is obsolete and causes an error in this
checkout. `PERMISSIVE_HOLD` and `HOLD_ON_OTHER_KEY_PRESS` are left undefined;
adding them changes how typing rolls choose tap versus hold.

Changing `TAPPING_TERM` also changes Auto Shift's timeout here, since
`AUTO_SHIFT_TIMEOUT` is defined as `TAPPING_TERM`. Assign a separate numeric
timeout if you want to tune them independently. It also affects the timing of
the guarded double taps.

### Choose clipboard shortcuts

Edit `MHOANGSB_CLIPBOARD` in `config.h`:

| Value | Undo | Redo | Cut/Copy/Paste |
| --- | --- | --- | --- |
| `0` | USB Undo | USB Again | Shift+Delete / Ctrl+Insert / Shift+Insert |
| `1` | Ctrl+Z | Ctrl+Y | Ctrl+X / Ctrl+C / Ctrl+V |
| `2` | Command+Z | Command+Shift+Z | Command+X / Command+C / Command+V |

Profile 0 preserves upstream's default. Profile 1 is often more useful for
Windows/Linux applications; some Linux applications use Ctrl+Shift+Z for Redo.
Edit the aliases in section 3 of `keymap.c` to fit your applications. Clipboard
shortcuts are host/application dependent, and firmware cannot ensure every
application treats them alike.

### Caps Word, Caps Lock, and Auto Shift

Open NAV and tap `CW_TOGG` at `R24` (QWERTY apostrophe position) to toggle Caps
Word. QMK capitalizes word characters until a terminating key; this is different
from toggling the host's Caps Lock state. Hold Shift and tap this same key for
ordinary Caps Lock; the key override handles that replacement.

For Auto Shift, holding an eligible number or punctuation key past the configured
timeout sends its shifted form. Letters are excluded by `NO_AUTO_SHIFT_ALPHA`.
To disable Auto Shift, set `AUTO_SHIFT_ENABLE = no` in `rules.mk`; the timing
defines can remain in `config.h` without effect.

If you turn off Caps Word or key overrides, also remove their dependent code:
`CW_TOGG` assignments, `caps_word_to_caps_lock` / `key_overrides`, and the OLED's
`is_caps_word_on()` call as applicable. Likewise, Tap Dance cannot be disabled
while the `TD(...)` assignments and action table still depend on it.

### Mouse speed

`MOUSEKEY_DELAY` controls the initial movement delay. `MOUSEKEY_INTERVAL` controls
the movement update interval. `MOUSEKEY_MAX_SPEED` sets the maximum pointer speed
multiplier, and `MOUSEKEY_TIME_TO_MAX` is the number of movement reports used to
reach maximum speed. These settings are not pixels per second. The local values
match upstream Miryoku. Wheel delay has its own setting.

### Encoders, OLED, and RGB

`ENCODER_ENABLE = no` matches this keyboard's normal replacement switches.
Their actions are matrix keycodes in `LAYOUT` and work with rotation disabled.
If real encoders are installed later, set `ENCODER_ENABLE = yes` in `rules.mk`.
The retained callback then uses left rotation for volume and right rotation
for page up/down.

`encoder_update_user()` currently ignores layers. Change its `tap_code16(...)`
keycodes to change the actions. To make them depend on the layer, switch on
`get_highest_layer(layer_state | default_layer_state)` inside that function.
If rotation is opposite to your preference, swap the clockwise/counterclockwise
keycodes. Encoder push buttons are ordinary keys in `LAYOUT`, not part of this
callback. `ENCODER_MAP_ENABLE` remains off because this implementation uses the
callback instead of a separate encoder map.

OLED draws a compact text status on the USB/master half: active layer, Caps Lock,
and Caps Word. The secondary half shows a static label and does not require
extra synchronization. Change `OLED_ROTATION_270` or the text calls to customize
it. Add cases for new layers. You can set `OLED_ENABLE = no` or
`ENCODER_ENABLE = no`; both callbacks are already guarded with `#ifdef`.

RGB is disabled by default. Enable the feature appropriate for your PCB and LED
wiring in `rules.mk`. The aliases select Matrix keycodes when RGB Matrix is
enabled, otherwise RGB Light/underglow keycodes. Merely enabling a feature does
not validate LED count or pin wiring. Keep RGB disabled on hardware without LEDs.

The stock AVR target has limited program memory. `LTO_ENABLE = yes` helps reduce
size. If additions exceed the firmware limit, OLED or optional combos are useful
features to disable; do not bypass the bootloader size check.

## 8. Optional upstream thumb combos

Set `COMBO_ENABLE = yes` in `rules.mk` to compile the local combo definitions.
They are disabled initially because Sofle already has all six required thumbs.
Their timing is `COMBO_TERM 200` in `config.h`.

| Input pair | Output |
| --- | --- |
| BASE/EXTRA Enter + Backspace layer-taps | Delete / hold FUN |
| BASE/EXTRA Space + Tab layer-taps | Escape / hold MEDIA |
| Plain Enter + Backspace | Delete |
| Mouse Button 2 + Button 1 | Button 3 |
| Media Stop + Play | Mute |
| Number 0 + minus | Decimal point |
| Symbol right parenthesis + underscore | Left parenthesis |
| Function Space + Tab | Menu |

Combos match keycodes, not physical coordinates. The plain-key pairs can match
wherever those keycodes appear (including BUTTON or TAP). There is no layer
restriction added here. If you change a thumb's `LT()` assignment, update the
corresponding combo definition too. QMK derives the combo count from the array;
there is no manual `COMBO_COUNT` to maintain.

## 9. Differences from the generated Miryoku implementation

- All ten default layers are expanded into native Sofle arrays. Generated
  `MIRYOKU_LAYER_*` macros and the `LAYOUT_miryoku` mapping are gone.
- NAV, MOUSE, and MEDIA use the upstream VI alternative arrangement.
- The 24 extra physical positions have coding, modifier, and audio assignments.
  Functional layers inherit them, with NAV/MEDIA button and FUN number-row overrides.
- Modern QMK key override, mouse (`MS_*`), and RGB keycode forms replace the old APIs.
- The obsolete `IGNORE_MOD_TAP_INTERRUPT` define is omitted; current QMK already
  supplies that mod-tap behavior by default.
- Startup explicitly selects BASE (or your chosen startup layer), ignoring a
  default-layer value left by the previous firmware. Layer switches stay local
  to the current session.
- Simple local encoder and OLED callbacks are included for Sofle convenience.
- Optional upstream combos are local and disabled by default.
- Original `U_NA` (unavailable) and `U_NU` (unused) positions inside the Miryoku
  core are written directly as `KC_NO`; their runtime behavior is identical.

Build flags such as `MIRYOKU_ALPHAS=QWERTY`, `MIRYOKU_NAV=VI`, or
`MIRYOKU_NAV=INVERTEDT` do not select layouts in this standalone version. Edit
the exposed arrays directly.
Other upstream alternatives are not a hidden selectable collection here: the
included arrays are the complete layout with VI navigation plus its EXTRA and
TAP layers.

Miryoku design and original implementation: Manna Harbour,
<https://github.com/manna-harbour/miryoku> and
<https://github.com/manna-harbour/miryoku_qmk>. Derived code retains the
GPL-2.0-or-later license. See `LICENSE` for GPL version 2; the SPDX declaration
also permits use under later GPL versions.
