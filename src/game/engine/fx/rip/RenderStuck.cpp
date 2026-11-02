typedef int qword __attribute__((mode(TI)));
struct M { qword a, b, c; };
struct S { char pad[0x40]; M m; qword q70; char pad2[0xF0 - 0x80]; void *p; };
extern "C" {
qword func_0022FC48(void *p, int n, qword q);
M ConvertXfMat(void *p, int n, M *pm);
int FRenderRipPosMat(S *p, qword q, M m);
void RenderStuck(S *p);
}
/** @sly2-annot addr=0x1CFA40
 * Purpose: STUCK Render: transform by host object's matrix, FRenderRipPosMat
 * Info: proposed name RenderStuck (T1); TU rip
 * Notes: names_v2 B proposed PostBulletEmit__FP6BULLETP5EMITB (0.892)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void RenderStuck(S *p)
{
    qword q = func_0022FC48(p->p, 0, p->q70);
    M m;
    m = ConvertXfMat(p->p, 0, &p->m);
    FRenderRipPosMat(p, q, m);
}
