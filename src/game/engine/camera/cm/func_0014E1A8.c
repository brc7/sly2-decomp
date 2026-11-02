typedef int qword __attribute__((mode(TI)));
typedef struct { float x, y, z, w; } V4;
typedef struct { V4 r[3]; } M3 __attribute__((aligned(16)));
typedef struct { M3 m; V4 pos; } M4;
struct CM { char pad[0x60]; M4 mat; };
void GetVectorSphere(qword v, float *a, float *b, int c);
M3 LoadRotateMatrixPanTilt(float a, float b);
void func_00256D18(V4 *p, qword v);
void func_0014DC58(struct CM *p);
static __inline__ void LoadM4(M4 *pm, M3 m, qword pos)
{
    func_00256D18(&pm->pos, pos);
    pm->m = m;
    pm->m.r[1].w = 0;
    pm->m.r[2].w = 0;
    pm->m.r[0].w = 0;
    pm->pos.w = 1.0f;
}
/** @sly2-annot addr=0x14E1A8
 * Purpose: Place camera at pos looking at target, with extra pan/tilt offsets: pan/tilt from (tgt-pos)
 *   (0x11EFB0), rotation 0x11B720, pos row, UpdateCmMat4
 * Info: proposed name SetCmLookAt (T1); TU cm
 * Notes: names_v2 B proposed GetCmCpdefi__FP2CMP2SOfP6CPDEFI (0.985)
 * Confidence: mid (draft) [T1 0.75] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014E1A8(struct CM *p, qword pos, qword tgt, float da, float db)
{
    qword d;
    float a, b;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(tgt), "j"(pos));
    GetVectorSphere(d, &a, &b, 0);
    LoadM4(&p->mat, LoadRotateMatrixPanTilt(a + da, b + db), pos);
    func_0014DC58(p);
}
