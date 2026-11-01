/* func_001B98A0 (Claude agent, 2026-10-17): matched. The p/k s0/s1 swap came from declaring func_001443D8 with (p, k): it takes only p (as in func_0018D760), and the extra k use raised k's allocation priority. */
struct O { char pad[0x180]; void *x180; };
struct P { char pad[0x2C4]; int x2C4; char pad1[0x12F0 - 0x2C8]; int x12F0; char pad2[0x13D0 - 0x12F4]; int x13D0; char pad3[0x13F0 - 0x13D4]; struct O *x13F0; };
extern int D_002DFBC0;
extern char D_002DE2E0[];
int func_001443D8(struct P *p);
int func_0019C8D0(void *p, int k);
int func_00194558(void *p, int k);
int func_0019EEF0(void *p, int k);
int func_00132B10(void *p, int n);
int func_0014FD50(void *p);
/** @sly2-annot addr=0x1B98A0
 * Purpose: MURRAY vt99: can gadget/power k be used in the current state (per-state allow lists, components
 *   2/13/20)
 * Info: proposed name MURRAY_FCanUseGadget (T1); class MURRAY slot99; TU murray
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
int func_001B98A0(struct P *p, int k)
{
    if (func_001443D8(p)) return 0;
    switch (p->x13D0) {
    case 0: case 1: case 9: case 10:
        break;
    case 2:
        if (!func_0019C8D0((char *)p + 0x2C0, k)) return 0;
        break;
    case 13:
        if (!func_00194558((char *)p + 0x4C0, k)) return 0;
        break;
    case 20:
        if (!func_0019EEF0((char *)p + 0xF50, k)) return 0;
        break;
    case 5:
        if (k == 3) return 1;
        if (k == 19) { if (func_00132B10(p->x13F0->x180, 0x38)) return 1; }
        return 0;
    default:
        return 0;
    }
    switch (k) {
    case 3:
    {
        int r = 0;
        if (p->x12F0 && (D_002DFBC0 & 2)) r = !func_0014FD50(D_002DE2E0);
        return r;
    }
    case 22:
    {
        int r = 0;
        if (p->x2C4 != 0 && p->x2C4 != 11) r = p->x2C4 != -1;
        return r;
    }
    case 1: case 2: case 15: case 16: case 20: case 21:
        break;
    default:
        return 0;
    }
    return 1;
}
