extern "C" {
typedef int qword __attribute__((mode(TI)));
extern float D_002BE5F0[2];
struct VTE { short d; short pad; void *pfn; };
struct O { char pad[0xC]; struct VTE *vt; };
struct R { char pad[0x2E8]; int n2E8; char pad2[0x1000 - 0x2EC]; int n1000; };
struct Q { int pad[2]; int n8; };
struct M { int n0; int pad; void *p8; int nC; struct Q *p10; };
struct S { char pad[0x90]; int n90; struct O **a94; };
struct R *func_001CB250(void *p, int n);
void func_00173610(qword *pv, float a, float b, int c, int d);
int NRandInRange__Fii(int lo, int hi);
void *func_00205458(struct R *r, int n);
void *func_002333B4(void *p, int c, int n);
}
struct F { int a; int rest[0x1B]; F() { func_002333B4(this, 0, sizeof *this); } };
extern "C" {
void func_00176750(void *o, struct F *f);
void func_00176750(void *o, struct F *f);
}
extern "C" struct R *func_00174490(struct S *p, struct M *m, qword *pv, int a3, int t0) {
    struct R *r = 0;
    int k;
    switch (m->n0) {
    case 0:
        r = func_001CB250(m->p8, 0);
        break;
    case 1:
    case 2:
      {
        int idx[8];
        int i, j, l;
        func_00173610(pv, D_002BE5F0[1], D_002BE5F0[0], 1, 0);
        for (l = 0; l < p->n90; l++) idx[l] = l;
        for (j = 0; j < p->n90; j++) {
            int i = NRandInRange__Fii(0, p->n90 - 1);
            int t = idx[j];
            idx[j] = *(i + idx); *(i + idx) = t;
        }
        for (i = 0; i < p->n90; i++) {
            struct O *o = p->a94[idx[i]];
            { struct VTE *e = &o->vt[0x1D]; ((void (*)(void *, qword))e->pfn)((char *)o + e->d, *pv); }
            { struct VTE *e = &o->vt[0x1E]; ((void (*)(void *, int))e->pfn)((char *)o + e->d, a3); }
            r = func_001CB250(o, 0);
            if (r) break;
        }
      }
    k = m->n0;
        if (k == 1) {
            void *o;
            if (r) {
            m->p10->n8 = t0;
            r->n2E8 = m->nC;
            o = func_00205458(r, 0x9C);
            F f;
            f.a = -1;
            func_00176750(o, &f);
            f.a = k;
            func_00176750(o, &f);
            }
        }
    
        break;
    }
    if (r) r->n1000 = 1;
    return r;
}
