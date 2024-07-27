/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

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
#include <stdio.h>
#include "features/casemodes.h"
#include "features/leader.h"


int workmode = 0;
int keycount = 0;
int current_layer = 0;

bool SHIFTED;
bool PARAGRAPH_MODE;
static uint16_t p1_timer;
//static uint16_t p2_timer;
uint16_t shifted_keycode;

//static uint16_t copypasta_timer;

enum tap_dance_codes {
  DANCE_0, // ESC
  DANCE_1, // TAB
  DANCE_2, // Copy Pasta
  DANCE_3, // Layers
  DANCE_4, // numpad
  DANCE_5, // numpad
  DANCE_6, // numpad
  DANCE_7, // try space/layer since it gets stuck
  DANCE_8,
  DANCE_9,
  DANCE_10,  // numpad
};


enum preonic_keycodes {
  QWERTY = SAFE_RANGE,
  COLEMAK,
  DVORAK,
  LOWER,
  RAISE,
  BACKLIT,
  ST_MACRO_0,
  ST_MACRO_1,
  ST_MACRO_2,
  ST_MACRO_3,
  ST_MACRO_4,
  L7_MACRO_1,
  L7_MACRO_2,
  L7_MACRO_3,
  L7_MACRO_4,
  L7_MACRO_5,
  L7_MACRO_6,
  L7_MACRO_7,
  L7_MACRO_8,
  L7_MACRO_9,
  L7_MACRO_10,
  L7_MACRO_11,
  MRO_SPC_PERIOD,
  MRO_SPC_COMMA,
  MRO_SPC_EX,
  MRO_SPC_QUESTION,
  MRO_SPC_MINUS,
  MRO_COPY_PASTA,
  MRO_BACKSPACE_WORD,
  MRO_ALTTAB,
  BS2,
  BS3,
  BS4,
  BS5,
  BS6,
  LEADER

};


#define L_COLEMAK 0
#define L_NAVIGATION 1
#define L_SYMBOL 2
#define L_NUMPAD_RIGHT 3


