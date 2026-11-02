struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { int a, b, c; struct VTE *vt; };
struct E { int id; int pad[3]; struct O *po; int pad2[7]; };
struct S { char pad[0x4678]; int c; int pad1; struct E ae[1]; };
#define P4744(p) (*(int *)((char *)(p) + 0x4744))
struct O *PloAcquireJtGrabMarker(struct S *, int, struct E *);
/** @sly2-annot addr=0x195608
 * Purpose: Splice entry: for candidate with id, acquire a new marker proxy (0x1956D0) and replace (vt9
 *   Remove) the old one
 * Info: proposed name SetSwGrabMarkerProxy (T2); TU grab
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grab-0x191090-0x195950
 * @end-sly2-annot */
void func_00195608(struct S *p, int id) {
    struct E *pe;
    struct O *po;
    int i;
    if (!id || !P4744(p)) return;
    for (i = 0; i < p->c; i++) {
        struct E *pq = &p->ae[i];
        if (id == pq->id) { pe = pq; break; }
    }
    if (!pe) return;
    po = PloAcquireJtGrabMarker(p, P4744(p), pe);
    if (!po) return;
    if (pe->po) { struct O *o = pe->po; struct VTE *e = &o->vt[9]; e->pfn((char *)o + e->d); }
    pe->po = po;
}
