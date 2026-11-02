typedef struct R { int cref; char pad[8]; int xC[2]; int x14[1]; } R;
typedef struct X { char pad[0xC]; int a[4]; R *pr; } X;
extern char D_004520D0[], D_004520F8[];
void func_00257250(void *, int);
void func_00129EC8(void *);
void GcBinding_Unlink(void *);
void GcFrame_Unlink(void *);
void func_0011CD98(void *, void *);
void func_002572E0(void *, int);

/** @sly2-annot addr=0x257078
 * Purpose: CFrame destructor (gcc 2.95 deleting form with __in_chrg): release binding list ref, release the 4
 *   parent frame refs, unlink from the GC frame list, operator delete to pool D_004520F8 when flags&1
 * Info: proposed name CFrame_dtor (T2); TU game_linkonce
 * Notes: v3: superseded DeleteFrame | top50_round (0.55): Body: GcFrame_Unlink, FramePtr_Release on a[4]
 *   (CFrame_AddParent stores parents there), FreeSlotheapPv(D_004520F8) under flags&1 (the gcc 2.95
 *   destructor delete bit). FramePtr_Assign calls it with 3. Class name CFrame fr | mangled not used
 *   (signature_confident=no): _$_6CFrame
 * Confidence: low (draft) [T2 0.55] - evidence: semantic review; sources: sdk_names+top50_round
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_00257078(X *p, int flags)
{
    R *r = p->pr;
    if (r) {
        if (--r->cref == 0) {
            func_00257250(r->x14, 2);
            func_00129EC8(r->xC);
            GcBinding_Unlink(r);
            func_0011CD98(D_004520D0, r);
        }
    }
    {
        int *b = p->a;
        if (b) {
            int *e = b + 4;
            while (b != e) {
                --e;
                func_002572E0(e, 0);
            }
        }
    }
    GcFrame_Unlink(p);
    if (flags & 1)
        func_0011CD98(D_004520F8, p);
}
