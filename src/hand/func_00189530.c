struct VTE { short d; short pad; void (*pfn)(void *, int); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x19A0]; struct O *a; struct O *b; struct O *c; };
void func_00205D10(struct S *p, int f);
static __inline__ void Call(struct O *o, int n)
{
    struct VTE *e = &o->vt[21];
    e->pfn((char *)o + e->d, n);
}
/** @sly2-annot addr=0x189530
 * Purpose: JT vt87 (p,f): base STEP 0x205D10, then vt21(!f) on three attached sub-objects
 * Info: proposed name SetJtPlayable (T2); class JT slot87; TU jt
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_00189530(struct S *p, int f)
{
    func_00205D10(p, f);
    if (f) {
        Call(p->a, 0);
        Call(p->b, 0);
        Call(p->c, 0);
    } else {
        Call(p->a, 1);
        Call(p->b, 1);
        Call(p->c, 1);
    }
}
