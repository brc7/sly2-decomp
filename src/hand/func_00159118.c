typedef int qword __attribute__((mode(TI)));
typedef struct M4 { qword aq[4]; } M4;
typedef struct B7 { qword aq[7]; } B7;
extern M4 D_002DE5B0;
extern B7 D_002DE5F0;
struct S { int a; int b; int pad[2]; M4 m; B7 b7; char pad2[0x100 - 0xC0]; };
void *func_002333B4(void *pv, int b, int cb);
/** @sly2-annot addr=0x159118
 * Purpose: Zeroes an EMITB and installs the default EMITO/EMITV objects (D_002DE5B0/D_002DE5F0)
 * Info: proposed name InitEmitb (T1); TU emitter
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/core-library.md#6-source-files, docs/engine/particles-effects.md#5-source-files,
 *   docs/engine/particles-effects.md#6-key-functions
 * @end-sly2-annot */
void func_00159118(struct S *p)
{
    func_002333B4(p, 0, sizeof(struct S));
    p->a = 1;
    p->b = -1;
    p->m = D_002DE5B0;
    p->b7 = D_002DE5F0;
}
