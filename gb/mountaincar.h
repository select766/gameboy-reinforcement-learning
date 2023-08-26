#pragma once
#include <stdint.h>
#include "config.h"

typedef int16_t Reward;
// typedef double Reward; // for debug on PC
typedef int16_t Position;
typedef int16_t Velocity;
typedef uint8_t MountainCarAction; // 0=left, 1=nothing, 2=right

typedef struct {
    Position position;
    Velocity velocity;
    uint8_t steps;
    uint8_t done;
} MountainCarState;

Reward MountainCarStep(MountainCarState *state, MountainCarAction action);
void MountainCarReset(MountainCarState *state);
