struct N { int a; void *o; int b; struct N *next; };
struct S { char pad[0x150]; struct N *first; };
extern char D_003DD490[];
void func_001F2D48(struct S *p);
void func_00229738(void *o, struct N *n);
void func_002296C0(void *g, struct N *n);
/** @sly2-annot addr=0x229308
 * Purpose: SO slot12 then removes and frees all XAs
 * Info: proposed name RemoveWaterHierarchy (T2); class WATER slot12; TU water
 * Notes: names_v2 C proposed RotateSoToMat__FP2SOP7MATRIX3 (0.200)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r8_21BB88
 * See: docs/reference/functions/physics-collision.md#water-0x228b48-0x229ef0
 * @end-sly2-annot */
void func_00229308(struct S *p)
{
    struct N *n;
    func_001F2D48(p);
    for (n = p->first; n; n = n->next) func_00229738(n->o, n);
    func_002296C0(D_003DD490, p->first);
    p->first = 0;
}
