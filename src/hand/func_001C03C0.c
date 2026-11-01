typedef int qword __attribute__((mode(TI)));
struct A { int cb; int align; };
struct E8 { struct V *pv; int x4; };
struct V { qword pos; int c; struct E8 *ae; int pad[2]; };
struct F { struct V *apv[3]; };
struct P { float x, y, z; unsigned short c, d; };
struct S { char pad[0x18]; int x18; char p0[0xE0 - 0x1C]; int cE0; struct V *aE4; int cE8; struct F *aEC; int cF0; struct P *aF4; char p1[0x114 - 0xF8]; int x114; };
struct G { char pad[0x3D70]; int x3D70; char p1[0x45C8 - 0x3D74]; struct S *x45C8; };
extern struct G D_003DD490;
void func_00231170(struct S *p, void *pbis);
int func_00117C20(void *pbis);
int func_00117BD0(void *pbis);
void func_00117EB0(void *pbis, struct V *pv);
float func_00117E38(void *pbis);
void *func_001AE2A0(struct A *pa);
void *func_001AE1D0(struct A *pa);
void func_001C0880(int *p);
/** @sly2-annot addr=0x1C03C0
 * Purpose: CID 17 vt14: LoadXfmFromBrx, read vertices (pos + edge list), triangles, BSP nodes; HookupCg
 * Info: proposed name LoadPathzoneFromBrx (T1); class PATHZONE slot14; TU path
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5b_rest+parse-verified
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void func_001C03C0(struct S *p, void *pbis)
{
    struct A a;
    int i, j, k;
    func_00231170(p, pbis);
    p->cE0 = func_00117C20(pbis);
    a.cb = p->cE0 * 32;
    a.align = 0x10;
    p->aE4 = func_001AE2A0(&a);
    p->cE8 = func_00117C20(pbis);
    a.cb = p->cE8 * 12;
    a.align = 0x10;
    p->aEC = func_001AE2A0(&a);
    for (i = 0; i < p->cE0; i++) {
        struct V *pd = &p->aE4[i];
        func_00117EB0(pbis, pd);
        pd->c = func_00117BD0(pbis);
        a.cb = pd->c * 8 + 8;
        a.align = 0x10;
        pd->ae = func_001AE1D0(&a);
        for (j = 0; j < pd->c; j++) {
            struct E8 *pe = &pd->ae[j];
            pe->pv = &p->aE4[func_00117C20(pbis)];
        }
    }
    for (k = 0; k < p->cE8; k++) {
        struct V **ppv = p->aEC[k].apv;
        for (i = 0; i < 3; i++)
            *ppv++ = &p->aE4[func_00117C20(pbis)];
    }
    p->cF0 = func_00117C20(pbis);
    a.cb = p->cF0 * 16;
    a.align = 0x10;
    p->aF4 = func_001AE2A0(&a);
    for (i = 0; i < p->cF0; i++) {
        struct P *pp = &p->aF4[i];
        pp->x = func_00117E38(pbis);
        pp->y = func_00117E38(pbis);
        pp->z = func_00117E38(pbis);
        pp->c = func_00117C20(pbis);
        pp->d = func_00117C20(pbis);
    }
    func_001C0880(&p->cE0);
    if (p->x18 == 0x2CA)
        D_003DD490.x45C8 = p;
    p->x114 = ++D_003DD490.x3D70;
}
