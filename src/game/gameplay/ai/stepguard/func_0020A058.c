typedef struct { int tagk; int tag; } CRef;
void func_002013D8(CRef *pr, int a, int b, int c, void *d, void *e);
void func_00129658(CRef *pd, CRef *ps);
void Decref__4CRef(CRef *pr);
struct S { char pad[0xF94]; int a; int b; char pad2[0xFA8 - 0xF9C]; int c; int d; };
/** @sly2-annot addr=0x20A058
 * Purpose: Splice: fetch two typed CRefs (0x2002) into +0xF94/+0xF98 and +0xFA8/+0xFAC
 * Info: proposed name SetStepguardCRefPair (T2); TU stepguard
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020A058(struct S *p, int x, int y)
{
    CRef r;
    CRef t;
    r.tagk = -1;
    func_002013D8(&t, x, 0x2002, 4, &p->a, &p->b);
    func_00129658(&r, &t);
    Decref__4CRef(&t);
    func_002013D8(&t, y, 0x2002, 4, &p->c, &p->d);
    func_00129658(&r, &t);
    Decref__4CRef(&t);
    Decref__4CRef(&r);
}