#ifdef SWAP_HANDS_ENABLE
__attribute__ ((weak))
const keypos_t PROGMEM hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
	// Left
	{{0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4}},
	{{0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}},
	{{0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}},
	{{0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}},
	// Right
	{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}},
	{{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}},
	{{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}},
	{{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3}}
};
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3(
 //,---------------------------------------------------------------------------------------------------------------.      ,----------------------------------------------------------------------------------------------------------------.
      LT(8,KC_ESC),        LT(6,KC_Q),         KC_W,                KC_F,                KC_P,         KC_B,                         KC_J,            KC_L,           KC_U,                   KC_Y,          XXXXXXX,          LEADER,
 //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
      LT(7,KC_TAB),   MT(MOD_LCTL,KC_A), MT(MOD_LALT,KC_R), MT(MOD_LGUI,KC_S), MT(MOD_LSFT,KC_T), MT(MOD_LGUI,KC_G),        MT(MOD_RGUI,KC_M), MT(MOD_RSFT,KC_N), MT(MOD_RGUI,KC_E), MT(MOD_RALT,KC_I), MT(MOD_RCTL,KC_O), LT(5,KC_BSPACE),
 //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
      KC_LSFT,          MT(MOD_LSFT,KC_Z),       KC_X,             KC_C,         MT(MOD_MEH,KC_D),  MT(MOD_HYPR,KC_V),              KC_K,              KC_H,              KC_COMMA,          KC_DOT,        KC_SLASH,        KC_RSFT,
 //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
                                                                   TT(9),         MO(1),           LT(3,KC_SPC),               LT(3,KC_SPC),     MO(2),              XXXXXXX
                                                        //`--------------------------------------------------------'      `--------------------------------------------------------'
  ),
  [1] = LAYOUT_split_3x6_3(
 //,---------------------------------------------------------------------------------------------------------------.      ,----------------------------------------------------------------------------------------------------------------.
       TO(0),             KC_MS_WH_UP,      KC_MS_BTN2,       KC_MS_BTN3,       KC_MS_BTN1,        KC_MS_WH_DOWN,               XXXXXXX,            KC_PGUP,           KC_UP,          KC_PGDOWN,           XXXXXXX,          TO(0),
 //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
       KC_TAB,           OSM(MOD_LCTL),    OSM(MOD_LALT),    OSM(MOD_LGUI),    TD(DANCE_2),       OSM(MOD_LGUI),               KC_HOME,            KC_LEFT,          KC_DOWN,        KC_RIGHT,           LT(3,KC_END),      XXXXXXX,
 //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
       OSM(MOD_LSFT),    XXXXXXX,           XXXXXXX,          XXXXXXX,          OSM(MOD_MEH),      OSM(MOD_HYPR),              XXXXXXX,             MRO_SPC_EX,     MRO_SPC_COMMA,    MRO_SPC_PERIOD,  MRO_SPC_QUESTION,      _______,
 //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
                                                              _______,          _______,             KC_ENTER,                  KC_ENTER,          MO(4),            _______
                                                        //`--------------------------------------------------------'      `--------------------------------------------------------'
  ),
  [2] = LAYOUT_split_3x6_3(
 // //,---------------------------------------------------------------------------------------------------------------.      ,----------------------------------------------------------------------------------------------------------------.
 //        TO(0),             KC_GRAVE,      KC_AMPR,           KC_LCBR,             KC_RCBR,          XXXXXXX,                    XXXXXXX,            KC_PIPE,        KC_BSLASH,           KC_PERC,            XXXXXXX,           TO(0),
 // //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
 //        _______,             KC_TILD,      KC_EQUAL,          KC_LPRN,              KC_RPRN,        KC_HASH,                     KC_AT,          KC_DQUO,           KC_MINUS,           KC_COLN,          KC_CIRC,       KC_DELETE,
 // //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
 //        _______,             KC_EXLM,      KC_PLUS,          KC_LBRACKET,        KC_RBRACKET,      KC_ASTR,                     KC_DLR,         KC_QUOTE,           KC_UNDS,            KC_SCOLON,       KC_QUESTION,        _______,
 // //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
 //                                                                   _______,        MO(4),           KC_ENTER,                   KC_ENTER,         _______,           _______
 //                                                        //`--------------------------------------------------------'      `--------------------------------------------------------'

 //,---------------------------------------------------------------------------------------------------------------.      ,----------------------------------------------------------------------------------------------------------------.
        TO(0),             KC_GRAVE,        KC_LCBR,            KC_BSLASH,           XXXXXXX,         KC_RCBR,                   KC_AMPR,         KC_DQUO,          KC_PLUS ,           KC_EQUAL,          KC_CIRC,         TO(0),
 //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
        _______,           KC_TILD,        KC_LPRN,             KC_PIPE,             KC_HASH,        KC_RPRN,                     KC_AT,          KC_QUOTE,          KC_MINUS,           KC_COLN,          KC_SLASH,       KC_DELETE,
 //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
        _______,           KC_EXLM,        KC_LBRACKET,      KC_UNDS,              XXXXXXX,       KC_RBRACKET,                   KC_DLR,           KC_PERC,         KC_ASTR,            KC_SCOLON,        KC_QUESTION,        _______,
 //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
                                                                   _______,        MO(4),           KC_ENTER,                   KC_ENTER,         _______,           _______
                                                        //`--------------------------------------------------------'      `--------------------------------------------------------'

  ),
  [3] = LAYOUT_split_3x6_3(
// //,---------------------------------------------------------------------------------------------------------------.      ,----------------------------------------------------------------------------------------------------------------.
//             TO(0),          KC_ASTR,            KC_9,           KC_8,               KC_7,           TD(DANCE_6),               KC_DOT,             KC_7,             KC_8,               KC_9,             KC_ASTR,          TO(0),
// //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
//           KC_EQUAL,        KC_0,            KC_6,            KC_5,               KC_4,           TD(DANCE_5),               TD(DANCE_5),           KC_4,             KC_5,                KC_6,                KC_0,         KC_EQUAL,
// //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
//           _______,        KC_SLASH,            KC_3,           KC_2,                KC_1,          TD(DANCE_4),                TD(DANCE_4),         KC_1,             KC_2,               KC_3,             KC_SLASH,         _______,
// //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
//                                                                XXXXXXX,             XXXXXXX,             _______,                  _______,            XXXXXXX,             XXXXXXX
//                                                        //`--------------------------------------------------------'      `--------------------------------------------------------'
//                                                                
 //,---------------------------------------------------------------------------------------------------------------.      ,----------------------------------------------------------------------------------------------------------------.
             TO(0),          KC_ASTR,            KC_9,           KC_8,               KC_7,           TD(DANCE_5),               TD(DANCE_5),             KC_7,             KC_8,               KC_9,             KC_ASTR,          TO(0),
 //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
           KC_EQUAL,        KC_0,            KC_6,            KC_5,               KC_4,           TD(DANCE_4),               TD(DANCE_4),           KC_4,             KC_5,                KC_6,                KC_0,         KC_EQUAL,
 //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
           _______,        KC_SLASH,            KC_3,           KC_2,                KC_1,          TD(DANCE_6),                TD(DANCE_6),         KC_1,             KC_2,               KC_3,             KC_SLASH,         _______,
 //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
                                                                XXXXXXX,             XXXXXXX,             _______,                  _______,            XXXXXXX,             XXXXXXX
                                                        //`--------------------------------------------------------'      `--------------------------------------------------------'

  ),
  [4] = LAYOUT_split_3x6_3(
 //,---------------------------------------------------------------------------------------------------------------.      ,----------------------------------------------------------------------------------------------------------------.
        TO(0),           TO(1),             TO(2),              TO(3),            TO(7),            TO(9),                           TO(3),        _______,         _______,           _______,          _______,           TO(0),
 //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
       _______,           _______,          KC_MS_BTN2,      KC_MS_BTN3,         KC_MS_BTN1,         _______,                  _______,       KC_AUDIO_VOL_UP,   KC_MEDIA_PLAY_PAUSE,   _______,        _______,            _______,
 //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
       _______,           _______,             _______,       KC_MS_WH_UP,      KC_MS_WH_DOWN,       _______,                  _______,      KC_AUDIO_VOL_DOWN, KC_MEDIA_PREV_TRACK,KC_MEDIA_NEXT_TRACK,KC_MEDIA_PLAY_PAUSE,  _______,
 //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
                                                                 _______,            _______,          _______,                 _______,          _______,           _______
                                                        //`--------------------------------------------------------'      `--------------------------------------------------------'
  ),
  [5] = LAYOUT_split_3x6_3(
  //,---------------------------------------------------------------------------------------------------------------.      ,----------------------------------------------------------------------------------------------------------------.
           TO(0),            _______,           _______,            _______,         _______,            _______,                  _______,          _______,          _______,            _______,          _______,          TO(0),
  //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
          _______,           _______,           _______,            _______,         _______,            _______,                       BS6,              BS5,            BS4,                  BS3,          _______,          _______,
  //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
            _______,           _______,           _______,            _______,         _______,            _______,                  _______,          _______,          _______,            _______,          _______,          _______,
  //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
                                                               _______,            _______,          _______,                 _______,          _______,           _______
                                                        //`--------------------------------------------------------'      `--------------------------------------------------------'
  ),
  [6] = LAYOUT_split_3x6_3(
  //,---------------------------------------------------------------------------------------------------------------.      ,----------------------------------------------------------------------------------------------------------------.
           TO(0),         _______,           LGUI(KC_BSLASH),        _______,      _______,            LCTL(KC_UP),             _______,            _______,            _______,           _______,           RESET,          TO(0),
  //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
           _______,         _______,           _______,             _______,       _______,            _______,                _______,          _______,            _______,           _______,           _______,          _______,
  //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
           _______,         _______,           _______,             _______,      _______,            _______,                    _______,            _______,            _______,           _______,           _______,          _______,
  //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
                                                                 _______,         _______,           TO(9),                       _______,            _______,          _______
                                                         //`--------------------------------------------------------'      `--------------------------------------------------------'

  ),
  [7] = LAYOUT_split_3x6_3(
    //,---------------------------------------------------------------------------------------------------------------.      ,----------------------------------------------------------------------------------------------------------------.
              TO(0),         L7_MACRO_11,        XXXXXXX,          XXXXXXX,        XXXXXXX,             XXXXXXX,                    KC_F1,              KC_F2,          KC_F3,            KC_F4,             KC_F5,           TO(0),
    //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
              _______,        L7_MACRO_1,      L7_MACRO_2,       L7_MACRO_3,       L7_MACRO_4,         L7_MACRO_5,                  KC_F6,              KC_F7,          KC_F8,            KC_F9,             KC_F10,           _______,
    //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
              _______,         L7_MACRO_6,     L7_MACRO_7,      L7_MACRO_8,        L7_MACRO_9,        L7_MACRO_10,                  KC_F11,              KC_F12,          KC_F13,            KC_F14,             KC_F15,           _______,
    //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
                                                                   _______,           _______,          _______,                    _______,          _______,          _______
                                                           //`--------------------------------------------------------'      `--------------------------------------------------------'
  ),
  [8] = LAYOUT_split_3x6_3(
    //,---------------------------------------------------------------------------------------------------------------.      ,----------------------------------------------------------------------------------------------------------------.
             TO(0),         L7_MACRO_11,     ST_MACRO_2,          ST_MACRO_3,         _______,      LCTL(LGUI(KC_Q)),               _______,          _______,          _______,            _______,          _______,          TO(0),
    //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
            _______,            RGB_TOG,           _______,            _______,         _______,            _______,                  _______,          _______,          _______,            _______,          _______,          _______,
    //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
              _______,           RGB_MOD,           _______,            _______,         _______,            _______,                  _______,          _______,          _______,            _______,          _______,          _______,
    //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
                                                                 _______,            _______,          _______,                 _______,          _______,           _______
                                                          //`--------------------------------------------------------'      `--------------------------------------------------------'
  ),
  [9] = LAYOUT_split_3x6_3(
  //,---------------------------------------------------------------------------------------------------------------.      ,----------------------------------------------------------------------------------------------------------------.
        TO(0),           OSM(MOD_LALT),   TD(DANCE_2),        KC_MS_WH_UP,    XXXXXXX,            XXXXXXX,                   XXXXXXX,            XXXXXXX,         XXXXXXX,          XXXXXXX,              XXXXXXX,         TO(0),
  //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
        KC_TAB,           OSM(MOD_LCTL),    KC_MS_BTN2,         KC_MS_BTN3,      KC_MS_BTN1,         TD(DANCE_2),                XXXXXXX,            XXXXXXX,         MRO_SPC_MINUS,      XXXXXXX,            XXXXXXX,         _______,
  //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
       OSM(MOD_LSFT),    OSM(MOD_LGUI),      XXXXXXX,        KC_MS_WH_DOWN,      XXXXXXX,           XXXXXXX,                  XXXXXXX,            MRO_SPC_EX,     MRO_SPC_COMMA,    MRO_SPC_PERIOD,  MRO_SPC_QUESTION,      _______,
  //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
                                                               _______,           _______,          _______,                    _______,          _______,          _______
                                                         //`--------------------------------------------------------'      `--------------------------------------------------------'
  ),
  [10] = LAYOUT_split_3x6_3(
    //,---------------------------------------------------------------------------------------------------------------.      ,----------------------------------------------------------------------------------------------------------------.
             TO(0),            XXXXXXX,           XXXXXXX,            XXXXXXX,         XXXXXXX,            XXXXXXX,                  XXXXXXX,          XXXXXXX,          XXXXXXX,            XXXXXXX,          XXXXXXX,          TO(0),
    //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
            _______,           XXXXXXX,           XXXXXXX,            XXXXXXX,         XXXXXXX,            XXXXXXX,                  XXXXXXX,          XXXXXXX,          XXXXXXX,            XXXXXXX,          XXXXXXX,          _______,
    //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
              _______,           XXXXXXX,           XXXXXXX,            XXXXXXX,         XXXXXXX,            XXXXXXX,                  XXXXXXX,          XXXXXXX,          XXXXXXX,            XXXXXXX,          XXXXXXX,          _______,
    //|-----------------+-----------------+------------------+------------------+------------------+------------------|      |------------------+------------------+------------------+------------------+-----------------+------------------|
                                                                 _______,            _______,          _______,                 _______,          _______,           _______
                                                          //`--------------------------------------------------------'      `--------------------------------------------------------'
  )
};

