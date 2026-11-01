struct S { char pad[0x154]; float f154; char pad2[0x174 - 0x158]; int f174; char pad3[0x1e8 - 0x178]; int f1e8; };
extern float D_002A4B20;
void func_001CB518(struct S *);
void func_0013BC60(struct S *, struct S *);
void func_001F5720(struct S *, int);
void func_001F57C0(struct S *, int);
void func_001F6860(struct S *, int);
void func_001A58D0(struct S *, int, int);
/** @sly2-annot addr=0x13BB80
 * Purpose: BOMB vt26: base, clear detonation, tDetonate=const, owner=self, unlock constraints, unparent
 * Info: proposed name ResetBomb (T1); class BOMB slot26; TU bomb
 * Notes: runtime5a #15 CONSISTENT: spent darts out of world, tDetonate FLT_MAX, parent 0
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources: r2b_rest+runtime
 * See: docs/reference/functions/gameplay-objects.md#bomb-0x13a9e8-0x13bdf0
 * @end-sly2-annot */
void func_0013BB80(struct S *p) {
    func_001CB518(p);
    p->f174 = 0;
    p->f154 = D_002A4B20;
    func_0013BC60(p, p);
    func_001F5720(p, 0);
    func_001F57C0(p, 0);
    func_001F6860(p, 0);
    p->f1e8 = 0;
    func_001A58D0(p, 0, 0xf);
}
