extern int D_002C3588[];
struct S { char pad[0x74]; int n; int a[(0xAC - 0x78) / 4]; int hAC; };
int FReconTargetDone(int x);
void func_00213BD8(int h, int id, int v);
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
