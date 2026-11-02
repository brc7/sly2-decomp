extern char D_002DA9D0[];
struct P { char pad[0x30]; int n30; };
struct O { int n0; int n4; int pad8; void *pC; char pad10[0x10]; int n20; int h24; struct P *p28; int *p2C; int n30; char pad34[0xC]; };
void *func_002333B4(void *p, int c, int n);
void ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA(int a, int b, int c, int *ph, float f, float g);
void SubscribeLoObject(int h, struct O *po);
/** @sly2-annot addr=0x20C948
 * Purpose: init CID 0x92 object (vtable 0x2DA9D0) and apply ASEG
 * Info: proposed name StartDirAnim (T2); TU tu_20B840
 * Notes: T2 forced: verifier: coined class word without descriptor
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28
 * @end-sly2-annot */
void func_0020C948(struct P *p, int a, int b, struct O *po)
{
    func_002333B4(po, 0, sizeof(*po));
    func_002333B4(po, 0, sizeof(*po));
    po->p28 = p;
    po->n20 = a;
    po->pC = D_002DA9D0;
    po->n30 = b;
    po->n0 = 0;
    po->n4 = 0;
    ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA(p->n30, a, b, &po->h24, 0.0f, 1.0f);
    SubscribeLoObject(po->h24, po);
    po->p2C = &p->n30;
}
