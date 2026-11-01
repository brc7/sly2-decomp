struct VTE { short d; short pad; void (*pf)(); };
struct O { int a[3]; struct VTE *vt; char pad[0x140]; int k; };
struct Q { int a; struct O *o; };
void func_00133590(struct O *p, int k, struct Q *q);
/** @sly2-annot addr=0x2241D8
 * Purpose: base HandleLoMessage 0x133590; on msg 4 from self in state 1/2 -> SetState(3)
 * Info: proposed name HandleCid100Message (T2); class CID100 slot5; TU tu_223F70
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8
 * @end-sly2-annot */
void func_002241D8(struct O *p, int k, struct Q *q)
{
    func_00133590(p, k, q);
    if (k == 4) {
        struct O *o = q->o;
        if (o == p && (unsigned)(o->k - 1) < 2) o->vt[76].pf((char *)o + o->vt[76].d, 3);
    }
}
