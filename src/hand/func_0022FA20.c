typedef unsigned long ulong;
struct T { char pad[0xA4]; int k; };
struct S { char pad[0x10]; int a; int b; char pad2[0x10]; ulong f; char pad3[0x30]; struct T *t; char pad4[0xDC]; int k; };
extern char D_003DD490[];
void func_001D6310(struct S *p);
void func_001D6378(struct S *p);
void func_00147E88(void *g, int b);
/** @sly2-annot addr=0x22FA20
 * Purpose: if grf bit60: kind-specific refresh (0x1D6310/0x1D6378/0x147E88)
 * Info: proposed name PostSoSlotsUpdate (T2); TU xf
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/animation.md#xf-0x22c818-0x231100
 * @end-sly2-annot */
void func_0022FA20(struct S *p)
{
    if (!!((p->f >> 60) & 1)) {
        int k = p->k;
        if (k != -1 && p->t->k != k) { func_001D6310(p); func_001D6378(p); }
        if (p->a == 0) func_00147E88(D_003DD490, p->b);
    }
}
