extern int D_002DAA10[];
struct S { int a; int b; int c; int *vt; int d; };
/** @sly2-annot addr=0x20C058
 * Purpose: CID 0x91 ctor (vtable 0x2DAA10)
 * Info: proposed name LEAP_ctor (T1); class LEAP; TU tu_20B840
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28
 * @end-sly2-annot */
struct S *LEAP_ctor(struct S *p) { p->vt = D_002DAA10; p->d = -1; p->a = 0; p->b = 0; return p; }
