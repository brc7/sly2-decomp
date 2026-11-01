typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; char p1[0x24 - 0x10]; struct LO *x24; };
struct Q { char pad[0xDC]; int xDC; };
struct S { char pad[0x64]; struct LO *x64; char p1[0xCC - 0x68]; int xCC; char p2[0xE8 - 0xD0]; struct Q *xE8; };
void func_001CB4E0(struct S *p, int n);
void func_001F7A68(struct S *p, int n);
qword func_001A62E8(void *p);
void *PpathzoneFromPos(qword q, int n);
void func_001F37C8(struct S *p, void *pv);
void func_001CB518(struct S *p, int n)
{
    struct LO *plo;
    int f;
    func_001CB4E0(p, n);
    func_001F7A68(p, 0);
    p->xCC = -1;
    f = 0;
    if (p->xE8) f = p->xE8->xDC;
    if (f) {
        func_001F37C8(p, PpathzoneFromPos(func_001A62E8(p), 1));
    }
    for (plo = p->x64; plo; plo = plo->x24)
        ((void (*)(void *, int))plo->vt[26].pf)((char *)plo + plo->vt[26].d, n);
}
