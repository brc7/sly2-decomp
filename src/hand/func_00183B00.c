typedef int qword __attribute__((mode(TI)));
struct M { float a[12]; };
struct S { char pad[0x17C]; int x17c; char p1[0xC]; int x18c; };
extern qword D_002A2DD0;
extern char D_002BE8D0[], D_002BE8E0[];
void func_001F9B20(struct S *p);
void *func_001A6318(struct S *p);
void TiltMatUpright(void *p, int n, struct M *pm);
void AccelSoTowardMatSpring(struct S *p, struct M *pm, void *a, qword q, void *b);
void func_00183B00(struct S *p)
{
    func_001F9B20(p);
    if (p->x18c && !p->x17c) {
        struct M m;
        TiltMatUpright(func_001A6318(p), 0, &m);
        AccelSoTowardMatSpring(p, &m, D_002BE8D0, D_002A2DD0, D_002BE8E0);
    }
}
