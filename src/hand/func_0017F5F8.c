typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void *pfn; };
struct S { char pad[0xC]; struct VTE *vt; char pad2[0x178]; int n188; int pad3; void *p190; };
struct F { struct S *p0; int n4; int n8; int nC; int n10; int pad[3]; qword q20; qword q30; qword pad2[2]; };
void *func_002333B4(void *p, int c, int n);
qword func_001A62E8(void *p);
void func_001F7598(struct F *pf);
void func_0015C150(void *p);
/** @sly2-annot addr=0x17F5F8
 * Purpose: vt82: if it carries a blast damage x188 InflictZap area damage at its position, ExplodeExpl its
 *   explosion x190, then vt9 Remove
 * Info: proposed name HACK_Die (T2); class HACK slot82; TU tu_17F090
 * Notes: names_v2 C proposed OnStepguardWaypointActive__FP9STEPGUARD (0.438) | CID47->CID71 (CID 0x47=71,
 *   vtable) | v3 class identity: CID71_Die -> HACK_Die | T2 forced: v3: coined class word (HACK) without
 *   descriptor/string backing
 * Confidence: low (draft) [T2 0.50] - evidence: static naming pass; sources:
 *   r4b_rest+class_review(master_names_v2_1)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_0017F5F8(struct S *p, int n) {
    if (n && p->n188) {
        struct F f;
        struct VTE *e;
        func_002333B4(&f, 0, sizeof(f));
        f.n8 = n;
        f.p0 = p;
        f.q20 = func_001A62E8(p);
        e = &p->vt[0x21];
        f.q30 = ((qword (*)(void *))e->pfn)((char *)p + e->d);
        f.nC = 0x100;
        f.n10 = -p->n188;
        func_001F7598(&f);
    }
    if (p->p190) func_0015C150(p->p190);
    { struct VTE *e = &p->vt[9]; ((void (*)(void *))e->pfn)((char *)p + e->d); }
}
