#include "midi.h"
#include "quantum.h"
#include "quantum_keycodes.h"
static uint16_t copy_timer;
static uint16_t sync_timer;
static uint16_t tap_hold_timer;

extern MidiDevice midi_device;

// Define MIDI channel
#define MIDI_CHANNEL 2 // MIDI channel (0 = Channel 1)

#define LAYOUT( \
    k00, \
    k10, k11, k12, k13, \
    k20, k21, k22, k23, \
    k30, k31, k32, k33  \
) \
{ \
    { k00 }, \
    { k10, k11, k12, k13 }, \
    { k20, k21, k22, k23 }, \
    { k30, k31, k32, k33 }  \
}

// Tap Dance Enum
enum {
    TD_COPY_ACTION,
};

void copy_action_finished(tap_dance_state_t *state, void *user_data) {
    if (state->pressed) {
        // Hold: Send Ctrl+C
        register_code(KC_LCTL);
        tap_code(KC_C);
        unregister_code(KC_LCTL);
    } else if (state->count == 1) {
        // Single Tap: Send Ctrl+C and Enter
        register_code(KC_LCTL);
        tap_code(KC_C);
        unregister_code(KC_LCTL);
        wait_ms(200);
        tap_code(KC_ENT);
    } else if (state->count == 2) {
        // Double Tap: Send Ctrl+V
        register_code(KC_LCTL);
        tap_code(KC_V);
        unregister_code(KC_LCTL);
    }
}

void copy_action_reset(tap_dance_state_t *state, void *user_data) {
    // Reset logic (e.g., clean up held keys)
    unregister_code(KC_LCTL);
}

// Tap Dance Action Definition
tap_dance_action_t tap_dance_actions[] = {
    [TD_COPY_ACTION] = ACTION_TAP_DANCE_FN_ADVANCED(
        NULL, copy_action_finished, copy_action_reset
    ),
};

// Define a single layer
enum layers {
    _CULL,
    _LTRM,
    _PRST,
    _SHTCT
};
// Define custom keycodes
enum midi_cc_keycodes_LTRM {
	MIDI_CC1 = SAFE_RANGE,
	MIDI_CC2,
	MIDI_CC3,
	MIDI_CC4,
	MIDI_CC5,
	MIDI_CC6,
	MIDI_CC7,
	MIDI_CC8,
	MIDI_CC9,
	MIDI_CC10,
	MIDI_CC11,
	MIDI_CC12,
	MIDI_CC13,
	MIDI_CC14,
	MIDI_CC15,
	MIDI_CC16,
	MIDI_CC17,
	MIDI_CC18,
	MIDI_CC19,
	MIDI_CC20,
	MIDI_CC21,
	MIDI_CC22,
	MIDI_CC23,
	MIDI_CC24,
	MIDI_CC25,
	L_IND,
	L_CYC,
    CULL_MACRO_1,
    CULL_MACRO_2,
    CULL_MACRO_3,
    CULL_MACRO_4,
    SWITCH_G_L,
    SHTCT_MACRO_1,
    SHTCT_MACRO_2,
    SHTCT_MACRO_3,
    SHTCT_MACRO_4,
    SHTCT_MACRO_5,
    SHTCT_MACRO_6,
    SHTCT_MACRO_7,
	SHTCT_MACRO_8,
	COPY_ACTION,
	SYNC,
	PASTE,
	TAP_HOLD_CTRL_SHIFT_D
};

static char current_alpha_oled = '\0';
static char current_ltrm_alpha_oled = '\0';

static uint8_t current_MIDI_ccNumber         = 1;
static char    current_MIDI_ccNumber_char[3] = {'0', '1', '\0'};

// Keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_CULL] = LAYOUT(
        KC_SPACE,                        // Encoder Button
        CULL_MACRO_1, CULL_MACRO_2, CULL_MACRO_3, CULL_MACRO_4,
        KC_UP, SWITCH_G_L, KC_S, KC_A,
        KC_DOWN, TAP_HOLD_CTRL_SHIFT_D, SYNC, L_CYC
    ),
    [_LTRM] = LAYOUT(
        MIDI_CC2,                        // Encoder Button
        MIDI_CC3, MIDI_CC4, MIDI_CC5, MIDI_CC6,
        MIDI_CC7, MIDI_CC8, MIDI_CC9, MIDI_CC10,
        TD(TD_COPY_ACTION), TAP_HOLD_CTRL_SHIFT_D, SYNC, KC_TRNS
    ),
    [_PRST] = LAYOUT(
        KC_SPACE,                        // Encoder Button
        MIDI_CC11, MIDI_CC12, MIDI_CC13, MIDI_CC14,
        MIDI_CC15, MIDI_CC16, MIDI_CC17, MIDI_CC18,
        TD(TD_COPY_ACTION), TAP_HOLD_CTRL_SHIFT_D, SYNC, KC_TRNS
    ),
    [_SHTCT] = LAYOUT(
        KC_MUTE,                         // Encoder Button
        SHTCT_MACRO_1, KC_MPLY, KC_MPRV, KC_MNXT,
        SHTCT_MACRO_2, SHTCT_MACRO_3, SHTCT_MACRO_4, SHTCT_MACRO_5,
        TD(TD_COPY_ACTION), SHTCT_MACRO_7, SHTCT_MACRO_8, KC_TRNS
    ),
};

/*Custom Keycodes*/
const rgblight_segment_t PROGMEM layer_zero_all[]  = RGBLIGHT_LAYER_SEGMENTS({0, 6, HSV_WHITE});
const rgblight_segment_t PROGMEM layer_one_all[]   = RGBLIGHT_LAYER_SEGMENTS({0, 6, HSV_RED});
const rgblight_segment_t PROGMEM layer_two_all[]   = RGBLIGHT_LAYER_SEGMENTS({0, 6, HSV_GREEN});
const rgblight_segment_t PROGMEM layer_three_all[] = RGBLIGHT_LAYER_SEGMENTS({0, 6, HSV_BLUE});

const rgblight_segment_t PROGMEM layer_zero[]  = RGBLIGHT_LAYER_SEGMENTS({0, 1, HSV_WHITE});
const rgblight_segment_t PROGMEM layer_one[]   = RGBLIGHT_LAYER_SEGMENTS({1, 1, HSV_WHITE});
const rgblight_segment_t PROGMEM layer_two[]   = RGBLIGHT_LAYER_SEGMENTS({3, 1, HSV_WHITE});
const rgblight_segment_t PROGMEM layer_three[] = RGBLIGHT_LAYER_SEGMENTS({4, 1, HSV_WHITE});

const rgblight_segment_t *const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(

    layer_zero_all,  // 0
    layer_one_all,   // 1
    layer_two_all,   // 2
    layer_three_all, // 3
    layer_zero,      // 4
    layer_one,       // 5
    layer_two,       // 6
    layer_three      // 7

);

void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;
}

#define LAYER_CYCLE_START 0 // 1st layer on the cycle
#define LAYER_CYCLE_END 3 // Last layer on the cycle

bool led_mode; // false for Blinking Mode, true for Static Mode

bool midi_encoder_active = false; // Flag to track encoder behavior

bool encoder_layer_cycling = false;

