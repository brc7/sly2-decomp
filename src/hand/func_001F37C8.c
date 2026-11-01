struct Q { char pad[0x118]; char dl[1]; };
struct O { char pad[0x10]; struct Q *pq; char pad2[0x20 - 0x14]; char sub[1]; };
struct E { char pad[0xDC]; struct O *po; };
struct P { char pad[0xE8]; struct E *pe; };
int func_0011A380(void *, void *);
void func_0011A2E8(void *, void *);
void func_0011A220(void *, void *);
void func_001C1440(struct Q *, void *);
/** @sly2-annot addr=0x1F37C8
 * Purpose: Move the SOPHYS region link (+0xDC) from its current region DL (+0x118) to region q
 * Info: proposed name SetSoPhysRegion (T2); TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F37C8(struct P *p, struct Q *q)
{
    struct O *o = p->pe->po;
    struct Q *old = o->pq;
    if (old == q) return;
    if (old) {
        if (func_0011A380(old->dl, o)) func_0011A2E8(old->dl, o);
    }
    if (q) {
        if (!func_0011A380(q->dl, o)) {
            func_0011A220(q->dl, o);
            func_001C1440(q, o->sub);
        }
    }
    o->pq = q;
}
