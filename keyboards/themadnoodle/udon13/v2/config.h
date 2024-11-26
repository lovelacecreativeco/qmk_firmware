// Copyright 2023 The Mad Noodle(@the_mad_noodle)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma once

#define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_RAINBOW_SWIRL + 5

// I2C Settings For OLED
#define I2C1_SCL_PIN GP5
#define I2C1_SDA_PIN GP4
#define I2C_DRIVER I2CD0

// OLED Timeout
#define OLED_TIMEOUT 1800000
#define OLED_FADE_OUT
#define OLED_FADE_OUT_INTERVAL 8

#define MATRIX_ROWS 4
#define MATRIX_COLS 4

#define NUM_ENCODERS 1
#define NUM_DIRECTIONS 2
#define DEBOUNCE 10