char keylog_str[24] = {};

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

void count_chars(uint16_t keycode, keyrecord_t *record) {
  //char name = ' ';
    if (keycount > 50) {keycount = 0;}
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
        (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) {
          keycode = keycode & 0xFF; }

    if (keycode < 40 && keycode > 3)  {
        keycount++;
    } else {
        keycount = 0;
    }
}


#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master()) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  }
  return rotation;
}

void oled_render_layer_state(void) {
    oled_write_P(PSTR("Layer: "), false);
    switch (current_layer) {
        case 0:
            oled_write_ln_P(PSTR("Colemak-DHm"), false);
            break;
        case 1:
            oled_write_ln_P(PSTR("Navigation"), false);
            break;
        case 2:
            oled_write_ln_P(PSTR("Symbols"), false);
            break;
        case 3:
            oled_write_ln_P(PSTR("Numbers"), false);
            break;
        case 4:
            oled_write_ln_P(PSTR("Media"), false);
            break;
        case 7:
            oled_write_ln_P(PSTR("Function"), false);
            break;
        case 9:
            oled_write_ln_P(PSTR("Space"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Other"), false);
            break;
    }
}

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  char name = ' ';
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
        (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { keycode = keycode & 0xFF; }
  if (keycode < 60) {
    name = code_to_name[keycode];
  }

  // update keylog
  snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c",
           record->event.key.row, record->event.key.col,
           keycode, name);
}


void oled_render_keylog(void) {
    oled_write_ln(keylog_str, false);
}

void oled_render_workmode(void) {
  oled_write_P(PSTR("Mode: "), false);
  if (workmode){
    oled_write_ln_P(PSTR("Work"), false);
  } else {
    oled_write_ln_P(PSTR("Home"), false);
  }
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, false);
}

void oled_task_user(void) {
  if (is_oled_on()){
    if (is_keyboard_master()) {
      oled_render_layer_state();
      oled_render_workmode();
      //oled_render_keylog();
    } else {
      oled_render_logo();

    }
  }
}

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//   if (record->event.pressed) {
//     set_keylog(keycode, record);
//   }
//   return true;
// }
#endif // OLED_DRIVER_ENABLE


void keyboard_post_init_user(void) {
//  debug_enable=true;
//  debug_matrix=true;
  set_single_persistent_default_layer(0);
#ifdef RGBLIGHT_ENABLE
  //rgblight_sethsv_noeeprom(0, 0, 255);
  rgblight_setrgb (RGB_WHITE);
#endif
#ifdef RGB_MATRIX_ENABLE
  rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
  rgb_matrix_set_color_all(RGB_WHITE); 
#endif

}

#ifdef RGB_MATRIX_ENABLE

void matrix_init_user(void) {
//    #ifdef RGBLIGHT_ENABLE 
//      RGB_current_mode = rgblight_config.mode;
//    #endif
    rgb_matrix_set_color_all(RGB_WHITE); 
}
#endif

#ifdef RGB_MATRIX_ENABLE
void rgb_matrix_indicators_user(void) {
    switch (current_layer) {
      case 1:
         rgb_matrix_set_color_all(RGB_SPRINGGREEN);
         break;
      case 2:
          rgb_matrix_set_color_all(RGB_BLUE);
          break;
      case 3:
          rgb_matrix_set_color_all(RGB_PURPLE);
          break;
      case 4:
          rgb_matrix_set_color_all(RGB_GREEN);
          break;
      case 5:
          rgb_matrix_set_color_all(RGB_YELLOW);
          break;
      case 6:
          rgb_matrix_set_color_all(RGB_YELLOW);
          break;
      case 7:
          rgb_matrix_set_color_all(RGB_YELLOW);
          break;
      case 8:
          rgb_matrix_set_color_all(RGB_YELLOW);
          break;
      case 9:
          rgb_matrix_set_color_all(RGB_ORANGE);
          break;
      case 10:
          rgb_matrix_set_color_all(RGB_ORANGE);
          break;
        default: // for any other layers, or the default layer
            if (workmode == 0) {
              rgb_matrix_set_color_all(RGB_WHITE);
              break;
            } else {
              rgb_matrix_set_color_all(RGB_RED);
              break;
            }
    }
}
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
     current_layer = get_highest_layer(state);

