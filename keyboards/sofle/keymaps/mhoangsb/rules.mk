# Self-contained Miryoku for Sofle. No users/manna-harbour_miryoku dependency.
# Required by the keycodes and behavior in keymap.c:
MOUSEKEY_ENABLE = yes
EXTRAKEY_ENABLE = yes
AUTO_SHIFT_ENABLE = yes
TAP_DANCE_ENABLE = yes
CAPS_WORD_ENABLE = yes
KEY_OVERRIDE_ENABLE = yes

# Upstream optional thumb combos. Enable to compile section 9 in keymap.c.
COMBO_ENABLE = no

# This Sofle has normal switches in the encoder push-button positions.
# Matrix button actions work without rotary support. Enable only with encoders.
ENCODER_ENABLE = no
ENCODER_MAP_ENABLE = no
OLED_ENABLE = yes
OLED_DRIVER = ssd1306

# Keep firmware compact on the stock Pro Micro / ATmega32U4 target.
LTO_ENABLE = yes
CONSOLE_ENABLE = no
COMMAND_ENABLE = no

# This keymap uses six individual LT() thumbs, not Lower+Raise+Adjust.
TRI_LAYER_ENABLE = no

# Lighting controls exist on MEDIA but need matching hardware/firmware support.
# Leave disabled for a standard Sofle without RGB LEDs. Choose ONE if needed.
RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = no
