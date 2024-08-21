#include QMK_KEYBOARD_H
#include "4x5_3_all_track.h"

#define _QWERTY 0
#define _MOUSE 1
#define _NAVIG 2
#define _SYMBO 3


#define MOUSE LT(_MOUSE, KC_TAB)
#define NAVIG LT(_NAVIG, KC_SPC)
#define SYMBO LT(_SYMBO, KC_ENT)

#define SFTBS MT(MOD_LSFT, KC_BSPC)
#define GUESC MT(MOD_LGUI, KC_ESC)

#define Z_CTRL MT(MOD_LCTL, KC_Z)
#define X_ALT MT(MOD_LALT, KC_X)


enum wyld_keycodes {
    WYLD_AUTO_MS_TOG = SAFE_RANGE
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_4x5(
            KC_F1, KC_F2,KC_F3,KC_F4,KC_F5,                        KC_F6,KC_F7,KC_F8,  KC_F9,  KC_F10,
            KC_Q,  KC_W, KC_E, KC_R, KC_T,                         KC_Y, KC_U, KC_I,   KC_O,   KC_P,
            KC_A,  KC_S, KC_D, KC_F, KC_G,                         KC_H, KC_J, KC_K,   KC_L,   KC_SCLN,
            Z_CTRL,X_ALT, KC_C, KC_V, KC_B,                        KC_N, KC_M, KC_COMM,KC_DOT ,KC_SLSH,
                                MOUSE,                                NAVIG,
                                    SFTBS,                      SYMBO,
                                        GUESC,            XXXXXXX
    ),

    [_MOUSE] = LAYOUT_4x5(
            QK_BOOT,_______,_______,_______,_______,                  _______,_______,_______,_______,QK_BOOT,
            S(KC_1),S(KC_2),S(KC_3),S(KC_4),S(KC_5),                  S(KC_6),S(KC_7),S(KC_8),S(KC_9),S(KC_0),
            _______,DRGSCRL,KC_MS_BTN2,KC_MS_BTN1,_______,            _______,S(KC_LBRC),S(KC_RBRC),_______,_______,
            _______,DRG_TOG,_______,_______,_______,                  _______,KC_LBRC,KC_RBRC,_______,_______,
                                          _______,                            _______,
                                             _______,                     _______,
                                                 _______,            _______
    ),

    [_NAVIG] = LAYOUT_4x5(
            QK_BOOT,_______,_______,_______,_______,                   _______,_______,_______,_______,QK_BOOT,
            KC_1,   KC_2,   KC_3,   KC_4,   KC_5,                       KC_6,   KC_7,   KC_8,  KC_9,   KC_0,
            KC_GRV, _______,_______,_______,_______,                    KC_LEFT,KC_DOWN,KC_UP  ,KC_RGHT,_______,
            _______,_______,_______,_______,_______,                    _______,KC_MPLY,KC_MNXT,_______,_______,
                                    _______,                                  _______,
                                          _______,                        _______,
                                                _______,              _______
    ),
    [_SYMBO] = LAYOUT_4x5(
            _______,_______,_______,_______,_______,                    _______,_______,_______,_______,_______,
            _______,_______,_______,_______,_______,                    _______,_______,_______,_______,_______,
            _______,_______,_______,_______,_______,                    S(KC_MINS),KC_MINS,KC_EQL,S(KC_QUOT),KC_QUOT,
            _______,_______,_______,_______,_______,                   S(KC_EQL),S(KC_BSLS),KC_BSLS,_______,_______,
                                     _______,                                _______,
                                          _______,                        _______,
                                                _______,              _______
    ),
};

#ifdef POINTING_DEVICE_COMBINED
#ifdef IGNORE_DANBOX
static void check_drag(report_mouse_t* mouse_report) {
    static int16_t scroll_buffer_x = 0;
    static int16_t scroll_buffer_y = 0;

#    ifdef CHARYBDIS_DRAGSCROLL_REVERSE_X
    scroll_buffer_x -= mouse_report->x;
#    else
    scroll_buffer_x += mouse_report->x;
#    endif  // CHARYBDIS_DRAGSCROLL_REVERSE_X
#    ifdef CHARYBDIS_DRAGSCROLL_REVERSE_Y
    scroll_buffer_y -= mouse_report->y;
#    else
    scroll_buffer_y += mouse_report->y;
#    endif  // CHARYBDIS_DRAGSCROLL_REVERSE_Y
    mouse_report->x = 0;
    mouse_report->y = 0;
    if (abs(scroll_buffer_x) > 6) {
        mouse_report->h = scroll_buffer_x > 0 ? 1 : -1;
        scroll_buffer_x = 0;
    }
    if (abs(scroll_buffer_y) > 6) {
        mouse_report->v = scroll_buffer_y > 0 ? 1 : -1;
        scroll_buffer_y = 0;
    }
}
#endif

report_mouse_t pointing_device_task_combined_user(report_mouse_t left_report, report_mouse_t right_report) {
    // check_drag(&left_report); // updated in trackball.c
    return pointing_device_combine_reports(left_report, right_report);
}
#endif


