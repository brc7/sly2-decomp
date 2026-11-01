struct S { char pad[0x40]; unsigned int fl; char pad2[0xF0 - 0x44]; int nF0; int nF4; float gF8; float gFC; char pad3[0x118 - 0x100]; int n118; char dl[0x148 - 0x11C]; float g148; };
void func_00231100(void *p);
void func_0011A1F8(void *pdl, int ib);
void func_001A6CA8(void *p, int n);
/** @sly2-annot addr=0x1591B0
 * Purpose: EMITTER::Init: XFM init, default indices -1, g=10.0, ClearDl of particle list, register for
 *   update, flag 2
 * Info: proposed name InitEmitter (T1); class EMITTER slot7; TU emitter
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/particles-effects.md#1-emitters-emittercpp-0x159118-0x15cd68,
 *   docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void func_001591B0(struct S *p)
{
    unsigned int v;
    func_00231100(p);
    p->nF4 = -1;
    p->nF0 = -1;
    p->n118 = -1;
    p->gFC = 10.0f;
    p->gF8 = 10.0f;
    p->g148 = -1.0f;
    func_0011A1F8(p->dl, 0x128);
    func_001A6CA8(p, 0);
    v = p->fl;
    if ((v ^ 1) & 1) *(int *)v |= 2;
    else p->fl = v | 2;
}
