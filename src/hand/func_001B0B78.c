struct E { int a; int b; int c; };
struct S { char pad[0x40]; unsigned int x40; char pad1[0x150 - 0x44]; float f150, f154, f158, f15C, f160, f164, f168; char pad2[0x17C - 0x16C]; struct E ae[6]; char pad3[0x1CC - 0x1C4]; float f1CC, f1D0; char pad4[0x220 - 0x1D4]; int x220; char pad5[0xC]; int x230; };
extern int D_002C0230[6];
void InitSo(struct S *p);
/** @sly2-annot addr=0x1B0B78
 * Purpose: CID 96 vt7 Init: InitSo + defaults
 * Info: proposed name InitCid96 (T2); class CID96 slot7; TU tu_1B0B78
 * Notes: names_v2 v1 proposed FCheckStepXpBase__FP4STEPP2XPi (0.074)
 * Confidence: low (draft) [T2 0.60] - evidence: static naming pass; sources: r5b_rest
 * See: docs/engine/gameplay-objects.md#source-files, docs/engine/hud-ui.md#4-source-files,
 *   docs/reference/functions/gameplay-objects.md#tu_1b0b78-0x1b0b78-0x1b1db8
 * @end-sly2-annot */
void func_001B0B78(struct S *p)
{
    int i;
    struct E *pe;
    int *pi;
    InitSo(p);
    p->f150 = 400.0f;
    p->f154 = 800.0f;
    p->f158 = 2.0f;
    p->f15C = 6.0f;
    p->f160 = 6.0f;
    p->f164 = 10.0f;
    p->f168 = 1.0f;
    pe = p->ae;
    pi = D_002C0230;
    for (i = 5; i >= 0; i--) {
        pe->a = -1;
        pe->c = *pi++;
        pe++;
    }
    p->f1CC = 300.0f;
    p->f1D0 = 1000.0f;
    p->x220 = -1;
    p->x230 = -1;
    if ((p->x40 ^ 1) & 1) *(int *)p->x40 |= 2;
    else p->x40 |= 2;
}
