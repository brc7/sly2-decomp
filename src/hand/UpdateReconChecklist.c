extern int D_002C3588[];
struct S { char pad[0x74]; int n; int a[(0xAC - 0x78) / 4]; int hAC; };
int FReconTargetDone(int x);
void func_00213BD8(int h, int id, int v);
/** @sly2-annot addr=0x2021F8
 * Purpose: per pending target play 0x33A (done) or 0x33B on widget +0xAC
 * Info: proposed name UpdateReconChecklist (T1); TU tu_201D78
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
void UpdateReconChecklist(struct S *p)
{
    int i;
    for (i = 0; i < p->n; i++) {
        int v;
        int a = p->a[i];
        v = D_002C3588[i];
        if (FReconTargetDone(a))
            func_00213BD8(p->hAC, 0x33A, v);
        else
            func_00213BD8(p->hAC, 0x33B, v);
    }
}
