typedef int qword __attribute__((mode(TI)));
struct G;
extern struct G D_003DD490;
extern char D_002F66B0[];
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct S { char pad[0xEC]; void *xEC; char pad1[0x138 - 0xF0]; int x138; int k; float t140; int x144; int c; int *a; int x150; int x154; float t158; float dt; };
void func_001A5E28(struct S *p);
void func_001B3A48(void *a, int b);
void func_0022B490(struct G *p, int a, int b, int c, int d, int e);
void *func_001C4840(struct G *pg);
qword func_001A62E8(void *p);
qword func_001A6F00(int a, struct S *p, qword q);
int PbspPointInBspQuick(void *p, int n, qword q);
void func_00228358(struct S *p, int k);
void func_002283D0(struct S *p);
void func_00228538(struct S *p)
{
    func_001A5E28(p);
    switch (p->k) {
    case 4:
        if (0.0f < p->t158 && p->t158 <= D_002DDED0.t) {
            int i;
            for (i = 0; i < p->c; i++)
                func_001B3A48(D_002F66B0, p->a[i]);
            func_0022B490(&D_003DD490, p->x144, p->x150, -1, p->x138 ? 0x20 : 0, p->x154);
            p->t158 = 0.0f;
        }
        break;
    case 0: case 1: case 3: {
        void *pj = func_001C4840(&D_003DD490);
        union { qword q; } u;
        int f;
        if (!pj) return;
        u.q = func_001A6F00(0, p, func_001A62E8(pj));
        f = PbspPointInBspQuick(p->xEC, 0x7FFFFFFF, u.q) != 0;
        switch (p->k) {
        case 3:
            if (f) func_002283D0(p);
            break;
        case 1:
            if (f) func_00228358(p, 0);
            else if (D_002DDED0.t - p->t140 > p->dt) func_00228358(p, 3);
            break;
        case 0:
            if (!f) func_00228358(p, 1);
            break;
        }
        break;
    }
    case 2:
        break;
    }
}
