typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x110]; int k; };
struct CM { char pad[0x90]; qword pos; };
extern char D_003DD490[];
int CploFindSwObjectsByClass(void *g, int a, int b, int c, int n, struct O **ap);
void func_002273F0(struct O *po, qword pos);
void func_002338C0(void *base, int n, int cb, int (*cmp)(const void *, const void *));
int func_0014E000(const void *a, const void *b);
void func_0014E148(struct CM *p, struct O *po);
void func_0014E078(struct CM *p)
{
    struct O *apo[32];
    qword pos = p->pos;
    int n = CploFindSwObjectsByClass(D_003DD490, 0x105, 5, 0, 0x20, apo);
    int i;
    for (i = 0; i < n; i++)
        func_002273F0(apo[i], pos);
    func_002338C0(apo, n, 4, func_0014E000);
    if (n > 0 && apo[0]->k == 1)
        func_0014E148(p, apo[0]);
}
