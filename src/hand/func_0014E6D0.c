// MATCH (Claude agent): hard-register VF locals; clock pointer pinned to v0 and the second dt read via a v0-pinned int.
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } VU;
struct CLOCK { float t; union { float g; int n; } dt; };
extern struct CLOCK D_002DDED0;
struct P { char pad[0x300]; int f300; };
/** @sly2-annot addr=0x14E6D0
 * Purpose: Critically-damped spring of *ppos/ *pv toward target with (rSpring,rDamping) and clock dt; snaps
 *   and zeroes v when nCut
 * Info: proposed name SpringCm (T1); TU cm
 * Notes: names_v2 v1 proposed RemoveCmFadeObject__FP2CMP3ALO (0.167)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014E6D0(struct P *p, qword *ppos, float *pvel, qword target, float k1, float k2)
{
    register struct CLOCK *pclock __asm__("v0");
    register qword s1 __asm__("vf3");
    register qword s2 __asm__("vf4");
    register qword s3 __asm__("vf5");
    register qword d __asm__("vf1");
    register qword v __asm__("vf2");
    if (p->f300) {
        *ppos = target;
        pvel[2] = 0.0f;
        pvel[1] = 0.0f;
        pvel[0] = 0.0f;
        return;
    }
    pclock = &D_002DDED0;
    __asm__("qmtc2.ni %1, %0" : "=j"(s1) : "r"(k1));
    d = *ppos;
    __asm__("qmtc2.ni %1, %0" : "=j"(s2) : "r"(k2));
    __asm__("vsub.xyzw %0, %1, %0" : "+j"(d) : "j"(target));
    __asm__("qmtc2.ni %1, %0" : "=j"(s3) : "r"(pclock->dt.n));
    v = *(qword *)pvel;
    __asm__("vmulax.xyzw ACC, %0, %2x\n\tvmaddx.xyzw %0, %1, %3x" : "+j"(d) : "j"(v), "j"(s1), "j"(s2));
    { VU u; u.q = s1; }
    __asm__("vadda.xyzw ACC, %0, $vf0\n\tvmaddx.xyzw %0, %1, %2x" : "+j"(v) : "j"(d), "j"(s3));
    { VU u; u.q = s2; }
    *(qword *)pvel = v;
    {
        register qword s4 __asm__("vf3");
        qword x;
        { register int n __asm__("v0") = pclock->dt.n;
        __asm__("qmtc2.ni %1, %0" : "=j"(s4) : "r"(n)); }
        x = *ppos;
        { VU u; u.q = s3; }
        __asm__("vadda.xyzw ACC, %0, $vf0\n\tvmaddx.xyzw %0, %1, %2x" : "+j"(x) : "j"(v), "j"(s4));
        { VU u; u.q = s4; }
        *ppos = x;
    }
}
