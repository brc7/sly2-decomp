struct S { char pad[0x50]; int f50; char pad2[0xF4 - 0x54]; int n; };
extern char D_003DD490[];
int func_0020E040(void *p);
int FTaskLive(struct S *p)
{
    if (p->f50 == 0 && p->n != 2 && p->n != 3)
        return func_0020E040(D_003DD490) == 0;
    return 0;
}
