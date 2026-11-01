struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { int a, b, c; struct VTE *vt; };
struct P { char pad[0x100]; struct O *o; };
struct S { char pad[0x30]; int c; struct P *a[1]; };
/** @sly2-annot addr=0x201E88
 * Purpose: vt slot 9 on each target companion object (+0x100)
 * Info: proposed name ResetReconTargets (T2); TU tu_201D78
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
void func_00201E88(struct S *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct O *o = p->a[i]->o;
        if (o) { struct VTE *e = &o->vt[9]; e->pfn((char *)o + e->d); }
    }
}
