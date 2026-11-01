struct VTE { short d; short pad; void (*pf)(); };
struct S { int a[3]; struct VTE *vt; char pad[0x140]; void *x; char pad2[0x6C]; void *y; void *z; void *w; };
int POCTL_FConsumePressed(void *x, int a);
void *PloAcquireProxy(void *y, int a);
void LaunchSoFromLauncher(void *z, void *v);
/** @sly2-annot addr=0x226250
 * Purpose: on fire button (0x1C4700 mask 0xFF) spawn projectile (0x1CB250/0x1D6B00) and send splice event 5
 * Info: proposed name FireTurret (T2); TU turret
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r8_21BB88
 * See: docs/engine/minigames.md#4-mounted-turret-turret-0x45-turret-tu-0x2259d8-0x226550-conf-high-ccode,
 *   docs/reference/functions/gameplay-objects.md#turret-0x2259d8-0x226550
 * @end-sly2-annot */
void func_00226250(struct S *p)
{
    if (p->x && POCTL_FConsumePressed(p->x, 0xFF)) {
        if (p->y) {
            void *v = PloAcquireProxy(p->y, 0);
            if (!v) return;
            p->w = v;
            if (p->z) LaunchSoFromLauncher(p->z, v);
            p->vt[3].pf((char *)p + p->vt[3].d, 5, 0, 0, 0, 0);
        } else
            p->vt[3].pf((char *)p + p->vt[3].d, 5, 0, 0, 0, 0);
    }
}
