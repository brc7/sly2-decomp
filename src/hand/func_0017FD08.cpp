typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern "C" struct CLOCK D_002DDED0;
extern "C" qword D_002A2DD0;
extern "C" char D_002DEB30[];
struct M4 { qword r[4]; };
struct VTE { short d; short pad; void *pfn; };
struct O { char pad[0xC]; struct VTE *vt; int n10; };
struct N { char pad[0xA0]; int nA0; };
struct E { void *p0; int pad[5]; };
struct Q { int a[4]; };
struct S {
    char pad[0xC]; struct VTE *vt; char pad2[0x44]; struct N *p54; struct N *p58; struct N *p5C;
    char pad3[0x1C0 - 0x60]; int n1C0; float f1C4; struct Q a1C8[6]; char padq[8]; int n230;
    char pad4[0x248 - 0x234]; struct E a248[3]; int pade; struct O *p294; int n298; int n29C; struct O *p2A0; void *p2A4; int n2A8;
};
extern "C" {
void func_00213B00(struct O *o, int n);
void UpdateHackshipHealthBar(struct S *p);
void *func_001FBB90(int n);
void func_001FB2F8(void *p, int n);
void func_00258040(void **pp, void *q);
void func_001FB018(void *p);
void ReleaseAct(int *p);
void func_002303A0(struct S *p, struct N *n, int k);
void SetLoParent_1A58D0(struct S *p, int a, int b);
void SetSoCnstrForce(struct S *p, int n);
void SetSoCnstrTorque(struct S *p, int n);
void func_001F68A8(struct S *p, int n);
void SetSmaGoal(void *p, int n);
void func_001FB098(void *p, float f);
struct M4 func_0022FF40(int a, int b, struct M4 *pm);
int func_0012DC18(struct S *p, struct N *n, int a, int b, int c, qword v);
qword func_0011BFF0(struct M4 *pm);
void BroadcastGameEvent(void *a, int n, struct S *p);
void func_0017F4C0(struct S *p, struct Q *q);
void func_0017FD08(struct S *p, int n);
}
void func_0017FD08(struct S *p, int n) {
    int old = p->n1C0;
    if (n == old) return;
    switch (old) {
    case -1:
        {
            struct O *q = p->p2A0;
            if (q) {
                { struct VTE *e = &q->vt[0x31]; ((void (*)(void *, int))e->pfn)((char *)q + e->d, -1); }
                func_00213B00(p->p2A0, 0x34F);
                UpdateHackshipHealthBar(p);
                if (p->n2A8 != old) {
                    void *r = func_001FBB90(p->n2A8);
                    func_001FB2F8(r, 0);
                    func_00258040(&p->p2A4, r);
                    func_001FB018(p->p2A4);
                }
            }
        }
        break;
    case 3:
        ReleaseAct(&p->n298);
        ReleaseAct(&p->n29C);
        func_002303A0(p, p->p58, 2);
        func_002303A0(p, p->p5C, 2);
        SetLoParent_1A58D0(p, 0, 0xF);
        SetSoCnstrForce(p, 5);
        SetSoCnstrTorque(p, 4);
        func_001F68A8(p, 0);
        p->p294 = 0;
        break;
    case 1:
        {
            int i;
            for (i = 0; i < p->n230; i++) {
                if (p->a248[i].p0) SetSmaGoal(p->a248[i].p0, 0x66);
            }
        }
        break;
    }
    p->n1C0 = n;
    p->f1C4 = D_002DDED0.t;
    switch (n) {
    case -1:
        {
            struct O *q = p->p2A0;
            if (q) { struct VTE *e = &q->vt[0x32]; ((void (*)(void *))e->pfn)((char *)q + e->d); }
            void **pp = &p->p2A4;
            if (*pp) {
                func_001FB098(*pp, 0.0f);
                func_00258040(pp, 0);
            }
        }
        break;
    case 3:
        {
            struct M4 m;
            int r;

            { struct O *q = p->p294; struct VTE *e = &q->vt[0x20]; r = ((int (*)(void *, struct M4 *))e->pfn)((char *)q + e->d, &m); }
            m = func_0022FF40(p->p54->nA0, p->p58->nA0, &m);
            { struct VTE *e = &p->vt[0xA]; ((void (*)(void *, int, int, int))e->pfn)((char *)p + e->d, p->p294->n10, r, 3); }
            { struct VTE *e = &p->vt[0x23]; ((void (*)(void *, qword))e->pfn)((char *)p + e->d, D_002A2DD0); }
            { struct VTE *e = &p->vt[0x24]; ((void (*)(void *, qword))e->pfn)((char *)p + e->d, D_002A2DD0); }
            p->n298 = func_0012DC18(p, p->p58, 1, 7, 2, m.r[3]);
            p->n29C = func_0012DC18(p, p->p5C, 1, 7, 2, func_0011BFF0(&m));
            SetSoCnstrForce(p, 1);
            SetSoCnstrTorque(p, 1);
            func_001F68A8(p, 1);
        }
        break;
    case 4:
        BroadcastGameEvent(D_002DEB30, 0x10, p);
        break;
    }
    func_0017F4C0(p, n == -1 ? 0 : &p->a1C8[n]);
}
