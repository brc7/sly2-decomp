typedef int ti128 __attribute__((mode(TI)));
struct S { char pad[0x40]; int f40; char pad2[0x170 - 0x44]; ti128 v170; char pad3[4]; int f184; };
extern ti128 D_002A2DE0;
void func_001F2108(struct S *);
/** @sly2-annot addr=0x13BDF0
 * Purpose: cid 0x67 vt7: InitSo, default scale vector (+0x170), state -1, set a flag bit
 * Info: proposed name InitBooty (T1); class BOOTY slot7; TU booty
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/game-progression.md#7-source-files,
 *   docs/engine/gameplay-objects.md#gameplay-object-classes, docs/engine/gameplay-objects.md#source-files
 * @end-sly2-annot */
void func_0013BDF0(struct S *p) {
    int x;
    func_001F2108(p);
    p->v170 = D_002A2DE0; p->f184 = -1; x = p->f40;
    if ((x ^ 1) & 1) *(int *)x |= 2;
    else p->f40 = x | 2;
}
