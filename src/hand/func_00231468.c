// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
typedef struct { char pad[0x40]; M4 m; } O;
typedef struct { char pad[0x50]; O *x50; char pad2[0x3C]; qword x90; } S;
extern qword D_002A2DD0;
qword func_0022D348(O *, int, qword);
static inline qword MulPos(const M4 *m, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddaz.xyz $ACC, %4, %1z\n\tvmaddw.xyz %0, %5, $vf0w"
            : "=j"(r) : "j"(v), "j"(m->r[0]), "j"(m->r[1]), "j"(m->r[2]), "j"(m->r[3]));
    return r;
}
/** @sly2-annot addr=0x231468
 * Purpose: velocity via GetXfVelocity of parent XF
 * Info: proposed name GetXfmVelocity (T2); class XFM slot33; TU xform
 * Notes: names_v2 C proposed EnsureAloThrob__FP3ALO (0.253)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r8_21BB88
 * See: docs/reference/functions/object-model.md#xform-0x231100-0x2319e8
 * @end-sly2-annot */
qword func_00231468(S *p)
{
    O *o = p->x50;
    if (!o) return D_002A2DD0;
    return func_0022D348(o, 0, MulPos(&o->m, p->x90));
}
