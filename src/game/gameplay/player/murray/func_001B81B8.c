struct VE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VE *vt; };
struct T { char pad[0x180]; struct O *po; };
struct P { char pad[0xF1C]; struct O *xF1C; char pad2[0x13F0 - 0xF20]; struct T *x13F0; };
void func_00205D10(struct P *p, int b);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *p, int n);
#define VCALL(o, n, ...) ((void (*)())(o)->vt[n].pfn)((char *)(o) + (o)->vt[n].d, __VA_ARGS__)
/** @sly2-annot addr=0x1B81B8
 * Purpose: MURRAY vt87(p,f): STEP base 0x205D10, then propagate to carried object (vt21(!f), and vt87 if it
 *   is a STEP)
 * Info: proposed name SetMurrayPlayable (T2); class MURRAY slot87; TU murray
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B81B8(struct P *p, int b)
{
    struct O *o;
    func_00205D10(p, b);
    if (b) {
        if (!p->x13F0) return;
        o = p->x13F0->po;
        if (!o) return;
        p->xF1C = o;
        VCALL(o, 21, 0);
        if (FIsBasicDerivedFrom__FP5BASIC3CID(p->xF1C, 0x33)) VCALL(p->xF1C, 87, b);
    } else {
        o = p->xF1C;
        if (!o) return;
        VCALL(o, 21, 1);
        if (FIsBasicDerivedFrom__FP5BASIC3CID(p->xF1C, 0x33)) VCALL(p->xF1C, 87, 0);
        p->xF1C = 0;
    }
}
