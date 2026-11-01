typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pf)(); };
struct MAT3 { qword a[3]; };
struct MAT4 { float m[4][4]; };
struct Y { char pad[0x24]; int x24; };
struct LO { char pad[0xC]; struct VTE *vt; char p1[0x3C - 0x10]; struct Y *x3C; };
struct X { qword q; struct MAT3 mat; };
struct Z { qword q; struct MAT4 mat; };
struct A { int cb; int align; };
struct R { int n; struct LO **ap; int x8; int c; int x10; void *x14; void **ap18; char pad[0x64 - 0x1C]; struct X *x64; struct Z *x68; };
struct G { char pad[0x3F04]; int c3F04; struct R *ar; };
void *func_001C9940(int i, int c);
void *func_001AE2A0(struct A *pa);
qword func_001A62E8(struct LO *p);
struct MAT3 *func_001A6318(struct LO *p);
void func_0011B3D0(struct MAT3 *pm);
void *func_002333B4(void *p, int c, int cb);
static inline void SetIdent(struct MAT4 *pm) { func_002333B4(pm, 0, 0x40); pm->m[0][0] = pm->m[1][1] = pm->m[2][2] = pm->m[3][3] = 1.0f; }
/** @sly2-annot addr=0x1CA500
 * Purpose: After load: turn every unused copy of each proxy source into a spare (rest pose array aposRest,
 *   PROXYSPARE array, identity mats)
 * Info: proposed name InitSwProxySpares (T1); TU proxy
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
void func_001CA500(struct G *pg)
{
    int i, j, k;
    struct LO *plo;
    struct X *px;
    struct A a;
    for (i = 0; i < pg->c3F04; i++) {
        struct R *pr = &pg->ar[i];
        if (pr->c >= pr->x8) continue;
        for (j = pr->c; j < pr->x8; j++) {
            void *v = func_001C9940(i, j);
            pr->ap18[j] = pr->x14;
            for (k = 0; k < pr->n; k++) {
                plo = pr->ap[j * pr->n + k];
                ((void (*)(void *, void *, int))plo->vt[25].pf)((char *)plo + plo->vt[25].d, v, 1);
                plo->x3C->x24 = 1;
            }
        }
        a.cb = pr->n * 64;
        a.align = 0x10;
        pr->x64 = func_001AE2A0(&a);
        for (k = 0; k < pr->n; k++) {
            px = &pr->x64[k];
            plo = pr->ap[pr->c * pr->n + k];
            px->q = func_001A62E8(plo);
            px->mat = *func_001A6318(plo);
            func_0011B3D0(&px->mat);
        }
        a.cb = (pr->x8 - pr->c) * 0x50;
        a.align = 0x10;
        pr->x68 = func_001AE2A0(&a);
        for (j = pr->c; j < pr->x8; j++)
            SetIdent(&pr->x68[j - pr->c].mat);
    }
}
