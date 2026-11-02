typedef struct R0 { int cref; int x4; int x8[2]; int x10[1]; } R0;
typedef struct R2 { int cref; } R2;
typedef struct X { int x0; int x4; R2 *p8; int xC; int x10; R0 *p14; R0 *p18; } X;
extern char D_004520A8[], D_00452120[];
void func_00256D40(void *, int);
void func_00129EC8(void *);
void func_002550F8(void *);
void func_00255AB0(void *);
void func_00257078(void *, int);
void func_0011CD98(void *, void *);

static inline void Release0(R0 *r)
{
    if (r) {
        if (--r->cref == 0) {
            func_00256D40(r->x10, 2);
            func_00129EC8(r->x8);
            func_002550F8(r);
            func_0011CD98(D_004520A8, r);
        }
    }
}

/** @sly2-annot addr=0x257148
 * Purpose: CProc destructor body: release arg/body pairs and frame, unlink, free [game linkonce
 *   (inline/template)]
 * Info: proposed name DeleteProc (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_00257148(X *p, int flags)
{
    Release0(p->p18);
    Release0(p->p14);
    {
        R2 *r = p->p8;
        if (r) {
            if (--r->cref == 0)
                func_00257078(r, 3);
        }
    }
    func_00255AB0(p);
    if (flags & 1)
        func_0011CD98(D_00452120, p);
}
