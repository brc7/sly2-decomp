extern struct { float t; float dt; } D_002DDED0;
extern float D_002C3DAC;
struct X { int pad; int n4; int pad2; float tC; };
struct S { char pad[0x2C0]; long fl; char pad2[0x3D0 - 0x2C8]; int n3D0; char pad3[0xF64 - 0x3D4]; void *pF64; };
int func_00132B10(void *p, int n);
long func_00207A20(int n);
struct X *func_00154180(void *p, void *q);
/** @sly2-annot addr=0x20A298
 * Purpose: Target check with MURRAY special case, detector entry age D_002C3DAC and mask
 * Info: proposed name FStepguardTargetValid (T2); TU stepguard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_0020A298(struct S *p, void *q, long mask)
{
    struct X *x;
    if (p->n3D0)
        goto ret0;
    if (func_00132B10(q, 0x36) && !func_00132B10(p, 0x3A) && !(p->fl & func_00207A20(6)))
        goto ret1;
    x = func_00154180(p->pF64, q);
    if (!x)
        goto ret1;
    if (!x->n4 || D_002DDED0.t - x->tC < D_002C3DAC)
        goto ret1;
    if (!(p->fl & mask))
        goto ret0;
ret1:
    return 1;
ret0:
    return 0;
}
