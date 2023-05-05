/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>
   Copyright 2021 Andre Brait <andrebrait@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#ifdef RGB_MATRIX_ENABLE
    #ifndef RGB_CONFIRMATION_BLINKING_TIME
        #define RGB_CONFIRMATION_BLINKING_TIME 2000 // 2 seconds
    #endif
    #if RGB_CONFIRMATION_BLINKING_TIME > 0
        #include <lib/lib8tion/lib8tion.h>
    #endif // RGB_CONFIRMATION_BLINKING_TIME > 0
#endif // RGB_MATRIX_ENABLE

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12	     Del          Rotary(Play/Pause)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)	     BackSpc           Home
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]        \                 PgUp
//      Caps     A        S        D        F        G        H        J        K        L        ;        "                 Enter             PgDn
//      Sh_L              Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right


    // The FN key by default maps to a momentary toggle to layer 1 to provide access to the RESET key (to put the board into bootloader mode). Without
    // this mapping, you have to open the case to hit the button on the bottom of the PCB (near the USB cable attachment) while plugging in the USB
    // cable to get the board into bootloader mode - definitely not fun when you're working on your QMK builds. Remove this and put it back to KC_RGUI
    // if that's your preference.
    //
    // To put the keyboard in bootloader mode, use FN+backslash. If you accidentally put it into bootloader, you can just unplug the USB cable and
    // it'll be back to normal when you plug it back in.
    //
    // This keyboard defaults to 6KRO instead of NKRO for compatibility reasons (some KVMs and BIOSes are incompatible with NKRO).
    // Since this is, among other things, a "gaming" keyboard, a key combination to enable NKRO on the fly is provided for convenience.
    // Press Fn+N to toggle between 6KRO and NKRO. This setting is persisted to the EEPROM and thus persists between restarts.
    [0] = LAYOUT(
        KC_ESC,          KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_F13,           C(KC_F15),
        KC_GRV,          KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_DEL,
        LCTL_T(KC_TAB),  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_PGUP,
        MO(1),           KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGDN,
        KC_LSFT,                  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_END,
        KC_LCTL,         KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [1] = LAYOUT(
        LCA(KC_Q), KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,  _______,          RESET,
        _______,   RGB_TOG, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,           _______,
        _______,   _______, KC_MPLY, KC_MPRV, KC_MNXT, _______, KC_INS,  _______, KC_UP,   KC_PAUS, KC_PGUP, KC_HOME, KC_PSCR, _______,          _______,
        _______,   _______, KC_MUTE, KC_VOLD, KC_VOLU, _______, KC_SLCK, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, KC_END,           _______,          _______,
        _______,            _______, RGB_HUI, _______, _______, _______, NK_TOGG, _______, _______, _______, _______,          KC_CAPS, RGB_MOD, _______,
        _______,   _______, _______,                            _______,                            _______, _______, _______, RGB_SPD, RGB_RMOD, RGB_SPI
    ),
    // Missing: RGB_VAI, RGB_VAD, RGB_HUI, NK_TOGG, RGB_MOD, RGB_RMOD, RGB_SPD, RGP_SPI


};
// clang-format on

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
  if (index == 0) {
    switch(biton32(layer_state)){
        case 1:
            if (clockwise){
                tap_code16(KC_PGDN);
            } else{
                tap_code16(KC_PGUP);
            }
            break;
        default:
            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
            break;
        }
    }
    return true;
}
#endif // ENCODER_ENABLE

#ifdef RGB_MATRIX_ENABLE

static void set_layerleds_color(int layer);
static void set_rgb_caps_leds(void);

#if RGB_CONFIRMATION_BLINKING_TIME > 0
static uint16_t effect_started_time = 0;
static uint8_t r_effect = 0x0, g_effect = 0x0, b_effect = 0x0;
static void start_effects(void);

#define effect_red() r_effect = 0xFF, g_effect = 0x0, b_effect = 0x0
#define effect_green() r_effect = 0x0, g_effect = 0xFF, b_effect = 0x0
#endif // RGB_CONFIRMATION_BLINKING_TIME > 0

