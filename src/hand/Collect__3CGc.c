typedef struct N { int x0; unsigned prev : 13; unsigned prevpg : 2; unsigned p1 : 1; unsigned next : 13; unsigned nextpg : 2; unsigned fMark : 1; } N;
typedef struct BIND { int x0; unsigned n : 31; unsigned fMark : 1; int x8; int ref[2]; struct BIND *pbNext; } BIND;
typedef struct ENV { int x0; unsigned prev : 13; unsigned prevpg : 2; unsigned p1 : 1; unsigned next : 13; unsigned nextpg : 2; unsigned fMark : 1; BIND *pbFirst; } ENV;
typedef struct ROOT { int x0; int x4; N *pobj; int xC; struct ROOT *prNext; } ROOT;
typedef struct GC { int f; int cFrame; N **apFrame; int cNode; N **apNode; int cClo; void **apClo; } GC;
typedef struct MSG { int cb; int nKind; int pad[6]; } MSG;
typedef struct CTX { int a[4]; } CTX;
extern char *D_004520E8[4];
extern char *D_00452138[4];
extern int D_00452108, D_004520B8, D_00452130;
extern N *D_002DD2AC;
extern ENV *D_002DD2CC;
extern ROOT *D_003E0E80;
extern unsigned int D_002DD598;
void func_002574E0(void);
void func_002574E8(void);
void func_002574F0(void);
void func_002574F8(void);
void func_00257500(void);
void func_00257508(void);
void func_00257510(void);
void func_00257678(void);
void func_002577E0(void);
void func_00257948(void);
void func_00257AB0(void);
void func_00257C18(void);
void func_001AE478(CTX *);
void func_001AE4A0(CTX *, int);
void *func_001AE318(MSG *);
void func_001285D8(GC *, void *);
void MarkLiveObjects__3CGc(GC *);

static inline void *AllocA(int c)
{
    MSG msg;
    msg.cb = c * 4;
    msg.nKind = 0x10;
    return func_001AE318(&msg);
}

static inline void InitStack(int *pc, void ***pap, int c)
{
    *pap = (void **)AllocA(c);
    *pc = 0;
}

static inline N *PtrN(unsigned idx, unsigned pg)
{
    if (idx == 0 && pg == 0)
        return 0;
    return (N *)(D_004520E8[pg] + idx * 0x28);
}
static inline N *NextN(N *p) { return PtrN(p->next, p->nextpg); }
static inline ENV *PtrEnv(unsigned idx, unsigned pg)
{
    if (idx == 0 && pg == 0)
        return 0;
    return (ENV *)(D_00452138[pg] + idx * 0xC);
}
static inline ENV *NextEnv(ENV *p) { return PtrEnv(p->next, p->nextpg); }

void Collect__3CGc(GC *pgc)
{
    CTX ctx;
    N *pn;
    ENV *penv;
    ROOT *pr;

    pgc->f = !pgc->f;
    func_002574E0();
    func_002574E8();
    func_002574F0();
    func_002574F8();
    func_00257500();
    func_00257508();
    func_001AE478(&ctx);
    InitStack(&pgc->cFrame, (void ***)&pgc->apFrame, D_00452108);
    InitStack(&pgc->cNode, (void ***)&pgc->apNode, D_004520B8);
    pgc->apClo = (void **)AllocA(D_00452130);
    pgc->cClo = 0;

    for (pn = D_002DD2AC; pn; )  {
        if (pn->fMark != D_002DD598) {
            pn->fMark = D_002DD598;
            pgc->apFrame[pgc->cFrame++] = pn;
        }
        {
            N *pnNext = NextN(pn);
            pn = pnNext;
        }
    }
    for (penv = D_002DD2CC; penv; )  {
        BIND *pb;
        if (penv->fMark != D_002DD598)
            penv->fMark = D_002DD598;
        for (pb = penv->pbFirst; pb; pb = pb->pbNext) {
            pb->fMark = D_002DD598;
            func_001285D8(pgc, pb->ref);
        }
        {
            ENV *pNext = NextEnv(penv);
            penv = pNext;
        }
    }
    for (pr = D_003E0E80; pr; pr = pr->prNext) {
        N *p = pr->pobj;
        if (p->fMark != D_002DD598) {
            p->fMark = D_002DD598;
            pgc->apNode[pgc->cNode++] = p;
        }
    }
    MarkLiveObjects__3CGc(pgc);
    func_00257510();
    func_00257678();
    func_002577E0();
    func_00257948();
    func_00257AB0();
    func_00257C18();
    func_001AE4A0(&ctx, 2);
}
