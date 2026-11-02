struct S { char pad[0x40]; int x40; char p1[0x150 - 0x44]; int x150; int x154; int x158; int x15C;
    char p2[0x1A4 - 0x160]; int x1A4; int x1A8; int x1AC; int x1B0; int x1B4; char p3[0x1C8 - 0x1B8]; int x1C8; float x1CC;
    char p4[0x204 - 0x1D0]; int x204; int x208; int x20C; int x210; int x214; int x218; float x21C; int x220; int x224; };
extern float D_002A7710;
void InitSo(struct S *p);
void func_001F7918(struct S *p, int a, int b);
static inline int FPtr(int v) { return !(v & 1); }
/** @sly2-annot addr=0x1C4E50
 * Purpose: DPRIZE::Init (vt7): InitSo + 0x1F7918 setup
 * Info: proposed name InitDprize (T1); class DPRIZE slot7; TU prize
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/game-progression.md#7-source-files, docs/engine/game-progression.md#8-key-functions,
 *   docs/engine/gameplay-objects.md#gameplay-object-classes
 * @end-sly2-annot */
void InitDprize(struct S *p)
{
    int v;
    InitSo(p);
    func_001F7918(p, 0x17E, 8);
    v = p->x40;
    p->x21C = 30.0f;
    p->x1C8 = 1;
    p->x1CC = D_002A7710;
    p->x150 = -1;
    p->x158 = 0;
    p->x15C = -1;
    p->x1A4 = -1;
    p->x1AC = -1;
    p->x1B4 = -1;
    p->x204 = -1;
    p->x20C = -1;
    p->x214 = -1;
    p->x224 = -1;
    if (FPtr(v)) *(int *)v |= 2; else p->x40 = v | 2;
}
