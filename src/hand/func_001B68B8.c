typedef int qword __attribute__((mode(TI)));
typedef struct MSG { void *x0; int x4; int x8; } MSG;
typedef struct P {
    char pad0[0x1030];
    void *x1030;
    void *x1034;
    char pad1038[0x13D0 - 0x1038];
    int x13D0;
    char pad13D4[0x1448 - 0x13D4];
    int x1448;
    int pad144C;
    int x1450;
    char pad1454[0x1524 - 0x1454];
    float x1524;
    float x1528;
    int x152C;
    char pad1530[4];
    int x1534;
    char pad1538[0x1818 - 0x1538];
    int x1818;
    int x181C;
    int x1820;
    char pad1824[0x190C - 0x1824];
    void *x190C;
    char x1910[0x10];
} P;
extern char D_003DD490[];
extern float D_002C0990[];
extern float D_002C099C;
void func_00140F90(P *p, int msg, MSG *pm);
void func_001B6DA8(P *p, int k);
qword func_001A62E8(P *p);
void func_00156B98(void *g, int z, qword q, P *p);
void func_001B7970(P *p);
void func_001B7AE8(P *p);
void func_002205A8(void *p);
void func_001B93E8(P *p, int k);
void func_001B6078(P *p, int s);

/** @sly2-annot addr=0x1B68B8
 * Purpose: MURRAY vt5: base HandleStepplayerMessage; msg 9 (anim events from own ASEGA): 0x1A4 flop dive,
 *   0x1A5/0x1C8 stimulus, 0x1EB release thrown object, 0x1C6/0x1C7 lift events, 0x240-0x242 attack window
 *   flags; msg 10 from SMA in struggle state
 * Info: proposed name HandleMurrayMessage (T1); class MURRAY slot5; TU murray
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B68B8(P *p, int msg, MSG *pm)
{
    func_00140F90(p, msg, pm);
    switch (msg) {
    case 9:
        if (pm->x0 != p->x1030 && pm->x0 != p->x190C) return;
        switch (pm->x8) {
        case 0x1A4:
            func_001B6DA8(p, 1);
            break;
        case 0x1A5:
            func_00156B98(D_003DD490, 0, func_001A62E8(p), p);
            break;
        case 0x1EB:
            switch (p->x13D0) {
            case 7:
                func_001B7970(p);
                break;
            case 8:
                func_001B7AE8(p);
                break;
            case 15:
                func_002205A8(p->x1910);
                break;
            }
            break;
        case 0x1C6:
            p->x1448 = 1;
            break;
        case 0x1C8:
            if (!p->x1450) {
                func_00156B98(D_003DD490, 0, func_001A62E8(p), p);
                func_001B93E8(p, 1);
            }
            break;
        case 0x1C7:
            if (p->x1450 == 1) func_001B93E8(p, 2);
            break;
        case 0x240:
            p->x1818 = 1;
            break;
        case 0x241:
            p->x181C = 1;
            break;
        case 0x242:
            p->x1820 = 1;
            break;
        }
        break;
    case 10:
        if (pm->x0 != p->x1034) return;
        if (p->x13D0 != 6) return;
        if (pm->x8 == pm->x4) return;
        switch (pm->x8) {
        case 0x1CA:
        case 0x1D0:
            func_001B6078(p, 0);
            break;
        case 0x1CB:
        case 0x1CC:
        case 0x1CD:
        case 0x1CE:
        case 0x1CF: {
            float f = D_002C0990[p->x1534];
            p->x1524 = p->x1528 = f;
            p->x152C = (int)(D_002C099C * f);
            break;
        }
        }
        break;
    }
}
