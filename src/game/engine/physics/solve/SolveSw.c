struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[18]; };
struct X { char pad[0xC]; struct VT *pvt; };
struct E { int fl; struct X *px; };
struct P { char pad[0x3F14]; int c; int pad2; int *pi; struct E *ae; };
void RecalcSwXpAll(struct P *);
void func_001F8F80(struct P *, int);
/** @sly2-annot addr=0x1F8DD8
 * Purpose: Per-frame physics solve over the world: pre-pass 0x134040, vt17 on entries with (grf&0x24)==4,
 *   then solve contact groups (0x1F8F80)
 * Info: proposed name SolveSw (T1); TU solve
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/overview.md#2-the-frame-loop-exact-order,
 *   docs/engine/overview.md#5-per-frame-update-and-render,
 *   docs/engine/physics-collision.md#2-contacts-and-solving
 * @end-sly2-annot */
void SolveSw(struct P *p)
{
    int i;
    RecalcSwXpAll(p);
    p->pi = &i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if ((pe->fl & 0x24) == 4) pe->px->pvt->a[17].pfn((char *)pe->px + pe->px->pvt->a[17].d);
    }
    p->pi = 0;
    func_001F8F80(p, 1);
    RecalcSwXpAll(p);
    func_001F8F80(p, 6);
}
