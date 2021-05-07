#include QMK_KEYBOARD_H

#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif

#include "conway.h"

#define CONWAY_X 16
#define CONWAY_Y 32

static conway_cell cells[CONWAY_X * CONWAY_Y] = {0};
static conway_grid grid = {
	.cells = cells,
	.width = CONWAY_X,
	.height = CONWAY_Y,
};


extern uint8_t is_master;

enum layer_number {
  _QWERTY = 0,
  _LOWER,
  _RAISE,
  _ADJUST,
  _ONI1,         // Oxygen Not Included mode.
  _ONI2,
};

#define CESC LCTL_T(KC_ESC)
#define MEH_ LCTL(LSFT(KC_LGUI))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {


/* QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  BS  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | CESC |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------| ONI   |    |   _   |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LOWER| LGUI | /Space  /       \Enter \  |RAISE | MEH  | RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_QWERTY] = LAYOUT( \
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC, \
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS, \
  CESC,     KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,  TO(_ONI1),KC_UNDS, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, \
                        KC_LALT, MO(_LOWER), KC_LGUI, KC_SPC,   KC_ENT,  MO(_RAISE), MEH_,    KC_RGUI \
),
/* LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | CMDTB| CMDQ | CMDW |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |   `  |   !  |   @  |   #  |   $  |   %  |-------.    ,-------|   ^  |   &  |   *  |   (  |   )  |   -  |
 * |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
 * |      | CMDZ | CMDX | CMDC | CMDV |      |-------|    |-------|      |   _  |   +  |   {  |   }  |   |  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
[_LOWER] = LAYOUT( \
  _______,   _______, _______, _______, _______, _______,                   _______, _______, _______,_______, _______, _______,\
  G(KC_TAB), G(KC_Q), G(KC_W), _______, _______, _______,                   _______, _______, _______,_______, _______, _______, \
  KC_GRV,    KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_TILD, \
  _______,   G(KC_Z), G(KC_X), G(KC_C), G(KC_V), _______, _______, _______, XXXXXXX, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, \
                               _______, _______, _______, _______, _______,  _______, _______, _______\
),
/* RAISE
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |   `  |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |-------.    ,-------| Left | Down |  Up  |Right |      |      |
 * |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
 * |  F7  |  F8  |  F9  | F10  | F11  | F12  |-------|    |-------|   +  |   -  |   =  |   [  |   ]  |   \  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

[_RAISE] = LAYOUT( \
  KC_GRV,  _______, _______, _______, _______, _______,                     _______, _______, _______, _______, _______, _______, \
  _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______, \
  KC_F1,   KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,                       KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, XXXXXXX, \
  KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,   _______, _______,  KC_PLUS, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS, \
                             _______, _______, _______,  _______, _______,  _______, _______, _______ \
),
/* ADJUST
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|      |      |RGB ON| HUE+ | SAT+ | VAL+ |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |      | MODE | HUE- | SAT- | VAL- |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
[_ADJUST] = LAYOUT( \
XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,\
                           _______, _______, _______, _______, _______,  _______, _______, _______ \
),

#define EXIT TO(_QWERTY)
#define SPONI LT(_ONI2, KC_SPC)

/* ONI1
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|      |      |RGB ON| HUE+ | SAT+ | VAL+ |
 * |------+------+------+------+------+------| EXIT  |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |      | MODE | HUE- | SAT- | VAL- |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt |      |      | /Space/ /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/  ONI2 /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
[_ONI1] = LAYOUT( \
_______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, \
_______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, \
_______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, \
_______, _______, _______, _______, _______, _______, EXIT,    _______, _______, _______, _______, _______, _______, _______,\
                           _______, _______, _______, SPONI,   _______, _______, _______, _______ \
),

/* ONI2
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |  M   |  O   |  P   |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  F1  |  ... |      |      |      |      |-------.    ,-------|      |      |RGB ON| HUE+ | SAT+ | VAL+ |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |      | MODE | HUE- | SAT- | VAL- |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |      |      |      | /       /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
[_ONI2] = LAYOUT( \
_______, _______, _______, _______, _______, _______,                     _______, _______, _______, _______, _______, _______, \
_______, _______, _______, KC_M,    KC_O,    KC_P,                     _______, _______, _______, _______, _______, _______, \
  KC_F1, KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                    _______, _______, _______, _______, _______, _______, \
  KC_F7, KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, _______, _______, _______, _______, _______, _______, _______, _______, \
                           _______, _______, _______, _______, _______,  _______, _______, _______ \
)
};


// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

//SSD1306 OLED update loop, make sure to enable OLED_DRIVER_ENABLE=yes in rules.mk
#ifdef OLED_DRIVER_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master()) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  } else {
    return OLED_ROTATION_270;
  }
}

// When you add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

void show_layer_status(void);

void keyboard_post_init_user(void) {
  conway_seed_random(&grid, 123);
  oled_clear();
}

void render_frame(void) {
  show_layer_status();

  // TODO: probably way too fast
  conway_advance(&grid);

  //if (is_keyboard_master()) {
}

void show_layer_status() {
  int blank_lines = 3;
  if (layer_state & (1 << _LOWER)) {
    oled_write_ln_P(PSTR("LOWER"), false);
    blank_lines--;
  }

  if (layer_state & (1 << _RAISE)) {
    oled_write_ln_P(PSTR("RAISE"), false);
    blank_lines--;
  }

  if (layer_state & (1 << _ONI2)) {
    oled_write_ln_P(PSTR("ONI+"), false);
    blank_lines--;
  } else if (layer_state & (1 << _ONI1)) {
    oled_write_ln_P(PSTR("ONI"), false);
    blank_lines--;
  }

  for (int i = 0; i < blank_lines; i++) {
    oled_write_ln_P(PSTR("     "), false);
  }
}

#define FRAME_TIMEOUT 1000/20
static uint16_t anim_timer = 0;

#define CONWAY_TIMEOUT 1000/10
static uint16_t conway_timer = 0;

void oled_task_user(void) {
  // time for the next frame?
  if (timer_elapsed(anim_timer) > FRAME_TIMEOUT) {
    anim_timer = timer_read();
    render_frame();
  }

  if (timer_elapsed(conway_timer) > CONWAY_TIMEOUT) {
    conway_timer = timer_read();
    conway_advance(&grid);

    // TODO: Efficient writing only changes.
    for (size_t y = 0; y < CONWAY_Y; y++) {
      for (size_t x = 0; x < CONWAY_X; x++) {
        conway_cell* cell = cell_at(&grid, x, y);

        oled_write_pixel(x*2, 64 + (y*2), cell->is_alive);
      }
    }
  }
}
#endif // OLED_DRIVER_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // Add some entropy to our Conway's Game of Life
  cell_at(&grid, keycode % CONWAY_X, anim_timer % CONWAY_Y)->is_alive = 1;

  if (record->event.pressed) {
#ifdef OLED_DRIVER_ENABLE
    set_keylog(keycode, record);
#endif
    // set_timelog();
  }
  return true;
}
