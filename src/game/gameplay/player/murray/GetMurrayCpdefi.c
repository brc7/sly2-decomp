struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
struct S { char pad[0x4CC]; int x4CC; char pad1[0x1080 - 0x4D0]; float t1080; char pad2[0x10E4 - 0x1084]; int x10E4; char pad3[0x13D0 - 0x10E8]; int k; };
void GetStepplayerCpdefi(struct S *p, int *pf);
void func_00220388(void *p, int *pf);
void func_0019EA00(void *p, int *pf);
/** @sly2-annot addr=0x1B6728
 * Purpose: MURRAY vt50: stamp x1080 while moving, base GetStepplayerCpdefi 0x141640, then set flag bit 1 for
 *   flop/strafe/power/takedown states and defer to JTTHROW / +0xF50 component
 * Info: proposed name GetMurrayCpdefi (T1); class MURRAY slot50; TU murray
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void GetMurrayCpdefi(struct S *p, int *pf)
{
    if (p->k != 0 && p->k != 16 && p->x4CC) p->t1080 = D_002DDED0.t;
    GetStepplayerCpdefi(p, pf);
    switch (p->k) {
    case 11:
        if (p->x10E4 != 4) break;
    case 3: case 9: case 12:
        *pf |= 1;
        break;
    case 15: func_00220388((char *)p + 0x1910, pf); break;
    case 20: func_0019EA00((char *)p + 0xF50, pf); break;
    }
}
