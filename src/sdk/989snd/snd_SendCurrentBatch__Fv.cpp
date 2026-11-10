// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern "C" {
extern int gCommandFillBuffer, D_002C46B8;
extern int *gSndCommandBuffePtr[2];
extern int gCommandBuffeBytesAvail[2];
extern void *gReturnValuesPtr[2];
extern char D_00443800[];
extern char D_002AA1C8[];
void snd_PrepareReturnBuffer(void *, int, int *);
void printf(const char *, ...);
void func_00234CA0(int) __asm__("FlushCache");
int func_00237488(void *) __asm__("sceSifCheckStatRpc");
int func_00237288(void *, int, int, void *, int, void *, int, void *, void *) __asm__("sceSifCallRpc");
}

void snd_SendCurrentBatch()
{
    snd_PrepareReturnBuffer(gReturnValuesPtr[gCommandFillBuffer], *gSndCommandBuffePtr[gCommandFillBuffer], gSndCommandBuffePtr[gCommandFillBuffer]);
    while (func_00237488(D_00443800)) {
        if (!D_002C46B8)
            printf(D_002AA1C8);
        func_00234CA0(0);
    }
    func_00237288(D_00443800, 0x4D, 1, gSndCommandBuffePtr[gCommandFillBuffer], 0x1000 - gCommandBuffeBytesAvail[gCommandFillBuffer],
                  gReturnValuesPtr[gCommandFillBuffer], *gSndCommandBuffePtr[gCommandFillBuffer] * 4 + 8, 0, 0);
    int c = gCommandFillBuffer != 1;
    gCommandFillBuffer = c;
    *gSndCommandBuffePtr[c] = 0;
    gCommandBuffeBytesAvail[c] = 0xFFC;
}
