typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; char p1[0xEF0]; char xf00[4]; };
struct Q { char pad[0xC]; char xc[0x38]; int fl; };
struct S { char pad[0xC]; struct VTE *vt; char p1[0x16C]; int x17c; char p2[0x10]; struct O *x190; };
struct M { struct Q *q; void *pv; };
extern char D_003DD490[];
int func_001F7588(struct S *p, struct Q *q);
qword func_001A62E8(struct S *p);
void EmitSwNoise(void *g, int n, qword pos, struct S *p);
void SetJtModeState__FP6JTMODEi(void *p, int n);
void func_00183B80(struct S *p, int n);
int func_00183E08(struct S *p, struct Q *q)
{
    struct M m;
    struct VTE *e;
    if (!func_001F7588(p, q)) return 0;
    if (p->x17c == 3) return 1;
    if (!(q->fl & 8)) return 1;
    EmitSwNoise(D_003DD490, 0, func_001A62E8(p), p);
    if (p->x190) {
        struct O *o;
        SetJtModeState__FP6JTMODEi(p->x190->xf00, -1);
        o = p->x190;
        e = &o->vt[88];
        ((void (*)(void *, int))e->pfn)((char *)o + e->d, 0);
    }
    func_00183B80(p, 3);
    m.q = q;
    m.pv = q->xc;
    e = &p->vt[3];
    ((void (*)(void *, int, int, int, struct M *, int))e->pfn)((char *)p + e->d, 0xB, 0, 2, &m, 0);
    return 1;
}
