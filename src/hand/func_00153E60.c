typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct T { char pad[0xE0]; void *xE0; };
extern qword D_002A2DC0;
qword func_001A62E8(void *p);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int k);

/** @sly2-annot addr=0x153E60
 * Purpose: Returns the detector eye position: own position, or its parent position (+80 units up if the
 *   parent is a STEPGUARD-family object, cid test 0x38)
 * Info: proposed name PosDetectorEye (T2); TU detector
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/guards-ai.md#detector-0x153d58-0x154a60
 * @end-sly2-annot */
qword func_00153E60(struct T *p)
{
    if (!p->xE0) return func_001A62E8(p);
    if (FIsBasicDerivedFrom__FP5BASIC3CID(p->xE0, 0x38)) {
        U g;
        qword r;
        qword pos = func_001A62E8(p->xE0);
        float f = 80.0f;
        __asm__("qmtc2.ni %1, %0" : "=j"(g.q) : "r"(f));
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(pos), "j"(D_002A2DC0), "j"(g.q));
        return r;
    }
    return func_001A62E8(p->xE0);
}
