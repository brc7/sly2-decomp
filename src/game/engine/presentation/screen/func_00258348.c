typedef struct R1 { int cref; int x4; int x8; int xC[2]; int x14[1]; } R1;
typedef struct R2 { int cref; } R2;
typedef struct R4 { int cref; int x4; R1 *p8; } R4;
extern char D_004520D0[], D_00452148[];
void func_00257250(void *, int);
void func_00129EC8(void *);
void GcBinding_Unlink(void *);
void GcSidebag_Unlink(void *);
void func_0011CD98(void *, void *);
void func_00257078(void *, int);
void func_001AE520(void *);

static inline void Release1(R1 *r)
{
    if (r) {
        if (--r->cref == 0) {
            func_00257250(r->x14, 2);
            func_00129EC8(r->xC);
            GcBinding_Unlink(r);
            func_0011CD98(D_004520D0, r);
        }
    }
}

static inline void Release4(R4 *r)
{
    if (r) {
        if (--r->cref == 0) {
            Release1(r->p8);
            GcSidebag_Unlink(r);
            func_0011CD98(D_00452148, r);
        }
    }
}

typedef struct S { R2 *p0; R4 *p4; } S;

/** @sly2-annot addr=0x258348
 * Purpose: base destructor: release binding/sidebag refs [game linkonce (inline/template)]
 * Info: proposed name BASIC_dtor (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_00258348(S *p, int flags)
{
    Release4(p->p4);
    {
        R2 *r = p->p0;
        if (r) {
            if (--r->cref == 0)
                func_00257078(r, 3);
        }
    }
    if (flags & 1)
        func_001AE520(p);
}