// MIDI key handling
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MIDI_CC1:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                current_MIDI_ccNumber         = 1;
                current_ltrm_alpha_oled       = 'Q';
                current_MIDI_ccNumber_char[0] = '0';
                current_MIDI_ccNumber_char[1] = '1';
             } else {
                // Revert encoder to default behavior on release
                midi_encoder_active = false;
            }
            return false;
            break;
        case MIDI_CC2:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                midi_send_cc(&midi_device, MIDI_CHANNEL, 2, 127); // Sends CC #14 on MIDI_CHANNEL with value 127
            } else {
                midi_send_cc(&midi_device, MIDI_CHANNEL, 2, 0); // Sends CC #14 on MIDI_CHANNEL with value 0
            }
            return false;
            break;
        case MIDI_CC3:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                current_MIDI_ccNumber         = 3;
                current_ltrm_alpha_oled       = 'E';
                current_MIDI_ccNumber_char[0] = '0';
                current_MIDI_ccNumber_char[1] = '3';
             } else {
                // Revert encoder to default behavior on release
                midi_encoder_active = false;
            }
            return false;
            break;
        case MIDI_CC4:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                current_MIDI_ccNumber         = 4;
                current_ltrm_alpha_oled       = 'R';
                current_MIDI_ccNumber_char[0] = '0';
                current_MIDI_ccNumber_char[1] = '4';
             } else {
                // Revert encoder to default behavior on release
                midi_encoder_active = false;
            }
            return false;
            break;
        case MIDI_CC5:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                current_MIDI_ccNumber         = 5;
                current_ltrm_alpha_oled       = 'T';
                current_MIDI_ccNumber_char[0] = '0';
                current_MIDI_ccNumber_char[1] = '5';
             } else {
                // Revert encoder to default behavior on release
                midi_encoder_active = false;
            }
            return false;
            break;
        case MIDI_CC6:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                current_MIDI_ccNumber         = 6;
                current_ltrm_alpha_oled       = 'Y';
                current_MIDI_ccNumber_char[0] = '0';
                current_MIDI_ccNumber_char[1] = '6';
             } else {
                // Revert encoder to default behavior on release
                midi_encoder_active = false;
            }
            return false;
            break;
        case MIDI_CC7:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                current_MIDI_ccNumber         = 7;
                current_ltrm_alpha_oled       = 'U';
                current_MIDI_ccNumber_char[0] = '0';
                current_MIDI_ccNumber_char[1] = '7';
             } else {
                // Revert encoder to default behavior on release
                midi_encoder_active = false;
            }
            return false;
            break;
        case MIDI_CC8:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                current_MIDI_ccNumber         = 8;
                current_ltrm_alpha_oled       = 'I';
                current_MIDI_ccNumber_char[0] = '0';
                current_MIDI_ccNumber_char[1] = '8';
             } else {
                // Revert encoder to default behavior on release
                midi_encoder_active = false;
            }
            return false;
            break;
        case MIDI_CC9:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                current_MIDI_ccNumber         = 9;
                current_ltrm_alpha_oled       = 'O';
                current_MIDI_ccNumber_char[0] = '0';
                current_MIDI_ccNumber_char[1] = '9';
             } else {
                // Revert encoder to default behavior on release
                midi_encoder_active = false;
            }
            return false;
            break;
        case MIDI_CC10:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                current_MIDI_ccNumber         = 10;
                current_ltrm_alpha_oled       = 'P';
                current_MIDI_ccNumber_char[0] = '1';
                current_MIDI_ccNumber_char[1] = '0';
             } else {
                // Revert encoder to default behavior on release
                midi_encoder_active = false;
            }
            return false;
            break;
        case MIDI_CC11:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                current_MIDI_ccNumber         = 11;
                current_ltrm_alpha_oled       = 'A';
                current_MIDI_ccNumber_char[0] = '1';
                current_MIDI_ccNumber_char[1] = '1';
             } else {
                // Revert encoder to default behavior on release
                midi_encoder_active = false;
            }
            return false;
            break;
        case MIDI_CC12:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                current_MIDI_ccNumber         = 12;
                current_ltrm_alpha_oled       = 'S';
                current_MIDI_ccNumber_char[0] = '1';
                current_MIDI_ccNumber_char[1] = '2';
             } else {
                // Revert encoder to default behavior on release
                midi_encoder_active = false;
            }
            return false;
            break;
        case MIDI_CC13:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                current_MIDI_ccNumber         = 13;
                current_ltrm_alpha_oled       = 'D';
                current_MIDI_ccNumber_char[0] = '1';
                current_MIDI_ccNumber_char[1] = '3';
             } else {
                // Revert encoder to default behavior on release
                midi_encoder_active = false;
            }
            return false;
            break; // Fall through for unhandled keycodes
        case MIDI_CC14:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                current_MIDI_ccNumber         = 14;
                current_ltrm_alpha_oled       = 'D';
                current_MIDI_ccNumber_char[0] = '1';
                current_MIDI_ccNumber_char[1] = '4';
             } else {
                // Revert encoder to default behavior on release
                midi_encoder_active = false;
            }
            return false;
            break; // Fall through for unhandled keycodes
        case MIDI_CC15:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                current_MIDI_ccNumber         = 15;
                current_ltrm_alpha_oled       = 'D';
                current_MIDI_ccNumber_char[0] = '1';
                current_MIDI_ccNumber_char[1] = '5';
             } else {
                // Revert encoder to default behavior on release
                midi_encoder_active = false;
            }
            return false;
            break; // Fall through for unhandled keycodes
        case MIDI_CC16:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                current_MIDI_ccNumber         = 16;
                current_ltrm_alpha_oled       = 'D';
                current_MIDI_ccNumber_char[0] = '1';
                current_MIDI_ccNumber_char[1] = '6';
             } else {
                // Revert encoder to default behavior on release
                midi_encoder_active = false;
            }
            return false;
            break; // Fall through for unhandled keycodes
        case MIDI_CC17:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                current_MIDI_ccNumber         = 17;
                current_ltrm_alpha_oled       = 'D';
                current_MIDI_ccNumber_char[0] = '1';
                current_MIDI_ccNumber_char[1] = '7';
             } else {
                // Revert encoder to default behavior on release
                midi_encoder_active = false;
            }
            return false;
            break; // Fall through for unhandled keycodes
        case MIDI_CC18:
            if (record->event.pressed) {
                // Set encoder to control MIDI_CC8
                midi_encoder_active = true;
                current_MIDI_ccNumber         = 18;
                current_ltrm_alpha_oled       = 'D';
                current_MIDI_ccNumber_char[0] = '1';
                current_MIDI_ccNumber_char[1] = '8';
             } else {
                // Revert encoder to default behavior on release
                midi_encoder_active = false;
            }
            return false;
            break; // Fall through for unhandled keycodes
        case MIDI_CC19:
            if (record->event.pressed) {
                midi_send_cc(&midi_device, MIDI_CHANNEL, 19, 127);
            } else {
                midi_send_cc(&midi_device, MIDI_CHANNEL, 19, 0);
            }
            return false;
            break;
        case MIDI_CC20:
            if (record->event.pressed) {
                midi_send_cc(&midi_device, MIDI_CHANNEL, 20, 127);
            } else {
                midi_send_cc(&midi_device, MIDI_CHANNEL, 20, 0);
            }
            return false;
            break;
        case MIDI_CC21:
            if (record->event.pressed) {
                midi_send_cc(&midi_device, MIDI_CHANNEL, 21, 127);
            } else {
                midi_send_cc(&midi_device, MIDI_CHANNEL, 21, 0);
            }
            return false;
            break;
        case MIDI_CC22:
            if (record->event.pressed) {
                midi_send_cc(&midi_device, MIDI_CHANNEL, 22, 127);
            } else {
                midi_send_cc(&midi_device, MIDI_CHANNEL, 22, 0);
            }
            return false;
            break;
        case MIDI_CC23:
            if (record->event.pressed) {
                midi_send_cc(&midi_device, MIDI_CHANNEL, 23, 127);
            } else {
                midi_send_cc(&midi_device, MIDI_CHANNEL, 23, 0);
            }
            return false;
            break;
        case MIDI_CC24:
            if (record->event.pressed) {
                midi_send_cc(&midi_device, MIDI_CHANNEL, 24, 127);
            } else {
                midi_send_cc(&midi_device, MIDI_CHANNEL, 24, 0);
            }
            return false;
            break;
        case MIDI_CC25:
            if (record->event.pressed) {
                midi_send_cc(&midi_device, MIDI_CHANNEL, 25, 127);
            } else {
                midi_send_cc(&midi_device, MIDI_CHANNEL, 25, 0);
            }
            return false;
            break;
		case CULL_MACRO_1: //REJECT, MARK RED, AND 1 STAR PHOTO
		    if (record->event.pressed) {
		        tap_code(KC_1);
		        tap_code(KC_6);
		        tap_code(KC_X);
		    }
		    break;
		case CULL_MACRO_2: //UNPICK, MARK YELLOW, AND 3 STAR PHOTO
		    if (record->event.pressed) {
		        tap_code(KC_3);
		        tap_code(KC_7);
		        tap_code(KC_U);
		    }
		    break;
		case CULL_MACRO_3: //PICK, MARK GREEN, 5 STAR PHOTO
		    if (record->event.pressed) {
		        tap_code(KC_5);
		        tap_code(KC_8);
		        tap_code(KC_P);
		    }
		    break;
		case CULL_MACRO_4: //PICK, MARK GREEN, 5 STAR PHOTO, ADD TO QUICK COLLECTION (LR) or My Selections (Aftershoot)
		    if (record->event.pressed) {
		        tap_code(KC_5);
		        tap_code(KC_9);
		        tap_code(KC_B);
		        tap_code(KC_P);
		    }
		    break;
		case SHTCT_MACRO_1: // Launch YOUTUBE MUSIC via PowerToys
		    if (record->event.pressed) {
		        register_code(KC_LCTL);
		        tap_code(KC_KP_0);
		        unregister_code(KC_LCTL);
		    }
		    break;
		case SHTCT_MACRO_2:  // Launch Aftershoot via PowerToys
		    if (record->event.pressed) {
		        register_code(KC_LCTL);
		        tap_code(KC_KP_1);
		        unregister_code(KC_LCTL);
		    }
		    break;
		case SHTCT_MACRO_3: // Launch LR Classic via PowerToys
		    if (record->event.pressed) {
		        register_code(KC_LCTL);
		        tap_code(KC_KP_2);
		        unregister_code(KC_LCTL);
		    }
		    break;
		case SHTCT_MACRO_4: // Launch Photoshop via PowerToys
		    if (record->event.pressed) {
		        register_code(KC_LCTL);
		        tap_code(KC_KP_3);
		        unregister_code(KC_LCTL);
		    }
		    break;
		case SHTCT_MACRO_5: // Launch Adobe DNG Converter via PowerToys
		    if (record->event.pressed) {
		        register_code(KC_LCTL);
		        tap_code(KC_KP_4);
		        unregister_code(KC_LCTL);
		    }
		    break;
		case SHTCT_MACRO_6:  // Launch Blitzit Task Organization via PowerToys
		    if (record->event.pressed) {
		        register_code(KC_LCTL);
		        tap_code(KC_KP_5);
		        unregister_code(KC_LCTL);
		    }
		    break;
		case SHTCT_MACRO_7: // Launch NaviUpgrade Admin via PowerToys
		    if (record->event.pressed) {
		        register_code(KC_LCTL);
		        tap_code(KC_KP_6);
		        unregister_code(KC_LCTL);
		    }
		    break;
		case SHTCT_MACRO_8: // Launch Custom Web Dashboard via PowerToys
		    if (record->event.pressed) {
		        register_code(KC_LCTL);
		        tap_code(KC_KP_7);
		        unregister_code(KC_LCTL);
		    }
		    break;
        case L_IND:
            if (record->event.pressed) {
                // Code to toggle between the two Layer Indicators
                led_mode = !led_mode; // Toggle the mode

                // Updateing lighting layer when key is pressed
                if (led_mode) {
                    if (layer_state_is(0)) {
                        rgblight_set_layer_state(4, true);
                    }
                    if (layer_state_is(1)) {
                        rgblight_set_layer_state(5, true);
                    }
                    if (layer_state_is(2)) {
                        rgblight_set_layer_state(6, true);
                    }
                    if (layer_state_is(3)) {
                        rgblight_set_layer_state(7, true);
                    }
                } else {
                    rgblight_set_layer_state(4, false);
                    rgblight_set_layer_state(5, false);
                    rgblight_set_layer_state(6, false);
                    rgblight_set_layer_state(7, false);

                    if (layer_state_is(0)) {
                        rgblight_blink_layer(0, 1000);
                    }
                    if (layer_state_is(1)) {
                        rgblight_blink_layer(1, 1000);
                    }
                    if (layer_state_is(2)) {
                        rgblight_blink_layer(2, 1000);
                    }
                    if (layer_state_is(3)) {
                        rgblight_blink_layer(3, 1000);
                    }
                }
            } else {
            }
            return false; // Skip all further processing of this key
        
        case COPY_ACTION: {
            static uint16_t press_time = 0;    // Timer for press duration
            static uint16_t last_tap_time = 0; // Timer for detecting double-tap
            static bool is_double_tap = false; // Tracks if a double-tap was detected

            if (record->event.pressed) {
                uint16_t current_time = timer_read();

                // Check for double-tap
                if (timer_elapsed(last_tap_time) < TAPPING_TERM) {
                    is_double_tap = true; // Mark as a double-tap
                } else {
                    is_double_tap = false; // Reset double-tap state
                }

                // Record press time
                press_time = current_time;

                // Always update last tap time
                last_tap_time = current_time;
            } else {
                uint16_t release_time = timer_elapsed(press_time);

                if (is_double_tap) {
                    // Handle double-tap: Send Ctrl + V
                    register_code(KC_LCTL);
                    tap_code(KC_V);
                    unregister_code(KC_LCTL);

                    // Reset double-tap state
                    is_double_tap = false;
                } else if (release_time >= TAPPING_TERM) {
                    // Handle long press: Send Ctrl + C
                    register_code(KC_LCTL);
                    tap_code(KC_C);
                    unregister_code(KC_LCTL);
                } else {
                    // Handle short press: Send Ctrl + C + Enter
                    wait_ms(TAPPING_TERM); // Ensure no second tap comes
                    if (!is_double_tap) {
                        register_code(KC_LCTL);
                        tap_code(KC_C);
                        unregister_code(KC_LCTL);
                        wait_ms(200);
                        tap_code(KC_ENT);
                    }
                }
            }

            return false; // Skip further processing
        }

        case TAP_HOLD_CTRL_SHIFT_D:
		    if (record->event.pressed) {
		        // Start the timer when the key is pressed
		        tap_hold_timer = timer_read();

		        // Set the MIDI encoder active for holding behavior
		        midi_encoder_active = true;
		        current_MIDI_ccNumber = 8;
		        current_ltrm_alpha_oled = 'I';
		        current_MIDI_ccNumber_char[0] = '0';
		        current_MIDI_ccNumber_char[1] = '8';
		    } else {
		        // Check elapsed time when the key is released
		        if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
		            // Short press: Send CTRL + SHIFT + D
		            register_code(KC_LCTL);
		            register_code(KC_LSFT);
		            tap_code(KC_D);
		            unregister_code(KC_LSFT);
		            unregister_code(KC_LCTL);
		        }

        // Reset MIDI encoder state when the key is released
        midi_encoder_active = false;
    }

    // Handle "held" behavior for long press
    if (record->event.pressed && timer_elapsed(tap_hold_timer) >= TAPPING_TERM) {
        // Long hold: Activate MIDI_CC8 and enable MIDI encoder
        midi_encoder_active = true;
        current_MIDI_ccNumber = 8;
        current_ltrm_alpha_oled = 'I';
        current_MIDI_ccNumber_char[0] = '0';
        current_MIDI_ccNumber_char[1] = '8';
    }

            return false; // Skip further processing

        case PASTE:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("v")); // Sends Ctrl + V
            }
            return false; // Skip further processing of this key

        case SYNC:
            if (record->event.pressed) {
                sync_timer = timer_read();
            } else {
                if (timer_elapsed(sync_timer) < TAPPING_TERM) {
                    tap_code16(C(S(KC_S)));
                    wait_ms(200);
                    tap_code(KC_ENT);
                } else {
                    tap_code16(C(S(KC_S)));
                }
            }
            return false;

        case L_CYC:
            // Our logic will happen on presses, nothing is done on releases
            if (!record->event.pressed) {
                // We've already handled the keycode (doing nothing), let QMK know so no further code is run unnecessarily
                return false;
            }

            uint8_t current_layer = get_highest_layer(layer_state);

            // Check if we are within the range, if not quit
            if (current_layer > LAYER_CYCLE_END || current_layer < LAYER_CYCLE_START) {
                return false;
        }
            uint8_t next_layer = current_layer + 1;
                if (next_layer > LAYER_CYCLE_END) {
                    next_layer = LAYER_CYCLE_START;
                }
                layer_move(next_layer);
                return false;
        
        default:
            return true; // Process all other keycodes normally
    }
    return true; // Ensure all paths return a value
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if (led_mode) {
        // Code for Static Layer Indicators

        rgblight_set_layer_state(4, layer_state_cmp(state, 0));
        rgblight_set_layer_state(5, layer_state_cmp(state, 1));
        rgblight_set_layer_state(6, layer_state_cmp(state, 2));
        rgblight_set_layer_state(7, layer_state_cmp(state, 3));

    } else {
        // Code for Blinking Layer Indicators

        uint8_t layer = get_highest_layer(state);

        switch (layer) {
            case 0:
                rgblight_blink_layer(0, 500);
                break;
            case 1:
                rgblight_blink_layer(1, 500);
                break;
            case 2:
                rgblight_blink_layer(2, 500);
                break;
            case 3:
                rgblight_blink_layer(3, 500);
                break;

            default:
                rgblight_blink_layer(0, 500);
        }
    }
    return state;
}