#ifdef RGBLIGHT_ENABLE
    switch (get_highest_layer(state)) {
      case 1:
         rgblight_setrgb(RGB_SPRINGGREEN);
         break;
      case 2:
          rgblight_setrgb(RGB_BLUE);
          break;
      case 3:
          rgblight_setrgb(RGB_PURPLE);
          break;
      case 4:
          rgblight_setrgb(RGB_GREEN);
          break;
      case 5:
          rgblight_setrgb(RGB_YELLOW);
          break;
      case 6:
          rgblight_setrgb(RGB_YELLOW);
          break;
      case 7:
          rgblight_setrgb(RGB_YELLOW);
          break;
      case 8:
          rgblight_setrgb(RGB_YELLOW);
          break;
      case 9:
          rgblight_setrgb(RGB_ORANGE);
          break;
      case 10:
          rgblight_setrgb(RGB_ORANGE);
          break;
        default: // for any other layers, or the default layer
            if (workmode == 0) {
              rgblight_setrgb (RGB_WHITE);
              break;
            } else {
              rgblight_setrgb (RGB_RED);
              break;
            }
    }
#endif
// #ifdef RGB_MATRIX_ENABLE
//     switch (get_highest_layer(state)) {
//       case 1:
//          rgb_matrix_set_color_all(RGB_SPRINGGREEN);
//          break;
//       case 2:
//           rgb_matrix_set_color_all(RGB_BLUE);
//           break;
//       case 3:
//           rgb_matrix_set_color_all(RGB_PURPLE);
//           break;
//       case 4:
//           rgb_matrix_set_color_all(RGB_GREEN);
//           break;
//       case 5:
//           rgb_matrix_set_color_all(RGB_YELLOW);
//           break;
//       case 6:
//           rgb_matrix_set_color_all(RGB_YELLOW);
//           break;
//       case 7:
//           rgb_matrix_set_color_all(RGB_YELLOW);
//           break;
//       case 8:
//           rgb_matrix_set_color_all(RGB_YELLOW);
//           break;
//       case 9:
//           rgb_matrix_set_color_all(RGB_ORANGE);
//           break;
//       case 10:
//           rgb_matrix_set_color_all(RGB_ORANGE);
//           break;
//         default: // for any other layers, or the default layer
//             if (workmode == 0) {
//               rgb_matrix_set_color_all(RGB_WHITE);
//               break;
//             } else {
//               rgb_matrix_set_color_all(RGB_RED);
//               break;
//             }
//     }
// #endif

   return state;
}