bool led_update_user(led_t led_state) {
    if (led_state.caps_lock) {
        if (!rgb_matrix_is_enabled()) {
            /* Turn ON the RGB Matrix for CAPS LOCK */
            rgb_matrix_set_flags(LED_FLAG_NONE);
            rgb_matrix_enable();
        }
    } else if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
        /* RGB Matrix was only ON because of CAPS LOCK. Turn it OFF. */
        rgb_matrix_set_flags(LED_FLAG_ALL);
        rgb_matrix_disable();
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    #ifdef NKRO_ENABLE
    #if RGB_CONFIRMATION_BLINKING_TIME > 0
        case NK_TOGG:
            if (record->event.pressed) {
                if (keymap_config.nkro) {
                    /* Turning NKRO OFF */
                    effect_red();
                } else {
                    /* Turning NKRO ON */
                    effect_green();
                }
                start_effects();
            }
            break;
        case NK_ON:
            if (record->event.pressed) {
                if (!keymap_config.nkro) {
                    /* Turning NKRO ON */
                    effect_green();
                    start_effects();
                }
            }
            break;
        case NK_OFF:
            if (record->event.pressed) {
                if (keymap_config.nkro) {
                    /* Turning NKRO OFF */
                    effect_red();
                    start_effects();
                }
            }
            break;
    #endif // RGB_CONFIRMATION_BLINKING_TIME > 0
    #endif // NKRO_ENABLE
        case RGB_MOD:
        case RGB_RMOD:
        case RGB_HUI:
        case RGB_HUD:
        case RGB_SAI:
        case RGB_SAD:
        case RGB_VAI:
        case RGB_VAD:
        case RGB_SPI:
        case RGB_SPD:
            if (record->event.pressed) {
                if (rgb_matrix_get_flags() != LED_FLAG_ALL) {
                    /* Ignore changes to RGB settings while only it's supposed to be OFF */
                    return false;
                }
            }
            break;
        case RGB_TOG:
            if (record->event.pressed) {
                if (rgb_matrix_is_enabled()) {
                    switch (rgb_matrix_get_flags()) {
                        #if RGB_CONFIRMATION_BLINKING_TIME > 0
                        case LED_FLAG_INDICATOR:
                        #endif
                        case LED_FLAG_NONE:
                            /* Turned ON because of EFFECTS or CAPS, is actually OFF */
                            /* Change to LED_FLAG_ALL to signal it's really ON */
                            rgb_matrix_set_flags(LED_FLAG_ALL);
                            /* Will be re-enabled by the processing of the toggle */
                            rgb_matrix_disable_noeeprom();
                            break;
                        case LED_FLAG_ALL:
                            /* Is actually ON */
                            #if RGB_CONFIRMATION_BLINKING_TIME > 0
                            if (effect_started_time > 0) {
                                /* Change to LED_FLAG_INDICATOR to signal EFFECTS */
                                rgb_matrix_set_flags(LED_FLAG_INDICATOR);
                                /* Will be re-enabled by the processing of the toggle */
                                rgb_matrix_disable_noeeprom();
                            } else
                            #endif
                            if (host_keyboard_led_state().caps_lock) {
                                /* Change to LED_FLAG_NONE to signal CAPS */
                                rgb_matrix_set_flags(LED_FLAG_NONE);
                                /* Will be re-enabled by the processing of the toggle */
                                rgb_matrix_disable_noeeprom();
                            }
                            break;
                    }
                }
            }
            break;
    }
    return true;
}

// RGB LED layout
// led number, function of the key

//  67, Side led 01    0, ESC      6, F1       12, F2       18, F3       23, F4       28, F5       34, F6       39, F7       44, F8       50, F9       56, F10      61, F11      66, F12      69, Prt       Rotary(Mute)   68, Side led 12
//  70, Side led 02    1, ~        7, 1        13, 2        19, 3        24, 4        29, 5        35, 6        40, 7        45, 8        51, 9        57, 0        62, -_       78, (=+)     85, BackSpc   72, Del        71, Side led 13
//  73, Side led 03    2, Tab      8, Q        14, W        20. E        25, R        30, T        36, Y        41, U        46, I        52, O        58, P        63, [{       89, ]}       93, \|        75, PgUp       74, Side led 14
//  76, Side led 04    3, Caps     9, A        15, S        21, D        26, F        31, G        37, H        42, J        47, K        53, L        59, ;:       64, '"                    96, Enter     86, PgDn       77, Side led 15
//  80, Side led 05    4, Sh_L     10, Z       16, X        22, C        27, V        32, B        38, N        43, M        48, ,<       54, .<       60, /?                    90, Sh_R     94, Up        82, End        81, Side led 16
//  83, Side led 06    5, Ct_L     11,Win_L    17, Alt_L                              33, SPACE                              49, Alt_R    55, FN                    65, Ct_R     95, Left     97, Down      79, Right      84, Side led 17
//  87, Side led 07                                                                                                                                                                                                        88, Side led 18
//  91, Side led 08                                                                                                                                                                                                        92, Side led 19

static uint16_t key_timer;
const uint8_t PROGMEM layerleds[] = {
    42, 46, 47, 53 // J, I, K, L
};

void matrix_init_user(void) {
    key_timer = timer_read();
}

