struct CRef { int tagk; int tag; };
struct VE { short d; short pad; void (*pf)(void *, int); };
struct S { char pad[0xC]; struct VE *vt; char pad2[0xBC]; char xcc[4]; };
struct L { int a[4]; };
void func_002013D8(struct CRef *r, void *a, int b, int c, struct L *pl, void *d);
void func_00129EC8(struct CRef *r);
/** @sly2-annot addr=0x212828
 * Purpose: copy Splice list into +0xCC ids (max 10) then ShowWidget(-1)
 * Info: proposed name ShowWidgetArgsFromSplice (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void ShowWidgetArgsFromSplice(struct S *p, void *a)
{
    struct CRef r;
    struct L l;
    func_002013D8(&r, a, 0x10, 0xA, &l, p->xcc);
    func_00129EC8(&r);
    p->vt[49].pf((char *)p + p->vt[49].d, -1);
}
