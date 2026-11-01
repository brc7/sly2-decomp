typedef struct VTE { short d; short pad; void (*pfn)(); } VTE;
typedef struct VO { char pad[0xC]; VTE *vt; } VO;
typedef struct R { char pad[0x20]; struct P *owner; char pad2[4]; } R;
typedef struct O {
    char pad0[0xC];
    VTE *vt;
    char pad10[4];
    VO **x14;
    char pad18[0x28 - 0x18];
    unsigned long x28;
    char pad30[0x40 - 0x30];
    int x40;
} O;
typedef struct P {
    char pad0[0xC];
    VTE *vt;
    char pad10[0xE0 - 0x10];
    int c;
    O *ap[4];
    int af[4];
    char pad104[0x128 - 0x104];
    R *pr;
} P;
extern R D_003E1A78[];
extern char D_003DD4BC[];
int func_001A61A0(O *o);
void AppendDlEntry__FP2DLPv(void *pdl, void *p);

R *func_001BEB28(P *p)
{
    unsigned int i;
    int j;
    if (p->pr) return p->pr;
    for (i = 0; i < 2; i++) {
        R *pr = &D_003E1A78[i];
        if (!pr->owner) {
            p->pr = pr;
            break;
        }
    }
    if (p->pr) {
        p->pr->owner = p;
        for (j = 0; j < p->c; j++) {
            O *o = p->ap[j];
            int x;
            p->af[j] = 0;
            if (!((int)(o->x28 >> 60) & 1)) {
                p->af[j] = 1;
                ((void (*)(void *))o->vt[8].pfn)((char *)o + o->vt[8].d);
                if (!func_001A61A0(o)) {
                    VO *q;
                    AppendDlEntry__FP2DLPv(D_003DD4BC, o->x14);
                    q = *o->x14;
                    ((void (*)(void *))q->vt[23].pfn)((char *)q + q->vt[23].d);
                }
            }
            x = o->x40;
            if ((x ^ 1) & 1) x = *(int *)x;
            if (x & 8) {
                p->af[j] |= 2;
                ((void (*)(void *, int))o->vt[21].pfn)((char *)o + o->vt[21].d, 0);
            }
        }
        ((void (*)(void *, int))p->vt[21].pfn)((char *)p + p->vt[21].d, 1);
        ((void (*)(void *, int, int, int, int, int))p->vt[3].pfn)((char *)p + p->vt[3].d, 2, 0, 0, 0, 0);
    }
    return p->pr;
}
