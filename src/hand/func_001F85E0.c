struct Q { int a, b, c; void *pvt; int pad[4]; void *p20; void *p24; int pad2[2]; };
extern char D_002CA280[];
struct Pool;
extern struct Pool D_003E15CC;
void *func_001F6030(void *);
struct Q *PvAllocSlotheapClear(struct Pool *);
void *func_002333B4(void *, int, int);
struct Q *func_001F85E0(void *pa)
{
    void *r = func_001F6030(pa);
    struct Q *q;
    if (!r) return 0;
    q = PvAllocSlotheapClear(&D_003E15CC);
    func_002333B4(q, 0, 0x30);
    q->p20 = pa;
    q->p24 = r;
    q->pvt = D_002CA280;
    q->a = 0;
    q->b = 0;
    return q;
}
