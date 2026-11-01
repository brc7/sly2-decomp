struct S { char pad[0x40]; int f; char pad2[0x10C]; int a, b, c, d; int pad3[4]; float e, g; int h; };
void func_001F2108(struct S *p);
static inline int FIsPtr(int f) { return !(f & 1); }
/** @sly2-annot addr=0x223F70
 * Purpose: InitSo; +0x?=-10, 3000.0, -1, -1; grf|=6
 * Info: proposed name InitCid100 (T2); class CID100 slot7; TU tu_223F70
 * Confidence: low (draft) [T2 0.85] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/camera.md#4-source-files, docs/engine/gameplay-objects.md#source-files,
 *   docs/engine/gameplay-objects.md#key-functions
 * @end-sly2-annot */
void func_00223F70(struct S *p)
{
    int f;
    func_001F2108(p);
    f = p->f;
    p->h = -10;
    p->e = 3000.0f;
    p->c = -1;
    p->a = -1;
    if (FIsPtr(f)) *(int *)f |= 6;
    else p->f = f | 6;
}
