typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x90]; int n90; char pad2[4]; short k; };
struct E { char pad[0x80]; qword q; };
struct P { char pad[0x58]; struct O *po; char pad2[0xE8 - 0x5C]; struct E *pe; };
void func_002303A0(struct P *, struct O *, int);
void func_001F5078(struct P *);
void SetSoDvGravity(struct P *p, qword q)
{
    if (p->pe) {
        struct O *po = p->po;
        if (po->n90 == 0 && po->k != 2) func_002303A0(p, po, 1);
        p->pe->q = q;
        func_001F5078(p);
    }
}