void pointing_device_init_user(void) {
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
    set_auto_mouse_enable(true);
#endif
#ifdef POINTING_DEVICE_COMBINED
    // pointing_device_set_cpi_on_side(true, CHARYBDIS_DRAGSCROLL_DPI); // LEFT
    // pointing_device_set_cpi_on_side(false, 2000);  // RIGHT
    pointing_device_set_cpi_on_side(true, 2000); // LEFT
    pointing_device_set_cpi_on_side(false, 2000);  // RIGHT
#endif// always required before the auto mouse feature will work
}

#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case WYLD_AUTO_MS_TOG:
            if (record->event.pressed) {
                set_auto_mouse_enable(!get_auto_mouse_enable());
            }
            return false; // Skip all further processing of this key
        default:
            return true; // Process all other keycodes normally
    }
}
#endif


#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_left()) {
        return OLED_ROTATION_270;
    }
    return OLED_ROTATION_0;
}

static const char PROGMEM wylderbuilds[] = {
        0,  0,  0,  0,  0,  0,240,252,252,252,248,224,128,  0,  0,  0,  0,  0,192,240,252,254,254,248,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 15, 63,255,255,255,255,255,124,252,252,252,252,255,255,191,255,255, 31,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31,255,255,255,255,255,255,255,231,199,199,199,247,255,255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  7, 31, 63, 63, 63, 63,127,127, 63, 63, 31, 31, 15,  7,  0,  0,  0,  0,  0,  0,  0
};


static void render_wylderbuilds(void) {
    oled_write_raw_P(wylderbuilds, sizeof(wylderbuilds));
}

bool oled_task_user(void) {
    oled_set_cursor(0, 3);
    if (is_keyboard_left()) {
//        oled_write_P(PSTR("Layer\n"), false);

        switch (get_highest_layer(layer_state)) {
            case _QWERTY:
                oled_write_P(PSTR(" BASE\n"), false);
                break;
            case _MOUSE:
                oled_write_P(PSTR("MOUSE\n"), false);
                break;
            case _NAVIG:
                oled_write_P(PSTR(" NAVI\n"), false);
                break;
            case _SYMBO:
                oled_write_P(PSTR(" SYMB\n"), false);
                break;
            default:
                // Or use the write_ln shortcut over adding '\n' to the end of your string
                oled_write_ln_P(PSTR("Undefined"), false);
        }
        oled_set_cursor(2, 7);
        oled_write_P(PSTR("WPM "), false);
        oled_set_cursor(1, 8);
        oled_write(get_u8_str(get_current_wpm(), ' '), false);
        // Host Keyboard LED Status

        oled_set_cursor(0, 9);
        led_t led_state = host_keyboard_led_state();

        oled_write_P(led_state.num_lock ? PSTR("NUMLK \n") : PSTR("    \n"), false);
        oled_write_P(led_state.caps_lock ? PSTR("CAPLK \n") : PSTR("    \n"), false);
        oled_write_P(led_state.scroll_lock ? PSTR("SCRLK \n") : PSTR("    \n"), false);
    } else {
        // write WPM to right OLED
        oled_set_cursor(0, 0);
        render_wylderbuilds();
        oled_scroll_left();
    }

    return false;
}
#endif

#if defined(ENCODER_MAP_ENABLE)
// TWO ENCODERS
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_QWERTY] =  { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_LOWER] =   { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN) },
    [_RAISE] =   { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN) },
    [_MOUSE] =   { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) }
};
#endif

#ifdef RGBLIGHT_ENABLE
const rgblight_segment_t PROGMEM querty_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {9, 3, HSV_RED},       // Light 4 LEDs, starting with LED 6
    {21, 3, HSV_RED}      // Light 4 LEDs, starting with LED 6
);
const rgblight_segment_t PROGMEM lower_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {9, 3, HSV_BLUE},
    {21, 3, HSV_BLUE}
);
const rgblight_segment_t PROGMEM raise_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {9, 3, HSV_YELLOW},
    {21, 3, HSV_YELLOW}
);

const rgblight_segment_t PROGMEM mouse_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {9, 3, HSV_GREEN},
    {21, 3, HSV_GREEN}
);

const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    querty_layer,    // Overrides caps lock layer
    lower_layer,    // Overrides other layers
    raise_layer,     // Overrides other layers
    mouse_layer      // Overrides other layers
);

//bool led_update_user(led_t led_state) {
//    rgblight_set_layer_state(0, led_state.caps_lock);
//    return true;
//}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, _QWERTY));
    return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(1, layer_state_cmp(state, _MOUSE));
    rgblight_set_layer_state(2, layer_state_cmp(state, _NAVIG));
    return state;
}


#endif

//    rgblight_enable();


void keyboard_post_init_user(void) {
#ifdef RGBLIGHT_ENABLE
    rgblight_layers = my_rgb_layers;
//    rgblight_layers = my_rgb_layers;
//    rgblight_enable_noeeprom(); // Enables RGB, without saving settings
//    rgblight_sethsv_noeeprom_cyan();
//    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
#endif

#ifdef CONSOLE_ENABLE
    debug_enable=true;
    debug_matrix=true;
    debug_keyboard=true;
    debug_mouse=true;
#else
    debug_enable=false;
    debug_matrix=false;
    debug_keyboard=false;
    debug_mouse=false;
#endif
}


