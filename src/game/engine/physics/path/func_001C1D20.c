struct E { float t; unsigned short k; unsigned short bit; };
struct S { char pad[0xFC]; int xFC; int x100; struct E *x104; struct E *x108; void **x10C; };
struct A { int cb; int align; };
extern const float D_002A76E0;
extern const float D_002A76E4;
void *func_001AE1D0(struct A *pa);
void func_001AE5A0(void *dst, void *src, int cb);
void *func_002333B4(void *p, int c, int cb);
/** @sly2-annot addr=0x1C1D20
 * Purpose: Grow obstacle/event arrays 0->16->128 with sentinel events
 * Info: proposed name GrowPathzoneObstacleArrays (T2); TU path
 * Notes: names_v2 v1 proposed FUN_001725e0 (0.000)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void func_001C1D20(struct S *p)
{
    struct A a;
    int n;
    int cb;
    struct E *ae1, *ae2;
    void **apv;
    switch (p->xFC) {
    case 0: n = 0x10; break;
    case 0x10: n = 0x80; break;
    case 0x80: break;
    }
    cb = n * 16 + 16;
    a.cb = cb;
    a.align = 0x10;
    ae1 = func_001AE1D0(&a);
    a.cb = cb;
    a.align = 0x10;
    ae2 = func_001AE1D0(&a);
    a.cb = n * 4;
    a.align = 0x10;
    apv = func_001AE1D0(&a);
    func_001AE5A0(ae1, p->x104, p->x100 * 8);
    func_001AE5A0(ae2, p->x108, p->x100 * 8);
    func_001AE5A0(apv, p->x10C, p->xFC * 4);
    { int cz = (n - p->xFC) * 4; func_002333B4((void *)(p->xFC * 4 + (int)apv), 0, cz); }
    p->x108 = ae2;
    p->x10C = apv;
    p->xFC = n;
    p->x104 = ae1;
    if (p->x100 == 0) {
        p->x104[0].t = D_002A76E0;
        p->x104[0].k = 1;
        p->x104[0].bit = 0xFF;
        p->x104[1].t = D_002A76E4;
        p->x104[1].k = 0;
        p->x104[1].bit = 0xFF;
        p->x108[0].t = D_002A76E0;
        p->x108[0].k = 1;
        p->x108[0].bit = 0xFF;
        p->x108[1].t = D_002A76E4;
        p->x108[1].k = 0;
        p->x108[1].bit = 0xFF;
        p->x100 = 2;
    }
}