bool use_default_xcase_separator(uint16_t keycode, const keyrecord_t *record) {
    switch (keycode) {
        case KC_A ... KC_Z:
        case KC_1 ... KC_0:
            return true;
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // if (record->event.pressed) {
    //    set_keylog(keycode, record);
    //
    // }
    //int i = 0;  // for backspace word

    if (PARAGRAPH_MODE) {
        if (timer_elapsed(p1_timer) < 5001) {
            if (record->event.pressed) {
                p1_timer = timer_read();
                switch (keycode) {
                    case KC_DOT:
                        keycode = MRO_SPC_PERIOD;
                        break;
                    case KC_SLASH:
                        if (get_mods() & MOD_BIT(KC_LSHIFT) || get_mods() & MOD_BIT(KC_RSHIFT)){
                            keycode = MRO_SPC_QUESTION;
                        } 
                        break;
                    case KC_EXLM:
//                        if (get_mods() & MOD_BIT(KC_LSHIFT) || get_mods() & MOD_BIT(KC_RSHIFT)){
                            keycode = MRO_SPC_EX;
//                        } 
                        break;
                    case KC_COMMA:
                        keycode = MRO_SPC_COMMA;
                        break;
                    case KC_SCOLON:
                        SEND_STRING(SS_TAP(X_SCOLON) SS_DELAY(50) SS_TAP(X_SPACE));
                        //SHIFTED = true;
                        return false;
                }
            }
        } else {
            PARAGRAPH_MODE = false;
            SHIFTED = false;
        }
    }

    if (SHIFTED) {
        if (record->event.pressed) {
            SHIFTED = false; 
            if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
             (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { keycode = keycode & 0xFF; }
            switch (keycode) {
                case KC_A ... KC_Z:
                    register_code16(KC_LSFT);
                    tap_code16(keycode);
                    unregister_code16(KC_LSFT);
                    return false;
                case MRO_SPC_PERIOD:
                case KC_DOT:
                    SEND_STRING(SS_TAP(X_BSPC) SS_DELAY(10) SS_TAP(X_DOT));
                    return false;
            }
            return true;
        }
    }
    if (!process_case_modes(keycode, record)) {
       return false;
    }

    // Process leader key sequences
    if (!process_leader(keycode, record)) {
        return false;
    }
  switch (keycode) {
    case LEADER:
        if (record->event.pressed) {
            start_leading();
        }
        return false;
    case BS2:
        if (record->event.pressed) {
          SEND_STRING(SS_TAP(X_BSPC) SS_DELAY(10) SS_TAP(X_BSPC));
        }
        break;
    case BS3:
        if (record->event.pressed) {
          SEND_STRING(SS_TAP(X_BSPC) SS_DELAY(10) SS_TAP(X_BSPC) SS_DELAY(10) SS_TAP(X_BSPC));
        }
        break;
    case BS4:
        if (record->event.pressed) {
          SEND_STRING(SS_TAP(X_BSPC) SS_DELAY(10) SS_TAP(X_BSPC) SS_DELAY(10) SS_TAP(X_BSPC) SS_DELAY(10) SS_TAP(X_BSPC));
        }
        break;
    case BS5:
        if (record->event.pressed) {
          SEND_STRING(SS_TAP(X_BSPC) SS_DELAY(10) SS_TAP(X_BSPC) SS_DELAY(10) SS_TAP(X_BSPC) SS_DELAY(10) SS_TAP(X_BSPC) SS_DELAY(10) SS_TAP(X_BSPC));
        }
        break;
    case BS6:
        if (record->event.pressed) {
          SEND_STRING(SS_TAP(X_BSPC) SS_DELAY(10) SS_TAP(X_BSPC) SS_DELAY(10) SS_TAP(X_BSPC) SS_DELAY(10) SS_TAP(X_BSPC) SS_DELAY(10) SS_TAP(X_BSPC) SS_DELAY(10) SS_TAP(X_BSPC));
        }
        break;
    // case ST_MACRO_0:
    //     if (record->event.pressed) {
    //       SEND_STRING(SS_TAP(X_LSHIFT) SS_DELAY(300) SS_TAP(X_LSHIFT) SS_DELAY(300) SS_TAP(X_1));
    //       workmode = 0;
    //       //rgblight_sethsv_noeeprom(0, 0, 255);
    //       //PLAY_SONG(my_song);

    //     }
    //     break;
    // case ST_MACRO_1:
    //     if (record->event.pressed) {
    //       SEND_STRING(SS_TAP(X_LSHIFT) SS_DELAY(300) SS_TAP(X_LSHIFT) SS_DELAY(300) SS_TAP(X_2));
    //       workmode = 1;
    //       //rgblight_setrgb (RGB_RED);
    //       //PLAY_SONG(my_song);

    //     }
    //     break;
    case ST_MACRO_2:
        if (record->event.pressed) {
          SEND_STRING(SS_DELAY(150) SS_TAP(X_RCTL) SS_DELAY(150) SS_TAP(X_RCTL) SS_DELAY(150) SS_TAP(X_1) SS_DELAY(150));
          workmode = 0;
          //rgblight_sethsv_noeeprom(0, 0, 255);
        }
        break;
    case ST_MACRO_3:
        if (record->event.pressed) {
          SEND_STRING(SS_DELAY(150) SS_TAP(X_RCTL) SS_DELAY(150) SS_TAP(X_RCTL) SS_DELAY(150) SS_TAP(X_2) SS_DELAY(150));
          workmode = 1;
          //rgblight_setrgb (RGB_RED);
        }
        break;
    // case ST_MACRO_4:
    //     if (record->event.pressed) {
    //       SEND_STRING(SS_TAP(X_DOT) SS_DELAY(50) SS_TAP(X_SPACE) SS_DELAY(50) SS_TAP(X_SPACE));

    //     }
    //     break;
    case L7_MACRO_1:  // Magnet
        if (record->event.pressed) {
          SEND_STRING(SS_LCTL(SS_LALT(SS_TAP(X_LEFT))));

        }
        break;
    case L7_MACRO_2: // Magnet
        if (record->event.pressed) {
          SEND_STRING(SS_LCTL(SS_LALT(SS_TAP(X_RIGHT))));

        }
        break;
    case L7_MACRO_3:
        if (record->event.pressed) {
          SEND_STRING(SS_LGUI(SS_TAP(X_TAB)));

        }
        break;
    case L7_MACRO_4:
    if (workmode == 1){
      if (record->event.pressed) {
        register_code16(KC_LGUI);
        tap_code16(KC_TAB);
      } else {
        tap_code16(KC_ENTER);
        unregister_code16(KC_LGUI);
      }
    } else {
      if (record->event.pressed) {
        register_code16(KC_LALT);
        tap_code16(KC_TAB);
      } else {
        //tap_code16(KC_SPACE);
        unregister_code16(KC_LALT);
      }
    }
    break;
    case L7_MACRO_5: // Mission Control
    if (record->event.pressed) {
      SEND_STRING(SS_LCTL(SS_TAP(X_UP)));

    }
    break;
    case L7_MACRO_6: // Magnet
    if (record->event.pressed) {
      SEND_STRING(SS_LCTL(SS_LALT(SS_LGUI(SS_TAP(X_LEFT)))));

    }
    break;
    case L7_MACRO_7: // Magnet
    if (record->event.pressed) {
      SEND_STRING(SS_LCTL(SS_LALT(SS_LGUI(SS_TAP(X_RIGHT)))));

    }
    break;
    case L7_MACRO_8: // Magnet
    if (record->event.pressed) {
      SEND_STRING(SS_LCTL(SS_LALT(SS_LGUI(SS_TAP(X_M)))));

    }
    break;
    case L7_MACRO_9:
    if (workmode == 1){
      if (record->event.pressed) {
        SEND_STRING(SS_LGUI(SS_TAP(X_GRAVE)));
      }
    } else {
      if (record->event.pressed) {
        register_code16(KC_LALT);
        tap_code16(KC_GRAVE);
      } else {
//            tap_code16(KC_ENTER);
      unregister_code16(KC_LALT);
      }
//            SEND_STRING(SS_LALT(SS_TAP(X_GRAVE)));
    }
    break;
    case L7_MACRO_10:
    if (record->event.pressed) {
      if (workmode == 0)
        SEND_STRING(SS_LALT(SS_TAP(X_GRAVE)));
      else
        SEND_STRING(SS_LCTL(SS_TAP(X_UP)));
    }
    break;
    case L7_MACRO_11:
    if (record->event.pressed) {
      SEND_STRING(SS_LALT(SS_TAP(X_ROPT)));

    }
    break;
    case MRO_SPC_PERIOD:
    if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_DOT) SS_DELAY(50) SS_TAP(X_SPACE));
        SHIFTED = true;
    }
    return false;
    case MRO_SPC_COMMA:
    if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_COMMA) SS_DELAY(50) SS_TAP(X_SPACE));
    }
    return false;
    case MRO_SPC_QUESTION:
    if (record->event.pressed) {
        SEND_STRING(SS_LSFT(SS_TAP(X_SLASH)) SS_DELAY(50) SS_TAP(X_SPACE));
        SHIFTED = true;
    }
    return false;
    case MRO_SPC_EX:
    if (record->event.pressed) {
        SEND_STRING(SS_LSFT(SS_TAP(X_1)) SS_DELAY(50) SS_TAP(X_SPACE));
        SHIFTED = true;
    }
    return false;
    case MRO_SPC_MINUS:
    if (record->event.pressed) {
        SEND_STRING(" -");
    }
    break;
    // case MRO_BACKSPACE_WORD:
    // if (record->event.pressed) {
    //   for (i = keycount; i > 0; i--) {
    //     SEND_STRING(SS_TAP(X_BSPC) SS_DELAY(10));
    //   }
    //   keycount = 0;
    // }
    // break;
    //default:
     //  if (record->event.pressed) {
     //    count_chars(keycode, record);
     // }
  }

  return true;
}

void *leader_signature_func(uint16_t keycode) {
    switch (keycode) {
        case KC_Q:
            SEND_STRING_DELAY("\nRussell\n",20);
            break;
        case KC_W:
            SEND_STRING_DELAY("\nThanks,\nRussell\n",20);
            break;
        case KC_F:
            SEND_STRING_DELAY("\nThank you,\nRussell\n",20);
            break;
        case KC_P:
            SEND_STRING_DELAY("\nThank you for your time,\nRussell\n",20);
            break;
        case KC_B:
            SEND_STRING_DELAY("\nThank you for your time,\nRussell\n",20);
            break;
        default:
            break;
    }
    return NULL; // this function is always an endpoint
}

