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
#define EPSILON 0.168 // 0.168
#define EPSILON_RAND_MAX (int)(EPSILON * RAND_MAX)
#define ALPHA_BITSHIFT 6 // 0.015625 ~= 0.0126; x * alphaの代わりに x >> ALPHA_BITSHIFTを使う
#define GAMMA_BITSHIFT 5 // 1-0.03125=0.96875 ~= 0.963; x * gammaの代わりに x - (x >> GAMMA_BITSHIFT)を使う
#define REWARD_PER_STEP -128
#define REWARD_AT_GOAL (10*128)
