struct VTE { short d; short pad; void (*pf)(); };
struct S { int a[3]; struct VTE *vt; char pad[0x140]; void *x; char pad2[0x6C]; void *y; void *z; void *w; };
int func_001C4700(void *x, int a);
void *func_001CB250(void *y, int a);
void func_001D6B00(void *z, void *v);
/** @sly2-annot addr=0x226250
 * Purpose: on fire button (0x1C4700 mask 0xFF) spawn projectile (0x1CB250/0x1D6B00) and send splice event 5
 * Info: proposed name FireTurret (T2); TU turret
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/gameplay-objects.md#turret-0x2259d8-0x226550
 * @end-sly2-annot */
void func_00226250(struct S *p)
{
    if (p->x && func_001C4700(p->x, 0xFF)) {
        if (p->y) {
            void *v = func_001CB250(p->y, 0);
            if (!v) return;
            p->w = v;
            if (p->z) func_001D6B00(p->z, v);
            p->vt[3].pf((char *)p + p->vt[3].d, 5, 0, 0, 0, 0);
        } else
            p->vt[3].pf((char *)p + p->vt[3].d, 5, 0, 0, 0, 0);
    }
}
