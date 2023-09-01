#pragma once
#include <stdint.h>
#include <stdlib.h>
#ifdef __SDCC
#include <rand.h>
#endif
#define MAX_EPISODE_LEN 200
#define N_STATE_0 16
#define N_STATE_1 16
#define N_ACTIONS 3
#define EPSILON 0.26 // 0.260
#define EPSILON_RAND_MAX (int)(EPSILON * RAND_MAX)
#define ALPHA_BITSHIFT 6 // 0.015625 ~= 0.0117; x * alphaの代わりに x >> ALPHA_BITSHIFTを使う
#define GAMMA_BITSHIFT 7 // 1-0.0078125=0.984375 ~= 0.992; x * gammaの代わりに x - (x >> GAMMA_BITSHIFT)を使う
#define REWARD_PER_STEP -128
#define REWARD_AT_GOAL (10*128)
