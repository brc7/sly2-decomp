struct Q { int a, b, c; void *pvt; int pad[4]; void *p20; void *p24; int pad2[2]; };
struct E { char pad[0x94]; int n94; int pad2; int n9C; char pad3[0x100 - 0xA0]; };
struct P { char pad[0x50]; int c; struct E *ae; };
extern char D_002CA2C0[];
struct Pool;
extern struct Pool D_003E15CC;
struct Q *PvAllocSlotheapClear(struct Pool *);
void func_002333B4(void *, int, int);
struct Q *func_001F8510(struct P *p, int k, int m)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (pe->n94 == m && (k == -1 || pe->n9C == k)) {
            struct Q *q = PvAllocSlotheapClear(&D_003E15CC);
            func_002333B4(q, 0, 0x30);
            q->pvt = D_002CA2C0;
            q->p20 = p;
            q->p24 = pe;
            q->a = 0;
            q->b = 0;
            return q;
        }
    }
    return 0;
}
