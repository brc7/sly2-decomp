typedef int qword __attribute__((mode(TI)));
struct K { int a; int b; };
extern struct K D_002A4D10;
struct G { char pad[0x3BF4]; int grf; char pad2[0x3D58 - 0x3BF8]; int cid; };
extern struct G D_003DD490;
struct VTE { short d; short pad; void *pfn; };
struct M3 { qword r[3]; };
struct AREQ { int cb; int cbAlign; };
struct SO { float m[0x40 / 4]; char pad[0x94 - 0x40]; int x94; char pad2[0xA4 - 0x98]; int id; };
struct O { void *pad[3]; struct VTE *vt; };
struct P {
    int pad[3]; struct VTE *vt; char pad1[0x54 - 0x10]; struct SO *pso;
    char pad2[0x1088 - 0x58]; int x1088; char pad3[4]; struct O **apo; int cpo;
};
extern char D_002BC9F0[];
void func_00203A38(struct P *);
void func_001A64E8(struct P *, int, void *);
void func_001959C0(void *);
void func_001910F8(void *);
void func_0019F3F0(void *);
void func_001A1098(void *);
void func_0019CC30(void *);
void func_0021F388(void *);
void func_0019DE58(void *);
int func_0015E650(struct G *, int, int, struct P *, int, void **);
void *func_001AE2A0(struct AREQ *);
void func_00141EF8(struct P *);
void func_001440D0(struct P *, int);
int func_001F8278(struct P *);
void func_001F35C8(struct P *, float);
qword func_001A62E8(struct P *);
struct M3 *func_001A6318(struct P *);
void *func_002333B4(void *, int, int);
void func_00149A50(struct G *, struct P *, struct K, int);

/** @sly2-annot addr=0x1400B8
 * Purpose: STEPPLAYER vt15: base 0x203A38 then each component's post-load (0x1959C0, 0x1910F8, 0x19F3F0,
 *   0x1A1098, 0x19CC30, 0x21F388, 0x19DE58), allocations, registers a callback (0x149A50)
 * Info: proposed name PostLoadStepplayer (T1); class STEPPLAYER slot15; TU stepplayer
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_001400B8(struct P *p)
{
    void *apv[16];
    int c;
    int i;

    func_00203A38(p);
    func_001A64E8(p, 0x35, D_002BC9F0);
    func_001959C0((char *)p + 0x2C0);
    func_001910F8((char *)p + 0x4C0);
    func_0019F3F0((char *)p + 0x9E0);
    func_001A1098((char *)p + 0xDF0);
    func_0019CC30((char *)p + 0xF00);
    func_0021F388((char *)p + 0x11B0);
    func_0019DE58((char *)p + 0xF50);
    c = func_0015E650(&D_003DD490, 0x401, 0x2AB, p, 0x10, apv);
    if (c) {
        struct AREQ req;
        struct AREQ *preq = &req;
        preq->cb = c * 8;
        p->cpo = c;
        preq->cbAlign = 0x10;
        p->apo = func_001AE2A0(preq);
        for (i = 0; i < c; i++) {
            struct O *po;
            struct VTE *e;
            p->apo[i * 2] = apv[i];
            po = p->apo[i * 2];
            e = &po->vt[9];
            ((void (*)(void *))e->pfn)((char *)po + e->d);
        }
    }
    {
        struct VTE *e = &p->vt[79];
        ((void (*)(void *, int))e->pfn)((char *)p + e->d, 0);
    }
    {
        struct VTE *e = &p->vt[104];
        ((void (*)(void *))e->pfn)((char *)p + e->d);
    }
    func_00141EF8(p);
    func_001440D0(p, D_003DD490.grf & 4);
    p->x1088 = func_001F8278(p);
    func_001F35C8(p, 0.25f);
    if (p->pso->x94 == 0) {
        qword q = func_001A62E8(p);
        struct M3 m = *func_001A6318(p);
        struct SO *pso = p->pso;
        func_002333B4(pso, 0, 0x40);
        pso->m[0] = pso->m[5] = pso->m[10] = pso->m[15] = 1.0f;
        D_003DD490.cid++;
        p->pso->id = D_003DD490.cid;
        {
            struct VTE *e = &p->vt[18];
            ((void (*)(void *))e->pfn)((char *)p + e->d);
        }
        {
            struct VTE *e = &p->vt[29];
            ((void (*)(void *, qword))e->pfn)((char *)p + e->d, q);
        }
        {
            struct VTE *e = &p->vt[30];
            ((void (*)(void *, struct M3 *))e->pfn)((char *)p + e->d, &m);
        }
    }
    func_00149A50(&D_003DD490, p, D_002A4D10, 0);
}
