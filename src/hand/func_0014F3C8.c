typedef int qword __attribute__((mode(TI)));
typedef union { struct { qword r[4]; } q; float af[4][4]; } M4;
struct CM { char pad[0x60]; M4 mat; };
qword *func_00256D18(qword *p, qword v);
void func_0014E168(struct CM *p, M4 *m);
/** @sly2-annot addr=0x14F3C8
 * Purpose: Option setter (g_aeopid[171], otyp 2 vector): keep rotation, set CM::mat pos row, SetCmMat4
 * Info: proposed name SetCmPos (T1); TU cm
 * Notes: names_v2 v1 proposed SetupCm__FP2CM (0.438)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014F3C8(struct CM *p, qword pos)
{
    M4 m = p->mat;
    func_00256D18(&m.q.r[3], pos);
    m.af[3][3] = 1.0f;
    func_0014E168(p, &m);
}
