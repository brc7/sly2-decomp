typedef unsigned long ulong;
struct B4 { char a[4]; };
struct MSG { int cb; int nKind; };
struct S3 { char pad[6]; unsigned char c6; };
struct Src { int x0; char pad4; unsigned char b5; unsigned char b6; char pad7; struct B4 b8; char padc[0xC]; struct S3 *p18; };
struct Dst { int x0; int x4; int x8; struct B4 bc; int c; ulong *a; };
void *func_001AE2A0(struct MSG *);
ulong Tex0FromTexIframeCtk(struct S3 *, int, int);
/** @sly2-annot addr=0x1EB678
 * Purpose: Copy shader params and build its TEX0 array (Tex0FromTexIframeCtk per texture)
 * Info: proposed name BuildShdTex0List (T2); TU shd
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void func_001EB678(struct Src *ps, struct Dst *pd)
{
    struct S3 *p;
    struct MSG msg;
    int i;
    pd->x0 = ps->x0;
    pd->x4 = ps->b6;
    pd->x8 = ps->b5;
    pd->bc = ps->b8;
    p = ps->p18;
    {
        int c = p->c6;
        pd->c = c;
        msg.cb = c * 8;
    }
    msg.nKind = 0x10;
    pd->a = func_001AE2A0(&msg);
    for (i = 0; i < pd->c; i++)
    {
        ulong *pe = (ulong *)((i << 3) + (int)pd->a);
        *pe = Tex0FromTexIframeCtk(p, i, 0);
    }
}
