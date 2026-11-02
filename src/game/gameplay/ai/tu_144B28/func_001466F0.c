struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; };
void StartBhv(void *p);
void func_00146E18(void *p, int n);
/** @sly2-annot addr=0x1466F0
 * Purpose: BHV197 slot 8
 * Info: proposed name StartBhv197 (T2); class BHV197 slot8; TU tu_144B28
 * Notes: superseded: OnBhv197Resume (r2b_rest) | BHV vtable slot 8 role per r9 (CONFLICTS #15); was
 *   OnBhv197Resume
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void func_001466F0(char *p)
{
    struct O *o;
    struct VTE *e;
    StartBhv(p);
    func_00146E18(p, 0);
    o = *(struct O **)(p + 0x10);
    e = &o->vt[95];
    ((void (*)(void *, int, int, int, int))e->pfn)((char *)o + e->d, 4, *(int *)(p + 0x38), 4, *(int *)(p + 0x38));
    *(int *)(*(char **)(p + 0x10) + 0x10E0) |= 1;
}
