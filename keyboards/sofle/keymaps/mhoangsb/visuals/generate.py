#!/usr/bin/env python3
"""Render the local keymap as SVG and PNG using the Sofle layout and rsvg-convert."""
import html
import json
from pathlib import Path
import re
import subprocess

HERE = Path(__file__).resolve().parent
KEYMAP = HERE.parent
QMK = next(p for p in KEYMAP.parents if (p / 'keyboards/sofle/rev1/keyboard.json').exists())
GEOMETRY = json.loads((QMK / 'keyboards/sofle/rev1/keyboard.json').read_text())['layouts']['LAYOUT']['layout']
SOURCE = (KEYMAP / 'keymap.c').read_text()
LAYERS = {}
for name, body in re.findall(r'\[(\w+)\] = LAYOUT\(\n(.*?)\n    \)', SOURCE, re.S):
    keys, start, depth = [], 0, 0
    for i, ch in enumerate(body):
        if ch == '(': depth += 1
        elif ch == ')': depth -= 1
        elif ch == ',' and depth == 0:
            keys.append(body[start:i].strip())
            start = i + 1
    if body[start:].strip(): keys.append(body[start:].strip())
    assert len(keys) == len(GEOMETRY) == 60, name
    LAYERS[name] = keys
assert len(LAYERS) == 10
POSITIONS = [f'{hand}{row}{col}' for row in range(3) for hand in 'LR' for col in range(6)]
POSITIONS += [f'L3{col}' for col in range(6)] + ['LENC', 'RENC'] + [f'R3{col}' for col in range(6)]
POSITIONS += [f'L4{col}' for col in range(5)] + [f'R4{col}' for col in range(1, 6)]
LABELS = {
    'KC_NO': 'Blocked', 'KC_ESC': 'Esc', 'KC_SPC': 'Space', 'KC_TAB': 'Tab',
    'KC_ENT': 'Enter', 'KC_BSPC': 'Backspace', 'KC_DEL': 'Delete',
    'KC_LGUI': 'GUI', 'KC_LALT': 'Alt', 'KC_LCTL': 'Ctrl', 'KC_LSFT': 'Shift',
    'KC_ALGR': 'AltGr', 'KC_GRV': '`', 'KC_EQL': '=', 'KC_BSLS': '\\',
    'KC_QUOT': "'", 'KC_COMM': ',', 'KC_DOT': '.', 'KC_SLSH': '/',
    'KC_LBRC': '[', 'KC_RBRC': ']', 'KC_SCLN': ';', 'KC_MINS': '-',
    'KC_LCBR': '{', 'KC_RCBR': '}', 'KC_AMPR': '&', 'KC_ASTR': '*',
    'KC_LPRN': '(', 'KC_RPRN': ')', 'KC_COLN': ':', 'KC_DLR': '$',
    'KC_PERC': '%', 'KC_CIRC': '^', 'KC_PLUS': '+', 'KC_TILD': '~',
    'KC_EXLM': '!', 'KC_AT': '@', 'KC_HASH': '#', 'KC_PIPE': '|',
    'KC_UNDS': '_', 'KC_LEFT': 'Left', 'KC_DOWN': 'Down', 'KC_UP': 'Up',
    'KC_RGHT': 'Right', 'KC_HOME': 'Home', 'KC_END': 'End', 'KC_INS': 'Insert',
    'KC_PGDN': 'Page Down', 'KC_PGUP': 'Page Up', 'CW_TOGG': 'Caps Word',
    'KC_MUTE': 'Mute', 'KC_MPLY': 'Play/Pause', 'KC_MSTP': 'Stop',
    'KC_MPRV': 'Previous', 'KC_MNXT': 'Next', 'KC_VOLD': 'Volume -', 'KC_VOLU': 'Volume +',
    'KC_PSCR': 'Print Scr', 'KC_SCRL': 'Scroll Lk', 'KC_PAUS': 'Pause', 'KC_APP': 'Menu',
    'RGB_NEXT': 'RGB next', 'RGB_HUE_UP': 'Hue +', 'RGB_SAT_UP': 'Sat +',
    'RGB_VAL_UP': 'Value +', 'RGB_TOGGLE': 'RGB toggle', 'OU_AUTO': 'Output auto',
    'MS_LEFT': 'Mouse left', 'MS_DOWN': 'Mouse down', 'MS_UP': 'Mouse up', 'MS_RGHT': 'Mouse right',
    'MS_WHLL': 'Wheel left', 'MS_WHLD': 'Wheel down', 'MS_WHLU': 'Wheel up', 'MS_WHLR': 'Wheel right',
    'MS_BTN1': 'Left click', 'MS_BTN2': 'Right click', 'MS_BTN3': 'Middle click',
    'C(KC_O)': 'Ctrl+O', 'C(KC_I)': 'Ctrl+I',
}
for action in ('UNDO', 'CUT', 'COPY', 'PASTE', 'REDO'): LABELS['CLIP_' + action] = action.title()
SHIFTED = dict(zip([f'KC_{n}' for n in '1234567890'], '!@#$%^&*()'))
SHIFTED.update({'KC_GRV': '~', 'KC_EQL': '+', 'KC_BSLS': '|', 'KC_QUOT': '"',
                'KC_COMM': '<', 'KC_DOT': '>', 'KC_SLSH': '?', 'KC_LBRC': '{',
                'KC_RBRC': '}', 'KC_SCLN': ':', 'KC_MINS': '_'})
