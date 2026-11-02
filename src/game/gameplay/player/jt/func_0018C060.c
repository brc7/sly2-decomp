struct S { char pad[0x2C0]; char x2c0[0x200]; char x4c0[0x60]; void *x520; char p1[0x8CC]; char xdf0[0x930]; void *a1720[0x6A]; int n; };
void *func_001944C8(void *p, int *pn);
void *func_001A23F8(void *p, int *pn);
void *func_0019C870(void *p, int *pn);
void SetStepPhys(struct S *p, void *pv, int n);
/** @sly2-annot addr=0x18C060
 * Purpose: JT vt90 (pure virtual in STEPPLAYER): get state-specific array from jts 7/4/2 sub-objects and pass
 *   to 0x203D00
 * Info: proposed name JT_vt90 (T2); class JT slot90; TU jt
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018C060(struct S *p)
{
    int n = 0;
    void *pv;
    switch (p->n) {
    case 7: pv = func_001944C8(p->x4c0, &n); break;
    case 4: pv = func_001A23F8(p->xdf0, &n); break;
    case 2: pv = func_0019C870(p->x2c0, &n); break;
    case 21: case 22: pv = p->x520; break;
    default: pv = p->a1720[p->n]; break;
    }
    if (!pv) pv = p->a1720[0];
    SetStepPhys(p, pv, n);
}