#ifdef OLED_DRIVER_ENABLE
void oled_task_user(void) {
    static const char PROGMEM image[] = {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4};
    oled_write_P(image, false);
    uint8_t led_usb_state = host_keyboard_leds();
    switch (biton32(layer_state)) {
        case _LTRM:
            oled_write_P(PSTR("CC "), false);
            oled_write(current_MIDI_ccNumber_char, false);
            oled_write_P(PSTR("    ["), false);
            if (current_ltrm_alpha_oled == '\0') {
                oled_write_char('*', false);
            } else {
                oled_write_char(current_ltrm_alpha_oled, false);
            }
            oled_write_P(PSTR("]   "), false);
            break;
            // case _MIDI:
            //     oled_write_P(PSTR("               "), false);
            //     // oled_write(itoa(midi_config.octave, vel, 10), false);
            //     break;
    }
    switch (get_highest_layer(layer_state)) {
        case _LTRM:
            oled_write_P(PSTR("LTRM"), false);
            break;
        // case _MIDI:
        //     oled_write_P(PSTR("4 MIDI"), false);
        //     break;
        default:
            oled_write_ln_P(PSTR("  NONE"), false);
    }
}
#endif

// Encoder configuration
// int16_t encoder_val = 64;
// MIDI key handling

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { // Ensure this is the correct encoder
        if (midi_encoder_active) {
            // MIDI CC Behavior
            if (clockwise) {
                midi_send_cc(&midi_device, MIDI_CHANNEL, current_MIDI_ccNumber, 65);
            } else {
                midi_send_cc(&midi_device, MIDI_CHANNEL, current_MIDI_ccNumber, 63);
            }
        } else if (encoder_layer_cycling) {
            // Layer Cycling Behavior when L_CYC is held
            uint8_t current_layer = get_highest_layer(layer_state);
            uint8_t target_layer;

            if (clockwise) {
                target_layer = current_layer + 1;
                if (target_layer > LAYER_CYCLE_END) {
                    target_layer = LAYER_CYCLE_START; // Wrap around to the first layer
                }
            } else {
                target_layer = current_layer - 1;
                if (target_layer < LAYER_CYCLE_START) {
                    target_layer = LAYER_CYCLE_END; // Wrap around to the last layer
                }
            }

            layer_move(target_layer);

            // Reset encoder_layer_cycling after one turn
            encoder_layer_cycling = false;
        } else {
            // Layer-Specific Encoder Behavior
            switch (biton32(layer_state)) {
                case _CULL:
                    if (clockwise) {
                        tap_code(KC_RGHT);
                    } else {
                        tap_code(KC_LEFT);
                    }
                    break;
                case _PRST:
                    if (clockwise) {
                        tap_code(KC_RGHT);
                    } else {
                        tap_code(KC_LEFT);
                    }
                    break;
                case _LTRM:
                    if (clockwise) {
                        tap_code(KC_RGHT);
                    } else {
                        tap_code(KC_LEFT);
                    }
                    break;
                case _SHTCT:
                    if (clockwise) {
                        tap_code(KC_VOLU);
                    } else {
                        tap_code(KC_VOLD);
                    }
                    break;
                default:
                    break;
            }
        }
    }
    return false; // Return false to indicate no additional processing is needed
}
