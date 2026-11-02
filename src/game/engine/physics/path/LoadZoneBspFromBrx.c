typedef struct VECTOR { float x, y, z, w; } __attribute__((aligned(16))) VECTOR;
struct E { VECTOR *pv; short a; short b; };
struct S { int c0; struct E *ae; int c8; VECTOR *av; int c10; int *a14; void *x18; };
struct A { int cb; int align; };
int U32Read__18CBinaryInputStream(void *pbis);
int U16Read__18CBinaryInputStream(void *pbis);
void ReadVector4__18CBinaryInputStreamP7VECTOR4(void *pbis, VECTOR *pv);
int S32Read__18CBinaryInputStream(void *pbis);
void *func_001AE1D0(struct A *pa);
void *func_001AE2A0(struct A *pa);
/** @sly2-annot addr=0x1C0F30
 * Purpose: Read zone BSP nodes (plane index + 2 children), planes, leaf zone ids
 * Info: proposed name LoadZoneBspFromBrx (T1); TU path
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r5b_rest+parse-verified
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/formats/level-objects.md#2-per-world-entry-header-loop-body-of-loadswfrombrx,
 *   docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void LoadZoneBspFromBrx(struct S *p, int c, void *pbis)
{
    struct A a;
    int i, j;
    p->c0 = c;
    p->c8 = U32Read__18CBinaryInputStream(pbis);
    p->c10 = U32Read__18CBinaryInputStream(pbis);
    a.cb = p->c0 * 8;
    a.align = 0x10;
    p->ae = func_001AE1D0(&a);
    a.cb = p->c8 * 16;
    a.align = 0x10;
    p->av = func_001AE1D0(&a);
    a.cb = p->c10 * 4;
    a.align = 0x10;
    p->x18 = func_001AE2A0(&a);
    a.cb = p->c10 * 4;
    a.align = 0x10;
    p->a14 = func_001AE1D0(&a);
    for (i = 0; i < p->c0; i++) {
        struct E *pe = &p->ae[i];
        pe->pv = &p->av[U16Read__18CBinaryInputStream(pbis)];
        pe->a = U16Read__18CBinaryInputStream(pbis);
        pe->b = U16Read__18CBinaryInputStream(pbis);
    }
    for (j = 0; j < p->c8; j++)
        ReadVector4__18CBinaryInputStreamP7VECTOR4(pbis, &p->av[j]);
    for (j = 0; j < p->c10; j++)
        p->a14[j] = S32Read__18CBinaryInputStream(pbis);
}