MODS = {'LGUI_T': 'GUI', 'LALT_T': 'Alt', 'LCTL_T': 'Ctrl', 'LSFT_T': 'Shift', 'ALGR_T': 'AltGr'}
COLORS = {'plain': ('#ffffff', '#cad5e2'), 'inherit': ('#f0f5fa', '#c1cfdf'),
          'hold': ('#e8e5ff', '#9584e8'), 'layer': ('#dff4ed', '#60b59b'),
          'dance': ('#fff0d8', '#e2aa4b'), 'blocked': ('#e7ebf0', '#d3dae3'),
          'auto': ('#e4f0ff', '#78a8df')}

def label(code):
    if code in LABELS: return LABELS[code]
    if re.fullmatch(r'KC_([A-Z]|[0-9]|F\d+)', code): return code[3:]
    raise ValueError('Unlabelled keycode: ' + code)

def describe(code, view):
    if code == 'KC_NO': return 'Blocked', '', 'blocked'
    lt = re.fullmatch(r'LT\((\w+),\s*(KC_\w+)\)', code)
    if lt:
        return (lt[1], 'hold for layer', 'layer') if view == 'hold' else (label(lt[2]), 'hold: ' + lt[1], 'layer')
    mt = re.fullmatch(r'(\w+_T)\((KC_\w+)\)', code)
    if mt:
        return (MODS[mt[1]], 'hold modifier', 'hold') if view == 'hold' else (label(mt[2]), 'hold: ' + MODS[mt[1]], 'hold')
    td = re.fullmatch(r'TD\(TD_(\w+)\)', code)
    if td: return td[1], 'double tap', 'dance'
    if view == 'hold' and code in SHIFTED: return SHIFTED[code], 'Auto Shift*', 'auto'
    return label(code), '', 'plain'


def render(name, view='keys'):
    title = name + (' / TAP ACTIONS' if view == 'tap' else ' / HOLD ACTIONS' if view == 'hold' else ' / KEY ACTIONS')
    sub = 'Physical Sofle positions · 60 keys · replacement buttons included'
    parts = ['<svg xmlns="http://www.w3.org/2000/svg" width="1600" height="730" viewBox="0 0 1600 730">',
             '<rect width="1600" height="730" fill="#f8fafc"/>',
             '<g font-family="DejaVu Sans, sans-serif" fill="#172c43">']
    def text(x, y, value, size=18, anchor='start', color='#172c43', weight='normal'):
        parts.append(f'<text x="{x}" y="{y}" font-size="{size}" text-anchor="{anchor}" fill="{color}" font-weight="{weight}">{html.escape(value)}</text>')
    text(30, 38, title, 28, weight='bold')
    text(30, 66, sub, 15, color='#52667b')
    text(275, 97, 'LEFT HAND', 14, 'middle', '#52667b')
    text(1290, 97, 'RIGHT HAND', 14, 'middle', '#52667b')
    for i, (code, geo) in enumerate(zip(LAYERS[name], GEOMETRY)):
        inherited = code == 'KC_TRNS'
        if inherited: code = LAYERS['BASE'][i]
        main, secondary, kind = describe(code, view)
        if inherited: kind, secondary = 'inherit', 'from BASE / EXTRA'
        fill, stroke = COLORS[kind]
        x, y = 30 + geo['x'] * 93, 112 + geo['y'] * 84
        w, h = 86, geo.get('h', 1) * 84 - 9
        parts.append(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="9" fill="{fill}" stroke="{stroke}" stroke-width="1.5"/>')
        text(x + 8, y + 16, POSITIONS[i], 10, color='#607286')
        size = 20 if len(main) <= 5 else 14 if len(main) <= 10 else 12
        text(x + w / 2, y + h / 2 + 6, main, size, 'middle', weight='bold')
        if secondary: text(x + w / 2, y + h - 10, secondary, 8, 'middle', '#52667b')
    legend = [('plain', 'Direct key'), ('inherit', 'Inherited extra key'), ('hold', 'Tap / modifier'),
              ('layer', 'Tap / layer'), ('dance', 'Double-tap selector'), ('blocked', 'Blocked')]
    for i, (kind, caption) in enumerate(legend):
        x = 30 + i * 250
        fill, stroke = COLORS[kind]
        parts.append(f'<rect x="{x}" y="620" width="18" height="18" rx="4" fill="{fill}" stroke="{stroke}"/>')
        text(x + 27, 634, caption, 14)
    if view == 'hold':
        text(30, 672, '* Auto Shift: eligible numbers/punctuation become shifted after 200 ms when used without other modifiers.', 16)
        text(30, 699, 'Letter dual-role keys hold modifiers/layers; ordinary letters keep their usual behavior. Layer holds end on release.', 16)
    else:
        text(30, 672, 'Double-tap selectors: exactly two taps. Blocked keys do nothing. GUI = Windows / Super / Command.', 16)
        text(30, 699, 'Inherited extras match BASE and EXTRA. This view assumes one functional layer over a typing layer.', 16)
    parts.append('</g></svg>')
    stem = name.lower() + ('-' + view if view in ('tap', 'hold') else '')
    svg = HERE / (stem + '.svg')
    svg.write_text('\n'.join(parts) + '\n')
    subprocess.run(['rsvg-convert', str(svg), '-o', str(HERE / (stem + '.png'))], check=True)
    print(stem)

if __name__ == '__main__':
    for name in ('BASE', 'EXTRA'):
        render(name, 'tap')
        render(name, 'hold')
    for name in ('TAP', 'BUTTON', 'NAV', 'MOUSE', 'MEDIA', 'NUM', 'SYM', 'FUN'): render(name)
