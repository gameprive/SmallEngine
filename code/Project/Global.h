#pragma once

static const bool VSYNC = false;
static const float TARGET_FRAME_INTERVAL = 0.016665f * (VSYNC ? 1.0f : 0.0f);
static const float FRAME_TIME_ALPHA = 0.25f;
static const float MOVE_SPEED = 16.0f;

#define PI       3.14159265358979323846f
#define PI_2     1.57079632679489661923f