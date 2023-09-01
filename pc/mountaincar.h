#pragma once

#define MAX_EPISODE_LEN 200

typedef struct {
    double position;
    double velocity;
//    double max_position;
    int steps;
    int done;
} MountainCarState;

typedef int MountainCarAction; // 0=left, 1=nothing, 2=right

double MountainCarStep(MountainCarState *state, MountainCarAction action);
void MountainCarReset(MountainCarState *state);
