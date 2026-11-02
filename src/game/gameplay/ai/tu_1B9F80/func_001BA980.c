struct F { int a[0x1C]; };
struct S { char pad[0x108C]; int n; char ab[0xC]; };
void *func_002333B4(void *p, int c, int cb);
void *func_00205458(struct S *p, int oid);
void SetBhvguardMode(void *p, struct F *pf);
/** @sly2-annot addr=0x1BA980
 * Purpose: Clear 3 words, store n, send record 0xF to child behaviour 0x9C (0x176750)
 * Info: TU tu_1B9F80
 * Notes: names_v2 C proposed SetAloRealClock__FP3ALOi (0.242) | agent qualifier: splice: CID 60 command
 * Confidence: low (draft) [T2 0.20] - evidence: static naming pass; sources: r5b_rest
 * @end-sly2-annot */
void func_001BA980(struct S *p, int n)
{
    struct F f;
    void *po;
    func_002333B4(p->ab, 0, 0xC);
    p->n = n;
    func_002333B4(&f, 0, sizeof(f));
    f.a[0] = 0xF;
    po = func_00205458(p, 0x9C);
    if (po) SetBhvguardMode(po, &f);
}