void *leader_case_func(uint16_t keycode) {
    switch (keycode) {
        case KC_W:
            enable_caps_word();
            break;
        case KC_UNDS:
            enable_xcase_with(KC_UNDS);
            break;
        case KC_MINS:
            enable_xcase_with(KC_MINUS);
            break;
        case KC_A:
            enable_xcase_with(KC_UNDS);
            break;
        default:
            break;
    }
    return NULL; // this function is always an endpoint
}

void *leader_start_func(uint16_t keycode) {
    switch (keycode) {
        case KC_Q:
            return leader_signature_func;
        case KC_C:
            return leader_case_func;
        case KC_P:
            PARAGRAPH_MODE = !PARAGRAPH_MODE;
            p1_timer = timer_read();
            return NULL;
//        case KC_T:
//            return leader_toggles_func; // function that toggles keyboard settings
//        case KC_R:
//            reset_keyboard(); // here LDR r will reset the keyboard
//            return NULL; // signal that we're done
        default:
            return NULL;
    }
}


bool get_ignore_mod_tap_interrupt(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MT(MOD_RSFT,KC_SLASH):
            return false;
        // case MT(MOD_LSFT,KC_Z):
        //     return false;
        //case OSM(MOD_LCTL):
        //    return false;
        //case MT(MOD_RSFT,KC_SPC):
        //  return false;
        //case MT(MOD_LSFT,KC_SPC):
        //  return false;
        default:
            return true;
    }
}
bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MT(MOD_LSFT,KC_T):
            return true;
        case MT(MOD_RSFT,KC_N):
            return true;
        case MT(MOD_RSFT,KC_SLASH):
            return true;
        case MT(MOD_LSFT,KC_Z):
            return true;
        default:
            return false;
    }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
      case LT(3,KC_SPC):  //this was acting weird so I added it here
            return true;
      default:
          return false;
    }
}

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
     switch (keycode) {
        case LT(3,KC_SPC):
          return true;
        case LT(5,KC_BSPACE):
          return true;
//        case LT(3,KC_A):
//2          return true;
//        case LT(3,KC_O):
//          return true;
        default:
            return false;
     }
 }

 bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
     switch (keycode) {
       case LT(8,KC_ESC):
         return true;
       case LT(7,KC_TAB):
         return true;
//       case LT(3,KC_SPC):
//         return true;
      case LT(5,KC_BSPACE):
          return true;
       default:
         // Do not select the hold action when another key is pressed.
         return false;
     }
 }

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
     switch (keycode) {
        case TD(DANCE_2):
             return 300;
        // case LT(3,KC_O):
        //      return 175;
        // case LT(3,KC_A):
        //      return 175;
        // case LT(9,KC_SPC):
        //      return 150;
        // case LT(10,KC_SPC):
        //      return 150;
       default:
           return TAPPING_TERM;
   }
}



typedef struct {
    bool is_press_action;
    uint8_t step;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP,
    MORE_TAPS
};

static tap dance_state[11];

uint8_t dance_step(qk_tap_dance_state_t *state);

uint8_t dance_step(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }
    return MORE_TAPS;
}


// void on_dance_0(qk_tap_dance_state_t *state, void *user_data);
// void dance_0_finished(qk_tap_dance_state_t *state, void *user_data);
// void dance_0_reset(qk_tap_dance_state_t *state, void *user_data);

// void on_dance_0(qk_tap_dance_state_t *state, void *user_data) {
//     if(state->count == 3) {
//         tap_code16(KC_ESCAPE);
//         tap_code16(KC_ESCAPE);
//         tap_code16(KC_ESCAPE);
//     }
//     if(state->count > 3) {
//         tap_code16(KC_ESCAPE);
//     }
// }

// void dance_0_finished(qk_tap_dance_state_t *state, void *user_data) {
//     dance_state[0].step = dance_step(state);
//     switch (dance_state[0].step) {
//         case SINGLE_TAP: register_code16(KC_ESCAPE); break;
//         case SINGLE_HOLD: layer_on(9); break;
//         case DOUBLE_TAP: register_code16(KC_GRAVE); break;
//         case DOUBLE_SINGLE_TAP: tap_code16(KC_ESCAPE); register_code16(KC_ESCAPE);
//     }
// }

// void dance_0_reset(qk_tap_dance_state_t *state, void *user_data) {
//     wait_ms(10);
//     switch (dance_state[0].step) {
//         case SINGLE_TAP: unregister_code16(KC_ESCAPE); break;
//         case SINGLE_HOLD: layer_off(9); break;
//         case DOUBLE_TAP: unregister_code16(KC_GRAVE); break;
//         case DOUBLE_SINGLE_TAP: unregister_code16(KC_ESCAPE); break;
//     }
//     dance_state[0].step = 0;
// }
// void on_dance_1(qk_tap_dance_state_t *state, void *user_data);
// void dance_1_finished(qk_tap_dance_state_t *state, void *user_data);
// void dance_1_reset(qk_tap_dance_state_t *state, void *user_data);

// void on_dance_1(qk_tap_dance_state_t *state, void *user_data) {
//     if(state->count == 3) {
//         tap_code16(KC_TAB);
//         tap_code16(KC_TAB);
//         tap_code16(KC_TAB);
//     }
//     if(state->count > 3) {
//         tap_code16(KC_TAB);
//     }
// }

// void dance_1_finished(qk_tap_dance_state_t *state, void *user_data) {
//     dance_state[1].step = dance_step(state);
//     switch (dance_state[1].step) {
//         case SINGLE_TAP: register_code16(KC_TAB); break;
//         case SINGLE_HOLD: layer_on(7); break;
//         case DOUBLE_TAP: register_code16(KC_TAB); register_code16(KC_TAB); break;
//         case DOUBLE_SINGLE_TAP: tap_code16(KC_TAB); register_code16(KC_TAB);
//     }
// }

// void dance_1_reset(qk_tap_dance_state_t *state, void *user_data) {
//     wait_ms(10);
//     switch (dance_state[1].step) {
//         case SINGLE_TAP: unregister_code16(KC_TAB); break;
//         case SINGLE_HOLD: layer_off(7); break;
//         case DOUBLE_TAP: unregister_code16(KC_TAB); break;
//         case DOUBLE_SINGLE_TAP: unregister_code16(KC_TAB); break;
//     }
//     dance_state[1].step = 0;
// }


//void on_dance_2(qk_tap_dance_state_t *state, void *user_data);
void dance_2_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_2_reset(qk_tap_dance_state_t *state, void *user_data);

