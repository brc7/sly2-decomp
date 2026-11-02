typedef int qword __attribute__((mode(TI)));
struct X { char pad[0xA0]; qword r[3]; };
/** @sly2-annot addr=0x14F850
 * Purpose: True if point/sphere is inside all 4 frustum side planes using the transposed cull rows (+0xA0)
 *   and VU clip flags
 * Info: proposed name FInsideCmFrustum (T1); TU cm
 * Confidence: mid (draft) [T1 0.50] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
int FInsideCmFrustum(struct X *p, qword v, qword w)
{
    qword one;
    int f;
    __asm__("vmaxw.xyzw %0, $vf0, $vf0w" : "=j"(one));
    __asm__("vmulax.xyzw $ACC, %1, %4x\n\tvmadday.xyzw $ACC, %2, %4y\n\tvmaddaz.xyzw $ACC, %3, %4z\n\tvmsubax.xyzw $ACC, %5, %6x\n\tvnop\n\tvnop\n\tvnop\n\tvnop\n\tcfc2.ni %0, $vi17"
            : "=r"(f) : "j"(p->r[0]), "j"(p->r[1]), "j"(p->r[2]), "j"(v), "j"(one), "j"(w));
    return (f ^ 0xF0) == 0;
}
