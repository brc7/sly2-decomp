typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
typedef union { qword q; float f[4]; } U;
struct X { int a[4]; float g10; float g14; };
extern qword D_002A5000;
M3 LoadRotateMatrixPanTilt(float x, float y);
qword func_0014FE58(struct X *p);
/** @sly2-annot addr=0x14FEC8
 * Purpose: Eye = anchor + RotMatrix(radPan +0x10, radTilt +0x14) * D_002A5000 offset
 * Info: proposed name PosCplookEye (T1); TU cplcy
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
qword func_0014FEC8(struct X *p)
{
    M3 m;
    U v;
    qword pos;
    /* pinned $v0 for address and loaded value, like the target */
    { register qword *pa __asm__("$2") = &D_002A5000; register qword t __asm__("$2") = *pa; v.q = t; }
    m = LoadRotateMatrixPanTilt(p->g10, p->g14);
    pos = func_0014FE58(p);
    {
        register qword r1 __asm__("vf2") = m.r[1];
        register qword r0 __asm__("vf3") = m.r[0];
        register qword r2 __asm__("vf1") = m.r[2];
        register qword rv __asm__("vf4") = v.q;
        register qword rp __asm__("vf2");
        __asm__("vmulax.xyz $ACC, %1, %3x\n\tvmadday.xyz $ACC, %2, %3y\n\tvmaddz.xyz %0, %0, %3z"
                : "+j"(r2) : "j"(r0), "j"(r1), "j"(rv));
        rp = pos;
        __asm__("vadd.xyzw %0, %0, %1" : "+j"(rp) : "j"(r2));
        return rp;
    }
}
