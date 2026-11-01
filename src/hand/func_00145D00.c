struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; };
void StartBhv(void *p);
void func_00146158(void *p, int n);
/** @sly2-annot addr=0x145D00
 * Purpose: BHV195 slot 8: state 0, SetPlayerInteractMode(4, target, 4, target), set owner +0x10E0 bit 1
 * Info: proposed name StartBhv195 (T2); class BHV195 slot8; TU tu_144B28
 * Notes: superseded: OnBhv195Resume (r2b_rest) | BHV vtable slot 8 role per r9 (CONFLICTS #15); was
 *   OnBhv195Resume
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void func_00145D00(char *p)
{
    struct O *o;
    struct VTE *e;
    StartBhv(p);
    func_00146158(p, 0);
    o = *(struct O **)(p + 0x10);
    e = &o->vt[95];
    ((void (*)(void *, int, int, int, int))e->pfn)((char *)o + e->d, 4, *(int *)(p + 0x38), 4, *(int *)(p + 0x38));
    *(int *)(*(char **)(p + 0x10) + 0x10E0) |= 1;
}
