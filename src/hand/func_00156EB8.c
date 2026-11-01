struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a[3]; struct VTE *vt; char pad[0x58 - 0x10]; struct O *pNext; };
struct S { int a[3]; struct VTE *vt; char pad[0xE0 - 0x10]; unsigned int c; struct O *ao[8]; };
extern struct O *D_003E148C;
void UpdateLo(struct S *p);
void func_00156DE0(struct S *p, int f);
int func_00156FE8(struct S *p, struct O *o);
void func_00156EB8(struct S *p)
{
    struct O *o;
    UpdateLo(p);
    func_00156DE0(p, 0);
    for (o = D_003E148C; o; o = o->pNext) {
        int i;
        for (i = 0; i < (int)p->c; i++)
            if (o == p->ao[i])
                break;
        if (i < (int)p->c)
            continue;
        if (!func_00156FE8(p, o))
            continue;
        if (p->c < 8) {
            struct VTE *e = &o->vt[1];
            e->pfn((char *)o + e->d);
            p->ao[p->c++] = o;
            e = &p->vt[3];
            e->pfn((char *)p + e->d, 5, 0, 0, 0, 0);
        }
        return;
    }
}
