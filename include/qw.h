#ifndef QW_H
#define QW_H
#include "common.h"
/* 128-bit quadword (DMA tags, GIF/VIF packets) */
union QW {
    ulong aul[2];
    long al[2];
    uint aui[4];
    uint aun[4];
    int an[4];
    float ag[4];
    ushort aus[8];
} __attribute__((aligned(16)));
#endif
