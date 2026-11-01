typedef int qword __attribute__((mode(TI)));
struct MATRIX4 { float mat[4][4]; } __attribute__((aligned(16)));
struct BONE { char pad[0x40]; MATRIX4 mat; };
struct Y { char pad[0x9A]; short s9A; char pad2[0xA0 - 0x9C]; BONE *pA0; char pad3[0xC0 - 0xA4]; qword qC0; qword qD0; };
struct XX { char pad[8]; Y *py; char pad2[0x1C - 0xC]; BONE *p1C; MATRIX4 mat; };
extern qword D_002A2DD0;
extern "C" {
MATRIX4 InvertMatrix4Rigid(const MATRIX4 &mat);
MATRIX4 __ml__FRC7MATRIX4T0(const MATRIX4 &a, const MATRIX4 &b);
qword func_0022CA18(Y *py, MATRIX4 mat);
void func_0012DEF0(XX *x);
}
/** @sly2-annot addr=0x12DEF0
 * Purpose: Matrix-based act update (InvertMatrix4Rigid, M4*M4)
 * Info: proposed name UpdateActXform (T2); class ActXform; TU act
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012DEF0(XX *x) { MATRIX4 m; if (x->py->pA0) m = __ml__FRC7MATRIX4T0(__ml__FRC7MATRIX4T0(InvertMatrix4Rigid(x->py->pA0->mat), x->p1C->mat), x->mat); else m = __ml__FRC7MATRIX4T0(x->p1C->mat, x->mat); Y *py = x->py; py->qC0 = func_0022CA18(py, m); x->py->qD0 = D_002A2DD0; x->py->s9A = 3; }
