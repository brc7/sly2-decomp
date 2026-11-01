struct VE { short d; short pad; void (*pf)(void *, int); };
struct S { char pad[0xC]; struct VE *vt; char pad2[0xBC]; int a[10]; };
/** @sly2-annot addr=0x212880
 * Purpose: copy n ids into +0xCC (max 10) then ShowWidget(-1)
 * Info: proposed name ShowWidgetArgs (T1); TU hud
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00212880(struct S *p, int n, int *src)
{
    int i;
    for (i = 0; i < n && (unsigned)i < 10; i++)
        p->a[i] = src[i];
    p->vt[49].pf((char *)p + p->vt[49].d, -1);
}
