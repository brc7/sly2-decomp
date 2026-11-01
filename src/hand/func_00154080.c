typedef struct CLOCK { float t; float dt; } CLOCK;
extern CLOCK D_002DDED0;
extern char D_002DEB30[];
struct VTE { short d; short pad; void (*pfn)(); };
struct E { int id; int n; float t; float tC; };
struct O { int a[3]; struct VTE *vt; void *p10; };
struct ARGS { struct E *pe; int *pn; int *pnNew; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
void BroadcastGameEvent(void *p, int n, void *q);
void func_00154080(struct O *o, struct E *pe, int nNew)
{
    struct ARGS args;
    struct VTE *e;
    if (!pe || nNew == pe->n)
        return;
    if (pe->n == 0)
        pe->tC = D_002DDED0.t;
    args.pe = pe;
    args.pn = &pe->n;
    args.pnNew = &nNew;
    e = &o->vt[3];
    e->pfn((char *)o + e->d, 0x1C, 0, 3, &args, 0);
    pe->n = nNew;
    pe->t = D_002DDED0.t;
    if (nNew == 1) {
        void *q = o->p10;
        if (q && FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x33))
            BroadcastGameEvent(D_002DEB30, 0x13, q);
    }
}
