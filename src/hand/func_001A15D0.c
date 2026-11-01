struct PMF { short delta; unsigned short idx; void (*pfn)(void *); };
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { int x0, x4, x8; struct VTE *vt; };
struct B { char pad[0x148]; unsigned long x148; };
struct S { struct O *x0; int x4; char pad[0x14]; struct PMF pmf; char pad2[0xC]; struct B *x30; };
void *func_002333B4(void *p, int c, unsigned int n);
void func_001A16A8(struct S *p);
void func_00195A00(void *p, int a, int b, int c);
/** @sly2-annot addr=0x1A15D0
 * Purpose: Take and clear the pending pmf callback and call it; with none, death states 8..12 return to a
 *   safe place, others SetJtAirJump
 * Info: proposed name RunJtHurtCallback (T1); TU hurt
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
void func_001A15D0(struct S *s)
{
    struct PMF pmf = s->pmf;
    func_002333B4(&s->pmf, 0, sizeof(s->pmf));
    if (pmf.idx) {
        pmf.pfn((char *)s + pmf.delta);
    } else if ((unsigned int)(s->x4 - 8) < 5) {
        struct O *o = s->x0;
        struct VTE *e = &o->vt[0x2E8 / 8];
        e->pfn((char *)o + e->d);
    } else if (s->x30 && (s->x30->x148 & 0x200000000UL)) {
        func_001A16A8(s);
    } else {
        func_00195A00((char *)s->x0 + 0x2C0, 11, 0, 0);
    }
}
