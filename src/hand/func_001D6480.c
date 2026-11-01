typedef int qword __attribute__((mode(TI)));
struct MSG { int cb; int nKind; };
struct E { qword pos; float x10; long k:4; long m:8; struct S *child; int x1C; };
struct S { struct E ae[4]; };
void *func_001AE2A0(struct MSG *pmsg);
static inline void *PvAllocMsg(struct MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE2A0(pmsg); }
/** @sly2-annot addr=0x1D6480
 * Purpose: Allocate 0x80-byte node of 4 sphere entries (kinds = -1)
 * Info: proposed name PsphnodeNew (T2); TU tu_1D57A8
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/rendering.md#tu_1d57a8-0x1d57a8-0x1d6848
 * @end-sly2-annot */
struct S *func_001D6480(void) {
    struct MSG msg;
    struct S *p = PvAllocMsg(&msg, 0x80, 0x10);
    p->ae[0].k = p->ae[1].k = p->ae[2].k = p->ae[3].k = -1;
    return p;
}
