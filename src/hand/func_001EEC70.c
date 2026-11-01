struct MSG { int cb; int nKind; };
struct S { int a, b; void *ap[8]; };
int func_001EF6F0(void);
void *func_001AE1D0(struct MSG *pmsg);
/** @sly2-annot addr=0x1EEC70
 * Purpose: Allocate the 8 BHV slots (CbBhvMax()=0xF0 bytes each, align 0x10) from the world heap into
 *   apbhv[0..7]
 * Info: proposed name PostBhvstkLoad (T1); TU bhvstk
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#bhvstk-0x1eec60-0x1ef850
 * @end-sly2-annot */
void func_001EEC70(struct S *p)
{
    unsigned int i;
    struct MSG msg;
    for (i = 0; i < 8; i++) {
        msg.cb = func_001EF6F0();
        msg.nKind = 0x10;
        p->ap[i] = func_001AE1D0(&msg);
    }
}
