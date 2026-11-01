struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a, b, c; struct VTE *vt; };
struct S { char pad[0x26C]; char x26c[4]; };
void func_001EECD0(void *p);
struct O *func_00205438(struct S *p, int oid);
/** @sly2-annot addr=0x1B9CA0
 * Purpose: Splice-callable (p,f): 0x1EECD0(p->x26C); if f, find child 0xC6 and call its vt8
 * Info: TU murray
 * Notes: agent qualifier: splice: Murray SM op
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r5b_rest
 * @end-sly2-annot */
void func_001B9CA0(struct S *p, int f)
{
    func_001EECD0(p->x26c);
    if (f) {
        struct O *po = func_00205438(p, 0xC6);
        struct VTE *e = &po->vt[8];
        ((void (*)(void *))e->pfn)((char *)po + e->d);
    }
}
