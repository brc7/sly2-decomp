struct C { char pad[0x60]; void *p60; };
struct B { char pad[0xEC]; struct C *pc; };
struct Q { int a; struct B *pb; int pad[2]; unsigned long fl; };
int func_00132B10(void *p, int n);
/** @sly2-annot addr=0x1C5570
 * Purpose: collision filter callback (FIsCidDerived test)
 * Info: proposed name FFilterDprizeObjects (T1); TU prize
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
int func_001C5570(void *p, struct Q *q)
{
    void *s;
    /*B*/if (q->fl & (1UL << 48)) return 0; s = q->pb->pc->p60; if (func_00132B10(s, 0x32) || func_00132B10(s, 0x4F)) return 0; return 1;/*E*/
}
