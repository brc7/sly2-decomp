// (Claude agent) frame declared at function scope with explicit init/free calls (free = dtor with in-charge 2) so msg2 reuses msg's slot.
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct VTE { short d; short pad; void (*pfn)(); };
struct PO { int pad[3]; VTE *pvt; int x10; struct PO **pp14; };
struct P {
    int pad0[3]; VTE *pvt; char pad10[0x28 - 0x10]; unsigned long ul28; char pad30[0x154 - 0x30]; float g154; int pad158;
    float g15C; int pad160; int x164; PO *p168; float g16C; int pad170; int x174; int pad178[2];
    qword v180; char ab190[0x30]; qword v1C0; qword v1D0; int x1E0; int pad1E4[2]; int x1EC; int x1F0; int pad1F4[3]; PO *p200;
};
struct MSG { P *p; int x4; PO *po; int xC; int x10; int pad14[3]; qword v20; qword v30; char pad40[0x60 - 0x40]; };
struct MSG2 { int k; P *p; float g8; float gC; };
struct G { char pad[0x402C]; int x402C; };
extern "C" {
extern qword D_002A2DC0;
extern int D_002BC9E0;
extern float D_002A4B1C;
extern G D_003DD490;
qword func_001A62E8(void *p);
void func_0013B428(P *p, qword a, qword b);
void func_0013B5E8(P *p, int *pc, PO ***papo);
void *func_002333B4(void *pv, int b, int cb);
int func_001F7598(MSG *pmsg);
int FIsBasicDerivedFrom__FP5BASIC3CID(PO *po, int k);
int *func_00156698(int n, qword pos, void *pv);
}
struct FRAME { int a[2]; }; // FrameInit = ctor, FrameFree = dtor(this, 2)
extern "C" void *FrameInit(FRAME *); extern "C" void FrameFree(FRAME *, int);

extern "C" void func_0013B860(P *p)
{
    if (((p->ul28 >> 60) & 1) == 0)
        return;
    func_0013B428(p, func_001A62E8(p), D_002A2DC0);
    FRAME frame;
    if (p->x164 == 0) {
        FrameInit(&frame);
        int c;
        PO **apo;
        int i;
        func_0013B5E8(p, &c, &apo);
        for (i = 0; i < c; i++) {
            PO *po = apo[i];
            MSG msg;
            U g;
            qword d, n;
            float l;
            func_002333B4(&msg, 0, 0x60);
            msg.xC = p->x1EC;
            msg.x10 = p->x1F0;
            msg.v20 = p->v180;
            msg.p = p;
            msg.po = po;
            qword pos = func_001A62E8(po);
            qword def = D_002A2DC0;
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(pos), "j"(p->v180));
            {
                register qword t __asm__("vf2");
                qword one;
                __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(t), "=&j"(one) : "j"(d));
                __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(t) : "j"(t));
                __asm__("qmfc2.ni %0, %1" : "=r"(l) : "j"(t));
                if (l < 1e-4f) {
                    t = def;
                } else {
                    __asm__("vdiv Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, Q" : "=j"(t) : "j"(t), "j"(d));
                }
                n = t;
            }
            { qword t; __asm__("qmtc2.ni %1, %0" : "=j"(t) : "r"(D_002BC9E0)); g.q = t; }
            {
                qword v;
                __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(v) : "j"(n), "j"(g.q));
                msg.v30 = v;
            }
            func_001F7598(&msg);
        }
        FrameFree(&frame, 2);
    }
    ((void (*)(P *, qword))p->pvt[29].pfn)((P *)((char *)p + p->pvt[29].d), p->v180);
    ((void (*)(P *, void *))p->pvt[30].pfn)((P *)((char *)p + p->pvt[30].d), p->ab190);
    ((void (*)(P *, qword))p->pvt[35].pfn)((P *)((char *)p + p->pvt[35].d), p->v1C0);
    ((void (*)(P *, qword))p->pvt[36].pfn)((P *)((char *)p + p->pvt[36].d), p->v1D0);
    if (p->p168) {
        MSG2 msg2;
        PO *po = p->p168;
        msg2.k = 9;
        msg2.g8 = p->g16C;
        msg2.gC = p->g15C;
        msg2.p = p;
        ((void (*)(PO *, MSG2 *))po->pvt[51].pfn)((PO *)((char *)po + po->pvt[51].d), &msg2);
    }
    if (p->x1E0) {
        PO *po = p->p200;
        void *pv = p;
        if (po != (PO *)p && FIsBasicDerivedFrom__FP5BASIC3CID(po, 0x70) && po->pp14) {
            PO *pT = *po->pp14;
            if (pT != po) pv = pT;
        }
        func_00156698(D_003DD490.x402C, func_001A62E8(p), pv)[0x54 / 4] = 1;
    }
    ((void (*)(P *))p->pvt[9].pfn)((P *)((char *)p + p->pvt[9].d));
    p->x174 = 0;
    p->g154 = D_002A4B1C;
    ((void (*)(P *, int, int, int, int, int))p->pvt[3].pfn)((P *)((char *)p + p->pvt[3].d), 5, 0, 0, 0, 0);
}
