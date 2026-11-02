typedef int qword __attribute__((mode(TI)));
struct M { qword a, b, c; };
struct S { char pad[0x70]; qword q70; char p1[0x90 - 0x80]; qword q90; char p2[0xF4 - 0xA0]; int f; };
struct G { char pad[0x60]; qword q60; };
extern G D_002DDF40;
extern qword D_002A2DC0;
extern "C" {
M BuildOrthonormalMatrixZ(qword a, qword b);
int FRenderRipPosMat(S *p, qword q, M m);
void RenderFlying(S *p);
}
/** @sly2-annot addr=0x1CF618
 * Purpose: FLYING Render: oriented render via FRenderRipPosMat
 * Info: proposed name RenderFlying (T1); TU rip
 * Notes: names_v2 B proposed RenderStuck__FP5STUCKP2CM (0.957)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void RenderFlying(S *p)
{
    qword v;
    M m;
    if (p->f) {
        union { qword q; float f; } s; qword k;
        __asm__("qmtc2.ni %1, %0" : "=j"(k) : "r"(-1.0f));
        s.q = k;
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(v) : "j"(D_002DDF40.q60), "j"(k));
    } else {
        v = D_002A2DC0;
    }
    m = BuildOrthonormalMatrixZ(p->q90, v);
    FRenderRipPosMat(p, p->q70, m);
}
