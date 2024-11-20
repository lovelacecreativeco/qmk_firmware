#include "midi.h"
#include "quantum.h"
#include "quantum_keycodes.h"

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

// Define a single layer
enum layers {
    _LTRM // Single test layer
};

// Define custom keycodes
enum midi_cc_keycodes_LTRM { MIDI_CC1 = SAFE_RANGE, MIDI_CC2, MIDI_CC3, MIDI_CC4, MIDI_CC5, MIDI_CC6, MIDI_CC7, MIDI_CC8, MIDI_CC9, MIDI_CC10, MIDI_CC11, MIDI_CC12, MIDI_CC13};

static char current_alpha_oled = '\0';
static char current_ltrm_alpha_oled = '\0';

static uint8_t current_MIDI_ccNumber         = 1;
static char    current_MIDI_ccNumber_char[3] = {'0', '1', '\0'};

// Keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_LTRM] = LAYOUT(
        MIDI_CC1,         // Encoder press sends MIDI CC
        MIDI_CC2, MIDI_CC3, MIDI_CC4, MIDI_CC5,  // Top row keys
        MIDI_CC6, MIDI_CC7, MIDI_CC8, MIDI_CC9,  // Middle row keys
        MIDI_CC10, MIDI_CC11, MIDI_CC12, MIDI_CC13  // Bottom row keys
    ),
};

// MIDI key handling
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MIDI_CC1:
            if (record->event.pressed) {
                current_MIDI_ccNumber         = 1;
                current_ltrm_alpha_oled       = 'Q';
                current_MIDI_ccNumber_char[0] = '0';
                current_MIDI_ccNumber_char[1] = '1';
            } else {
            }
            return false;
            break;
        case MIDI_CC2:
            if (record->event.pressed) {
                current_MIDI_ccNumber         = 2;
                current_ltrm_alpha_oled       = 'W';
                current_MIDI_ccNumber_char[0] = '0';
                current_MIDI_ccNumber_char[1] = '2';
            } else {
            }
            return false;
            break;
        case MIDI_CC3:
            if (record->event.pressed) {
                current_MIDI_ccNumber         = 3;
                current_ltrm_alpha_oled       = 'E';
                current_MIDI_ccNumber_char[0] = '0';
                current_MIDI_ccNumber_char[1] = '3';
            } else {
            }
            return false;
            break;
        case MIDI_CC4:
            if (record->event.pressed) {
                current_MIDI_ccNumber         = 4;
                current_ltrm_alpha_oled       = 'R';
                current_MIDI_ccNumber_char[0] = '0';
                current_MIDI_ccNumber_char[1] = '4';
            } else {
            }
            return false;
            break;
        case MIDI_CC5:
            if (record->event.pressed) {
                current_MIDI_ccNumber         = 5;
                current_ltrm_alpha_oled       = 'T';
                current_MIDI_ccNumber_char[0] = '0';
                current_MIDI_ccNumber_char[1] = '5';
            } else {
            }
            return false;
            break;
        case MIDI_CC6:
            if (record->event.pressed) {
                current_MIDI_ccNumber         = 6;
                current_ltrm_alpha_oled       = 'Y';
                current_MIDI_ccNumber_char[0] = '0';
                current_MIDI_ccNumber_char[1] = '6';
            } else {
            }
            return false;
            break;
        case MIDI_CC7:
            if (record->event.pressed) {
                current_MIDI_ccNumber         = 7;
                current_ltrm_alpha_oled       = 'U';
                current_MIDI_ccNumber_char[0] = '0';
                current_MIDI_ccNumber_char[1] = '7';
            } else {
            }
            return false;
            break;
        case MIDI_CC8:
            if (record->event.pressed) {
                current_MIDI_ccNumber         = 8;
                current_ltrm_alpha_oled       = 'I';
                current_MIDI_ccNumber_char[0] = '0';
                current_MIDI_ccNumber_char[1] = '8';
            } else {
            }
            return false;
            break;
        case MIDI_CC9:
            if (record->event.pressed) {
                current_MIDI_ccNumber         = 9;
                current_ltrm_alpha_oled       = 'O';
                current_MIDI_ccNumber_char[0] = '0';
                current_MIDI_ccNumber_char[1] = '9';
            } else {
            }
            return false;
            break;
        case MIDI_CC10:
            if (record->event.pressed) {
                current_MIDI_ccNumber         = 10;
                current_ltrm_alpha_oled       = 'P';
                current_MIDI_ccNumber_char[0] = '1';
                current_MIDI_ccNumber_char[1] = '0';
            } else {
            }
            return false;
            break;
        case MIDI_CC11:
            if (record->event.pressed) {
                current_MIDI_ccNumber         = 11;
                current_ltrm_alpha_oled       = 'A';
                current_MIDI_ccNumber_char[0] = '1';
                current_MIDI_ccNumber_char[1] = '1';
            } else {
            }
            return false;
            break;
        case MIDI_CC12:
            if (record->event.pressed) {
                current_MIDI_ccNumber         = 12;
                current_ltrm_alpha_oled       = 'S';
                current_MIDI_ccNumber_char[0] = '1';
                current_MIDI_ccNumber_char[1] = '2';
            } else {
            }
            return false;
            break;
        case MIDI_CC13:
            if (record->event.pressed) {
                current_MIDI_ccNumber         = 13;
                current_ltrm_alpha_oled       = 'D';
                current_MIDI_ccNumber_char[0] = '1';
                current_MIDI_ccNumber_char[1] = '3';
            } else {
            }
            return false;
            default:
            break; // Fall through for unhandled keycodes
    }

    return true; // Allow further processing for unhandled keycodes
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
            oled_write_P(PSTR("3 LTRM"), false);
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
    if (index == 0) { // Assuming single encoder
        switch (biton32(layer_state)) {
            case _LTRM:
                if (clockwise) {
                    midi_send_cc(&midi_device, MIDI_CHANNEL, current_MIDI_ccNumber, 65);
                } else {
                    midi_send_cc(&midi_device, MIDI_CHANNEL, current_MIDI_ccNumber, 63);
                }
                break;

            default:
                break;
        }
    }
    return false; // Return false to indicate no additional processing is needed
}
