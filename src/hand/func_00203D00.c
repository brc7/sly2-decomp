typedef int qword __attribute__((mode(TI)));
typedef struct { qword a[4]; } MATRIX4;
struct X { char pad[0x40]; MATRIX4 m; };
struct B { qword *p; };
struct M { struct X *px; char pad[0xC]; struct B *pb; char pad2[0x78 - 0x14]; int c; qword *av; };
struct S { char pad[0x258]; struct M *pm0; struct M *pm1; };
MATRIX4 InvertMatrix4Rigid(MATRIX4 *ps);
void __ml__FRC7MATRIX4T0(MATRIX4 *pd, MATRIX4 *pa, MATRIX4 *pb);
void *func_002333B4(void *p, int c, int n);
void func_001F4BF8(struct S *p);
void func_00203D00(struct S *p, struct M *pm, int f)
{
    p->pm1 = pm;
    if (f) {
        MATRIX4 m, mm, mi;
        MATRIX4 *pmm = &mm;
        MATRIX4 *pmi = &mi;
        int i;
        qword *ps, *pd;
        mi = InvertMatrix4Rigid(&p->pm0->px->m);
        __ml__FRC7MATRIX4T0(pmm, pmi, &p->pm1->px->m);
        m = mm;
        ps = p->pm1->av;
        pd = p->pm0->av;
        i = p->pm1->c;
        while (--i >= 0) {
            qword r;
            __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
                    : "=j"(r) : "0"(m.a[0]), "j"(m.a[1]), "j"(m.a[2]), "j"(m.a[3]), "j"(*ps));
            *pd = r;
            ps++;
            pd++;
        }
        func_002333B4(p->pm0->pb->p, 0, p->pm0->c * 16);
        func_001F4BF8(p);
    }
}
