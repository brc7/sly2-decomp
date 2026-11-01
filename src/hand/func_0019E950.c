typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(void *, int, int, qword, qword, qword); };
struct O { int a, b, c; struct VTE *vt; };
struct S { struct O *o; int pad[2]; int c; float g; int pad2[7]; qword q30; };
void func_001E6F10(int, qword *, qword *, float);
qword func_001A6F00(struct O *, int, qword);
typedef union { qword q; float f[4]; } V;
/** @sly2-annot addr=0x19E950
 * Purpose: Add the pipe constraint: evaluate curve at s, vt82(7, curve, pos, tangent, local point)
 * Info: proposed name UpdateJtInternalXpsPipe (T1); TU pipe_jt
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4b_rest+verdicts_followups
 * See: docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#pipe_jt-0x19de10-0x19f0a8
 * @end-sly2-annot */
void func_0019E950(struct S *p) {
    V a, b, u;
    func_001E6F10(p->c, &a.q, &b.q, p->g);
    u.q = func_001A6F00(p->o, 0, p->q30);
    { struct O *o = p->o; struct VTE *e = &o->vt[82]; e->pfn((char *)o + e->d, 7, p->c, a.q, b.q, u.q); }
}
