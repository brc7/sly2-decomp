
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
struct CM { char pad[0x40]; float sx, sy; char pad2[0x18]; M4 mat; };
void InvertMatrix4Rigid(M4 *pm, M4 *pn);
/** @sly2-annot addr=0x14F308
 * Purpose: World -> screen: inverse(CM::mat) * pos, axis swap, divide by tanHalf and depth (if > 1e-4)
 * Info: proposed name ConvertCmWorldToScreen (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
qword ConvertCmWorldToScreen(struct CM *p, qword v)
{
    union { qword q; float f[4]; } r;
    M4 m;
    float x, y, z;
    InvertMatrix4Rigid(&m, &p->mat);
    __asm__("vmulax.xyz $ACC, %1, %2x\n\tvmadday.xyz $ACC, %3, %2y\n\tvmaddaz.xyz $ACC, %4, %2z\n\tvmaddw.xyz %0, %5, $vf0w"
            : "=j"(r.q) : "j"(m.r[0]), "j"(v), "j"(m.r[1]), "j"(m.r[2]), "j"(m.r[3]));
    x = r.f[0];
    y = r.f[1];
    z = r.f[2];
    r.f[2] = x;
    r.f[0] = -y / p->sx;
    r.f[1] = z / p->sy;
    if (x > 1e-4f) {
        r.f[0] = r.f[0] / x;
        r.f[1] = r.f[1] / x;
    }
    return r.q;
}
