typedef int qword __attribute__((mode(TI)));
typedef struct {
    int x0;
    int kind;
    char pad8[0x10 - 0x8];
    union { qword q; struct { void *pv; float f; } s; } u;
    char pad20[0xA0 - 0x20];
} E;
typedef struct { char pad[0x60]; void *x60; } OBJ;
qword func_0022FC48(void *, int, qword);
void func_001E6F10(void *, qword *, int, float);
/** @sly2-annot addr=0x1DEF98
 * Purpose: World positions of a beam's two endpoints (kind 0 fixed via 0x22FC48, kind 1 point on path) and
 *   optionally their distance
 * Info: proposed name GetLasenBeamEnds (T1); TU sensor
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/gameplay-objects.md#sensor-0x1de2a0-0x1dfad8
 * @end-sly2-annot */
void func_001DEF98(OBJ *p, E *e, qword *out, float *pdist)
{
    unsigned int i;
    for (i = 0; i < 2; i++, e++) {
        switch (e->kind) {
        case 0:
            out[i] = func_0022FC48(p->x60, 0, e->u.q);
            break;
        case 1:
            func_001E6F10(e->u.s.pv, &out[i], 0, e->u.s.f);
            break;
        }
    }
    if (pdist) {
        int f;
        __asm__("lqc2 $vf1, %1\n\tlqc2 $vf2, %2\n\tvsub.xyzw $vf2, $vf2, $vf1\n\tvmul.xyz $vf2, $vf2, $vf2\n\tvaddw.x $vf3, $vf0, $vf0w\n\tvadday.x $ACC, $vf2, $vf2y\n\tvmaddz.x $vf2, $vf3, $vf2z\n\tvsqrt $Q, $vf2x\n\tvwaitq\n\tvaddq.x $vf2, $vf0, $Q\n\tqmfc2.ni %0, $vf2" : "=r"(f) : "m"(out[1]), "m"(out[0]));
        *(int *)pdist = f;
    }
}
