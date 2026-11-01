// (Claude agent, lane E14) MATCH: the next av pointer is computed at the top of the k body (avNext = av + 1) and assigned after the vsub; this gives the target k+1/av+1 s7/fp assignment.
typedef int qword __attribute__((mode(TI)));
struct AL { int cb; int cbAlign; AL(int c, int a) { cb = c; cbAlign = a; } };
struct DZ;
struct X;
struct VEC { int x, y, z, w; };
class O {
public:
    int pad[3];
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6();
    virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67();
    virtual void Add(DZ *pdz, X *px, int a, int i);
    virtual void Get(void *p, qword q, VEC *pv);
};
struct E { int pad[2]; O *p; int pad2; qword q; char pad3[0x40 - 0x20]; };
struct B { E ae[2]; };
struct SO { char pad[0xA8]; int i; };
struct F { char pad[0x40]; qword v; SO *pso; char pad2[0x60 - 0x54]; };
struct X { char pad[0x20]; char x20[0x30]; B *pb; qword *av; };
struct C { E ae[2]; };
struct DZG { int x0; int c4; char pad[0x50 - 8]; int c50; C **apc; int pad58[2]; F *af; };
struct DZ { char pad[0x18]; int *ag; };
extern "C" {
void *func_001AE318(AL *pal);
void FillDzgDz(DZG *pdzg, int a, DZ *pdz, X *px);
}
static inline void *PvAlloc(int cb, int cbAlign) { AL al(cb, cbAlign); return func_001AE318(&al); }
static __inline__ int Dot(qword a, qword b)
{
    qword r, one;
    int g;
    __asm__("vmul.xyz %0, %0, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(r), "=&j"(one) : "0"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(r));
    return g;
}
void FillDzgDz(DZG *pdzg, int a, DZ *pdz, X *px)
{
    int i;
    B *pb = px->pb;
    for (i = 0; i < 2; i++) {
        O *po = pb->ae[i].p;
        if (po)
            po->Add(pdz, px, a, i);
    }
    qword *av = (qword *)PvAlloc(pdzg->c50 * 16, 16);
    px->av = av;
    for (int k = 0; k < pdzg->c50; k++) {
        C *pc = pdzg->apc[k];
        qword *avNext = av + 1;
        union { qword q[2]; VEC v[2]; } u;
        VEC *pv = u.v;
        E *pe = pc->ae;
        for (i = 0; i < 2; pv++, i++, pe++) {
            O *po = pe->p;
            if (!po)
                pv->x = pv->y = pv->z = 0;
            else if (po == pb->ae[0].p)
                po->Get(px, pe->q, pv);
            else if (po == pb->ae[1].p)
                po->Get(px->x20, pe->q, pv);
            else
                pv->x = pv->y = pv->z = 0;
        }
        qword d;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(u.q[0]), "j"(u.q[1]));
        qword *pq = av; av = avNext; *pq = d;
    }
    int *ag = (int *)PvAlloc(pdzg->c4 * 4, 16);
    pdz->ag = ag;
    for (int m = 0; m < pdzg->c4; m++) {
        F *pf = &pdzg->af[m];
        *ag++ = Dot(pf->v, px->av[pf->pso->i]);
    }
}
