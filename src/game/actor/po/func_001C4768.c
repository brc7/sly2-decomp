// Rotate a direction into a matrix frame, take the perpendicular (-y, z, 0) and normalize it (fallback D_002A2DB0).
// VF hard-register pins in Norm reproduce the original VF numbering.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };
struct M3 { qword row[3]; };
struct S { char pad[0x40]; struct M3 mat; };
extern qword D_002A2DB0;
static inline qword Local(const struct M3 *m, qword v)
{
    qword q, one, a, b, c;
    q = m->row[0];
    __asm__("vmul.xyzw %2,%0,%7\n\tvmaxw.xyzw %1,$vf0,$vf0w\n\tvmul.xyzw %3,%5,%7\n\tvmul.xyzw %4,%6,%7\n\tvadday.x $ACC,%2,%2y\n\tvmaddz.x %0,%1,%2z\n\tvaddax.y $ACC,%3,%3x\n\tvmaddz.y %0,%1,%3z\n\tvaddax.z $ACC,%4,%4x\n\tvmaddy.z %0,%1,%4y"
            : "+j"(q), "=&j"(one), "=&j"(a), "=&j"(b), "=&j"(c) : "j"(m->row[1]), "j"(m->row[2]), "j"(v));
    return q;
}
static inline qword Norm(qword v, qword fallback)
{
    register qword d __asm__("vf1"); register qword one __asm__("vf2"); qword r; float f;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "j"(v));
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "+j"(d));
    __asm__("qmfc2.ni %0,%1" : "=r"(f) : "j"(d));
    if (f < 0.0001f) d = fallback;
    else __asm__("vdiv $Q,$vf0w,%0x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q" : "+j"(d) : "j"(v));
    __asm__("qmfc2.ni %0,%1" : "=r"(r) : "j"(d));
    return r;
}
/** @sly2-annot addr=0x1C4768
 * Purpose: Transform a direction into matPanTilt frame, take (-y,z,0), normalize (fallback D_002A2DB0)
 * Info: proposed name POCTL_VecLateral (T2); class POCTL; TU po
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
qword func_001C4768(struct S *p, qword dir)
{
    union U v;
    v.q = Local(&p->mat, dir);
    v.f[0] = -v.f[1];
    v.f[1] = v.f[2];
    v.f[2] = 0.0f;
    return Norm(v.q, D_002A2DB0);
}
