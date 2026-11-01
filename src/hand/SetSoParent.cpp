typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct VT { short d; short pad; void *pfn; };
struct MAT { float m[4][4]; } __attribute__((aligned(16)));
struct E { char pad[0xA0]; struct E *pe; char pad2[0x100 - 0xA4]; };
struct LO;
struct ALO { char pad[0x40]; struct MAT mat; char pad2[0xA0 - 0x80]; struct ALO *pNext; };
struct LO10 { char pad[0x50]; int c; struct E *ae; char pad2[0x148 - 0x58]; ulong fl; };
struct LO {
    char pad[0xC]; struct VT *pvt; struct LO10 *p10; char pad2[0x28 - 0x14]; ulong pad28 : 60; ulong b60 : 1; ulong pad61 : 3;
    char pad3[0x50 - 0x30]; int c; struct E *ae; char pad4[0x60 - 0x58]; struct ALO *palo;
};
extern "C" {
void func_002333B4(void *, int, int);
void __ml__FRC7MATRIX4T0(struct MAT *, struct ALO *, struct MAT *);
void func_001A5900(struct LO *, struct LO *, struct LO *, int);
void UpdateSoXfWorldMatrices(struct LO *);
void ReleaseSoBoundsHierarchy(struct LO *);
void EnsureSoBoundsHierarchy(struct LO *);
void func_001F8668(struct LO *, int);
}

#define VCALL(po, slot, T) ((T)(po)->pvt[slot].pfn)((void *)((char *)(po) + (po)->pvt[slot].d)

extern "C" void SetSoParent(struct LO *plo, struct LO *pa, struct LO *pb, int grf)
{
    struct MAT mat;
    qword q1, q2;
    struct E *pv;
    int c, i;
    struct E *aeBase;
    if (grf & 3) {
        bool f = plo->b60; if (f) {
            mat = plo->palo->mat;
        } else {
            struct ALO *palo;
            func_002333B4(&mat, 0, 0x40);
            mat.m[0][0] = mat.m[1][1] = mat.m[2][2] = mat.m[3][3] = 1.0f;
            for (palo = plo->palo; palo; palo = palo->pNext) {
                struct MAT t;
                __ml__FRC7MATRIX4T0(&t, palo, &mat);
                mat = t;
            }
        }
    }
    if (grf & 4) q1 = VCALL(plo, 33, qword (*)(void *)));
    if (grf & 8) q2 = VCALL(plo, 34, qword (*)(void *)));
    if (plo->p10) {
        c = plo->p10->c;
        aeBase = plo->p10->ae;
    } else {
        c = 1;
        aeBase = 0;
    }
    if (pb) pv = (struct E *)pb; else if (pa) pv = (struct E *)pa->palo; else pv = 0;
    for (i = 0; i < plo->c; i++) {
        struct E *pe = &plo->ae[i];
        int d = pe->pe - aeBase;
        if (d >= 0 && d < c) pe->pe = pv;
    }
    func_001A5900(plo, pa, pb, grf);
    UpdateSoXfWorldMatrices(plo);
    if (grf & 1) VCALL(plo, 29, void (*)(void *, qword)), *(qword *)mat.m[3]);
    if (grf & 2) VCALL(plo, 30, void (*)(void *, struct MAT *)), &mat);
    if (grf & 4) VCALL(plo, 35, void (*)(void *, qword)), q1);
    if (grf & 8) VCALL(plo, 36, void (*)(void *, qword)), q2);
    ReleaseSoBoundsHierarchy(plo);
    EnsureSoBoundsHierarchy(plo);
    if (plo->p10 && !(plo->p10->fl & (1L << 35))) func_001F8668(plo, 0);
}
