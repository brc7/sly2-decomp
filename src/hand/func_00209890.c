typedef struct { short d; short pad; void *pfn; } VTE;
struct T { int a; int b; };
extern struct T D_002C3D98[2];
extern char D_002DEB30[];
struct S { char pad[0xC]; VTE *vt; char pad2[0x2C0 - 0x10]; long fl; };
struct L { int *pa; int *pb; };
long func_00207A20(int n);
void func_00209870(struct S *p, long fl);
int func_002099F0(long m, int a);
void func_001619A0(void *p, int b, struct S *ps);
/** @sly2-annot addr=0x209890
 * Purpose: Set guard flag n, send message 7 (n,-1) to self, broadcast game event for flags in D_002C3D98
 *   (3->0x14, 0xC->0x16)
 * Info: proposed name AddStepguardSgf (T1); TU stepguard
 * Notes: runtime: guard flag word +0x2C0 == union of BHV-raised flags in every state
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); sources: r7b_guard+runtime
 * See: docs/engine/guards-ai.md#2-the-guard-flag-word-0x2c0, docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_00209890(struct S *p, int n)
{
    long m = func_00207A20(n);
    struct L l;
    int b;
    unsigned int i;
    func_00209870(p, p->fl | m);
    b = -1;
    l.pa = &n;
    l.pb = &b;
    ((void (*)(void *, int, int, int, struct L *, int))p->vt[3].pfn)((char *)p + p->vt[3].d, 7, 0, 2, &l, 0);
    for (i = 0; i < 2; i++) {
        if (func_002099F0(m, D_002C3D98[i].a))
            func_001619A0(D_002DEB30, D_002C3D98[i].b, p);
    }
}
