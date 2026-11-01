typedef unsigned long ulong;
struct S8 { int a, b; };
struct N { int n0; struct N *next; };
struct E { char pad[0x64]; struct N *pn; int pad2; int n6C; };
struct P { char pad[0xEC]; struct E *pe; char pad2[0x148 - 0xF0]; ulong fl; };
struct G;
extern struct G D_003DD490;
extern struct S8 D_002A8278;
void EnsureSwCallback(struct G *, struct P *, struct S8, int);
void func_001F4F10(struct P *p)
{
    struct E *pe = p->pe;
    struct N *pn;
    if (pe->n6C) {
        EnsureSwCallback(&D_003DD490, p, D_002A8278, 0);
    } else if (p->fl & 0x3000000) {
        for (pn = pe->pn; pn; pn = pn->next) {
            if (pn->n0) {
                EnsureSwCallback(&D_003DD490, p, D_002A8278, 0);
                break;
            }
        }
    }
}
