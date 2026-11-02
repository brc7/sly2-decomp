typedef struct R1 { int cref; int x4; int x8; int xC[2]; int x14[1]; } R1;
typedef struct R4 { int cref; int x4; R1 *p8; } R4;
extern char D_004520D0[], D_00452148[];
void func_00257250(void *, int);
void Decref__4CRef(void *);
void GcBinding_Unlink(void *);
void GcSidebag_Unlink(void *);
void func_0011CD98(void *, void *);

static inline void Release1(R1 *r)
{
    if (r) {
        if (--r->cref == 0) {
            func_00257250(r->x14, 2);
            Decref__4CRef(r->xC);
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

/** @sly2-annot addr=0x2580A8
 * Purpose: refptr assign for CSidebag (release frees its binding list, GcSidebag_Unlink) [game linkonce
 *   (inline/template)]
 * Info: proposed name SidebagPtr_Assign (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_002580A8(R4 **pp, R4 *p)
{
    if (p)
        p->cref++;
    Release4(*pp);
    *pp = p;
}
