typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct N { int a[4]; qword plane; };
void func_002564B8(void *p);
/** @sly2-annot addr=0x223A10
 * Purpose: signed distance of pos to a plane
 * Info: proposed name GDistPlanePos (T1); TU tn
 * Notes: verifier PLAUSIBLE: GDistPlanePoint? (helper) -> GDistPlanePos
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
float func_00223A10(void *p, qword pos, qword b, struct N *n)
{
    U d, one;
    qword plane;
    float dist;
    plane = n->plane;
    func_002564B8(&d);
    func_002564B8(&one);
    __asm__("vmul.xyz %1, %2, %3\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddaz.x $ACC, %0, %1z\n\tvmsubw.x %1, %0, %2w"
            : "=&j"(one.q), "=&j"(d.q) : "j"(plane), "j"(pos));
    __asm__("qmfc2.ni %0, %1" : "=r"(dist) : "j"(d.q));
    return dist;
}