// void on_dance_2(qk_tap_dance_state_t *state, void *user_data) {
//     if(state->count == 3) {
//         tap_code16(KC_LSHIFT);
//         tap_code16(KC_LSHIFT);
//         tap_code16(KC_LSHIFT);
//     }
//     if(state->count > 3) {
//         tap_code16(KC_LSHIFT);
//     }
// }

void dance_2_finished(qk_tap_dance_state_t *state, void *user_data) {
    dance_state[2].step = dance_step(state);
    switch (dance_state[2].step) {
        case SINGLE_TAP: SEND_STRING(SS_LGUI(SS_TAP(X_V))); break;
        case SINGLE_HOLD: SEND_STRING(SS_LGUI(SS_TAP(X_C)));  break;
        case DOUBLE_TAP: SEND_STRING(SS_LGUI(SS_TAP(X_C)));   break;
        case DOUBLE_HOLD: SEND_STRING(SS_LGUI(SS_TAP(X_X)));  break;
        //case DOUBLE_SINGLE_TAP: tap_code16(KC_LSHIFT); register_code16(KC_LSHIFT);
    }
}

void dance_2_reset(qk_tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state[2].step) {
        //case SINGLE_TAP: unregister_code16(KC_LSHIFT); break;
        case SINGLE_HOLD: SEND_STRING(SS_LGUI(SS_TAP(X_V))); break;
        //case DOUBLE_TAP: unregister_code16(KC_LSHIFT); break;
        case DOUBLE_HOLD: SEND_STRING(SS_LGUI(SS_TAP(X_V))); break;
        //case DOUBLE_SINGLE_TAP: unregister_code16(KC_LSHIFT); break;
    }
    dance_state[2].step = 0;
}
// void dance_3_finished(qk_tap_dance_state_t *state, void *user_data);
// void dance_3_reset(qk_tap_dance_state_t *state, void *user_data);

// void dance_3_finished(qk_tap_dance_state_t *state, void *user_data) {
//     dance_state[3].step = dance_step(state);
//     switch (dance_state[3].step) {
//         case SINGLE_TAP: layer_move(3); break;
//         case SINGLE_HOLD: layer_on(4); break;
//         case DOUBLE_TAP: layer_move(1); break;
//         case DOUBLE_HOLD: layer_on(7); break;
//         case DOUBLE_SINGLE_TAP: layer_move(3); break;
//     }
// }

// void dance_3_reset(qk_tap_dance_state_t *state, void *user_data) {
//     wait_ms(10);
//     switch (dance_state[3].step) {
//         case SINGLE_HOLD: layer_off(4); break;
//         case DOUBLE_HOLD: layer_off(7); break;
//     }
//     dance_state[3].step = 0;
// }
void on_dance_4(qk_tap_dance_state_t *state, void *user_data);
void dance_4_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_4_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_4(qk_tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_MINUS);
        tap_code16(KC_MINUS);
        tap_code16(KC_MINUS);
    }
    if(state->count > 3) {
        tap_code16(KC_MINUS);
    }
}

void dance_4_finished(qk_tap_dance_state_t *state, void *user_data) {
    dance_state[4].step = dance_step(state);
    switch (dance_state[4].step) {
        case SINGLE_TAP: register_code16(KC_MINUS); break;
        case DOUBLE_TAP: register_code16(KC_PERC); break;
        case DOUBLE_SINGLE_TAP: tap_code16(KC_MINUS); register_code16(KC_MINUS);
    }
}

void dance_4_reset(qk_tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state[4].step) {
        case SINGLE_TAP: unregister_code16(KC_MINUS); break;
        case DOUBLE_TAP: unregister_code16(KC_PERC); break;
        case DOUBLE_SINGLE_TAP: unregister_code16(KC_MINUS); break;
    }
    dance_state[4].step = 0;
}
void on_dance_5(qk_tap_dance_state_t *state, void *user_data);
void dance_5_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_5_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_5(qk_tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_PLUS);
        tap_code16(KC_PLUS);
        tap_code16(KC_PLUS);
    }
    if(state->count > 3) {
        tap_code16(KC_PLUS);
    }
}

void dance_5_finished(qk_tap_dance_state_t *state, void *user_data) {
    dance_state[5].step = dance_step(state);
    switch (dance_state[5].step) {
        case SINGLE_TAP: register_code16(KC_PLUS); break;
        case DOUBLE_TAP: register_code16(KC_CIRC); break;
        case DOUBLE_SINGLE_TAP: tap_code16(KC_PLUS); register_code16(KC_PLUS);
        //default: register_code16(KC_ASTERISK); break;
    }
}

void dance_5_reset(qk_tap_dance_state_t *state, void *user_data) {
    //wait_ms(10);
    switch (dance_state[5].step) {
        case SINGLE_TAP: unregister_code16(KC_PLUS); break;
        case DOUBLE_TAP: unregister_code16(KC_CIRC); break;
        case DOUBLE_SINGLE_TAP: unregister_code16(KC_PLUS); break;
        //default: unregister_code16(KC_ASTERISK); unregister_code16(KC_SLASH); break;
    }
    dance_state[5].step = 0;
}
void on_dance_6(qk_tap_dance_state_t *state, void *user_data);
void dance_6_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_6_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_6(qk_tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_DOT);
        tap_code16(KC_DOT);
        tap_code16(KC_DOT);
    }
    if(state->count > 3) {
        tap_code16(KC_DOT);
    }
}

void dance_6_finished(qk_tap_dance_state_t *state, void *user_data) {
    dance_state[6].step = dance_step(state);
    switch (dance_state[6].step) {
        case SINGLE_TAP: register_code16(KC_DOT); break;
        case DOUBLE_TAP: register_code16(KC_DLR); break;
        case DOUBLE_SINGLE_TAP: tap_code16(KC_DOT); register_code16(KC_DOT);
    }
}

void dance_6_reset(qk_tap_dance_state_t *state, void *user_data) {
    //wait_ms(10);
    switch (dance_state[6].step) {
        case SINGLE_TAP: unregister_code16(KC_DOT); break;
        case DOUBLE_TAP: unregister_code16(KC_DLR); break;
        case DOUBLE_SINGLE_TAP: unregister_code16(KC_DOT); break;
    }
    dance_state[6].step = 0;
}
// void on_dance_7(qk_tap_dance_state_t *state, void *user_data);
// void dance_7_finished(qk_tap_dance_state_t *state, void *user_data);
// void dance_7_reset(qk_tap_dance_state_t *state, void *user_data);

// void on_dance_7(qk_tap_dance_state_t *state, void *user_data) {
//     if(state->count == 3) {
//         tap_code16(KC_SPC);
//         tap_code16(KC_SPC);
//         tap_code16(KC_SPC);
//     }
//     if(state->count > 3) {
//         tap_code16(KC_SPC);
//     }
// }