void rgb_matrix_indicators_user() {
    #if RGB_CONFIRMATION_BLINKING_TIME > 0
    if (effect_started_time != 0) {
        /* Render blinking EFFECTS */
        uint16_t deltaTime = sync_timer_elapsed(effect_started_time);
        if (deltaTime <= RGB_CONFIRMATION_BLINKING_TIME) {
            uint16_t time = scale16by8(deltaTime, UINT8_MAX / 2);
            uint8_t led_state = sin8(time) >> 7;
            uint8_t val_r = led_state * r_effect;
            uint8_t val_g = led_state * g_effect;
            uint8_t val_b = led_state * b_effect;
            rgb_matrix_set_color_all(val_r, val_g, val_b);
            if (host_keyboard_led_state().caps_lock) {
                set_rgb_caps_leds();
            }
            return;
        } else {
            /* EFFECTS duration is finished */
            effect_started_time = 0;
            if (rgb_matrix_get_flags() == LED_FLAG_INDICATOR) {
                /* It was turned ON because of EFFECTS */
                if (host_keyboard_led_state().caps_lock) {
                    /* CAPS is still ON. Demote to CAPS */
                    rgb_matrix_set_flags(LED_FLAG_NONE);
                } else {
                    /* There is nothing else keeping RGB enabled. Reset flags and turn if off. */
                    rgb_matrix_set_flags(LED_FLAG_ALL);
                    rgb_matrix_disable_noeeprom();
                }
            }
        }
    }
    #endif // RGB_CONFIRMATION_BLINKING_TIME > 0
    if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
        rgb_matrix_set_color_all(0x0, 0x0, 0x0);
    }
    if (host_keyboard_led_state().caps_lock) {
        set_rgb_caps_leds();
    }
    uint8_t layer = biton32(layer_state);
    set_layerleds_color(layer);
}

#if RGB_CONFIRMATION_BLINKING_TIME > 0
static void start_effects() {
    effect_started_time = sync_timer_read();
    if (!rgb_matrix_is_enabled()) {
        /* Turn it ON, signal the cause (EFFECTS) */
        rgb_matrix_set_flags(LED_FLAG_INDICATOR);
        rgb_matrix_enable_noeeprom();
    } else if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
        /* It's already ON, promote the cause from CAPS to EFFECTS */
        rgb_matrix_set_flags(LED_FLAG_INDICATOR);
    }
}
#endif // RGB_CONFIRMATION_BLINKING_TIME > 0

void set_layerleds_color(int layer) {
    uint16_t time = timer_elapsed(key_timer);
    uint16_t hval;

    if (layer > 0) {
        hval = time / 6;
        HSV hsv = {.h = hval, .s = 255, .v = 255};
        RGB rgb = hsv_to_rgb(hsv);

        rgb_matrix_set_color(3, rgb.r, rgb.g, rgb.b); // CAPS
        rgb_matrix_set_color(55, rgb.r, rgb.g, rgb.b); // FN
        for (int i = 0; i < 4; i++) {
            uint8_t val = pgm_read_byte(&layerleds[i]);
            rgb_matrix_set_color(val, rgb.r, rgb.g, rgb.b);
        }
    } else {
        hval = time / 32;
        HSV hsv = {.h = hval, .s = 255, .v = 255};
        RGB rgb = hsv_to_rgb(hsv);

        rgb_matrix_set_color(3, rgb.r, rgb.g, rgb.b); // CAPS
        rgb_matrix_set_color(55, rgb.r, rgb.g, rgb.b); // FN
    }
}

static void set_rgb_caps_leds() {
    rgb_matrix_set_color(67, 0xFF, 0x0, 0x0); // Left side LED 1
    rgb_matrix_set_color(68, 0xFF, 0x0, 0x0); // Right side LED 1
    rgb_matrix_set_color(70, 0xFF, 0x0, 0x0); // Left side LED 2
    rgb_matrix_set_color(71, 0xFF, 0x0, 0x0); // Right side LED 2
    rgb_matrix_set_color(73, 0xFF, 0x0, 0x0); // Left side LED 3
    rgb_matrix_set_color(74, 0xFF, 0x0, 0x0); // Right side LED 3
    rgb_matrix_set_color(76, 0xFF, 0x0, 0x0); // Left side LED 4
    rgb_matrix_set_color(77, 0xFF, 0x0, 0x0); // Right side LED 4
    rgb_matrix_set_color(80, 0xFF, 0x0, 0x0); // Left side LED 5
    rgb_matrix_set_color(81, 0xFF, 0x0, 0x0); // Right side LED 5
    rgb_matrix_set_color(83, 0xFF, 0x0, 0x0); // Left side LED 6
    rgb_matrix_set_color(84, 0xFF, 0x0, 0x0); // Right side LED 6
    rgb_matrix_set_color(87, 0xFF, 0x0, 0x0); // Left side LED 7
    rgb_matrix_set_color(88, 0xFF, 0x0, 0x0); // Right side LED 7
    rgb_matrix_set_color(91, 0xFF, 0x0, 0x0); // Left side LED 8
    rgb_matrix_set_color(92, 0xFF, 0x0, 0x0); // Right side LED 8
    rgb_matrix_set_color(3, 0xFF, 0x0, 0x0); // CAPS LED
}

#endif // RGB_MATRIX_ENABLE