// void dance_7_finished(qk_tap_dance_state_t *state, void *user_data) {
//     dance_state[7].step = dance_step(state);
//     switch (dance_state[7].step) {
//         case SINGLE_TAP: register_code16(KC_SPC); break;
//         case SINGLE_HOLD: layer_on(9); break;
//         case DOUBLE_TAP: tap_code16(KC_SPC); register_code16(KC_SPC); break;
//         case DOUBLE_SINGLE_TAP: tap_code16(KC_SPC); register_code16(KC_SPC);
//     }
// }

// void dance_7_reset(qk_tap_dance_state_t *state, void *user_data) {
//     wait_ms(10);
//     switch (dance_state[7].step) {
//         case SINGLE_TAP: unregister_code16(KC_SPC); break;
//         case SINGLE_HOLD: layer_off(9); unregister_code16(KC_SPC); break;
//         case DOUBLE_TAP: unregister_code16(KC_SPC); break;
//         case DOUBLE_SINGLE_TAP: unregister_code16(KC_SPC); break;
//     }
//     dance_state[7].step = 0;
// }
// void on_dance_8(qk_tap_dance_state_t *state, void *user_data);
// void dance_8_finished(qk_tap_dance_state_t *state, void *user_data);
// void dance_8_reset(qk_tap_dance_state_t *state, void *user_data);

// void on_dance_8(qk_tap_dance_state_t *state, void *user_data) {
//     if(state->count == 3) {
//         tap_code16(KC_ENTER);
//         tap_code16(KC_ENTER);
//         tap_code16(KC_ENTER);
//     }
//     if(state->count > 3) {
//         tap_code16(KC_ENTER);
//     }
// }

// void dance_8_finished(qk_tap_dance_state_t *state, void *user_data) {
//     dance_state[8].step = dance_step(state);
//     switch (dance_state[8].step) {
//         case SINGLE_TAP: register_code16(KC_ENTER); break;
//         case SINGLE_HOLD: layer_on(10); break;
//         case DOUBLE_TAP: tap_code16(KC_ENTER); register_code16(KC_ENTER); break;
//         case DOUBLE_SINGLE_TAP: tap_code16(KC_ENTER); register_code16(KC_ENTER);
//     }
// }

// void dance_8_reset(qk_tap_dance_state_t *state, void *user_data) {
//     wait_ms(10);
//     switch (dance_state[8].step) {
//       case SINGLE_TAP: unregister_code16(KC_ENTER); break;
//       case SINGLE_HOLD: layer_off(10); unregister_code16(KC_ENTER); break;
//       case DOUBLE_TAP: unregister_code16(KC_ENTER); break;
//       case DOUBLE_SINGLE_TAP: unregister_code16(KC_ENTER); break;
//     }
//     dance_state[8].step = 0;
// }
// void on_dance_9(qk_tap_dance_state_t *state, void *user_data);
// void dance_9_finished(qk_tap_dance_state_t *state, void *user_data);
// void dance_9_reset(qk_tap_dance_state_t *state, void *user_data);

// void on_dance_9(qk_tap_dance_state_t *state, void *user_data) {
//     if(state->count == 3) {
//         tap_code16(KC_DOT);
//         tap_code16(KC_DOT);
//         tap_code16(KC_DOT);
//     }
//     if(state->count > 3) {
//         tap_code16(KC_DOT);
//     }
// }

// void dance_9_finished(qk_tap_dance_state_t *state, void *user_data) {
//     dance_state[9].step = dance_step(state);
//     switch (dance_state[9].step) {
//         case SINGLE_TAP: register_code16(KC_DOT); break;
//         case DOUBLE_TAP: register_code16(KC_EQUAL); break;
//         case DOUBLE_SINGLE_TAP: tap_code16(KC_DOT); register_code16(KC_DOT);
//     }
// }

// void dance_9_reset(qk_tap_dance_state_t *state, void *user_data) {
//     wait_ms(10);
//     switch (dance_state[9].step) {
//         case SINGLE_TAP: unregister_code16(KC_DOT); break;
//         case DOUBLE_TAP: unregister_code16(KC_EQUAL); break;
//         case DOUBLE_SINGLE_TAP: unregister_code16(KC_DOT); break;
//     }
//     dance_state[9].step = 0;
// }
// void on_dance_10(qk_tap_dance_state_t *state, void *user_data);
// void dance_10_finished(qk_tap_dance_state_t *state, void *user_data);
// void dance_10_reset(qk_tap_dance_state_t *state, void *user_data);

// void on_dance_10(qk_tap_dance_state_t *state, void *user_data) {
//     if(state->count == 3) {
//         tap_code16(KC_PERCENT);
//         tap_code16(KC_PERCENT);
//         tap_code16(KC_PERCENT);
//     }
//     if(state->count > 3) {
//         tap_code16(KC_PERCENT);
//     }
// }

// void dance_10_finished(qk_tap_dance_state_t *state, void *user_data) {
//     dance_state[10].step = dance_step(state);
//     switch (dance_state[10].step) {
//         case SINGLE_TAP: register_code16(KC_PERCENT); break;
//         case DOUBLE_TAP: register_code16(KC_DOLLAR); break;
//         case DOUBLE_SINGLE_TAP: tap_code16(KC_PERCENT); register_code16(KC_PERCENT);
//     }
// }

// void dance_10_reset(qk_tap_dance_state_t *state, void *user_data) {
//     wait_ms(10);
//     switch (dance_state[10].step) {
//         case SINGLE_TAP: unregister_code16(KC_PERCENT); break;
//         case DOUBLE_TAP: unregister_code16(KC_DOLLAR); break;
//         case DOUBLE_SINGLE_TAP: unregister_code16(KC_PERCENT); break;
//     }
//     dance_state[10].step = 0;
// }

qk_tap_dance_action_t tap_dance_actions[] = {
        //[DANCE_0] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_0, dance_0_finished, dance_0_reset),
        //[DANCE_1] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_1, dance_1_finished, dance_1_reset),
        [DANCE_2] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_2_finished, dance_2_reset),
        //[DANCE_3] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_3_finished, dance_3_reset),
        [DANCE_4] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_4, dance_4_finished, dance_4_reset),
        [DANCE_5] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_5, dance_5_finished, dance_5_reset),
        [DANCE_6] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_6, dance_6_finished, dance_6_reset),
        //[DANCE_7] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_7, dance_7_finished, dance_7_reset),
        //[DANCE_8] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_8, dance_8_finished, dance_8_reset),
        //[DANCE_9] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_9, dance_9_finished, dance_9_reset),
        //[DANCE_10] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_10, dance_10_finished, dance_10_reset),
};
