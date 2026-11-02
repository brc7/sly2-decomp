typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x90]; int n90; char pad2[4]; short k; };
struct E { char pad[0x80]; qword q; };
struct P { char pad[0x58]; struct O *po; char pad2[0xE8 - 0x5C]; struct E *pe; };
void func_002303A0(struct P *, struct O *, int);
void func_001F5078(struct P *);
/** @sly2-annot addr=0x1F5880
 * Purpose: Make pos XF dynamic if needed and store gravity vector, invalidate
 * Info: proposed name SetSoDvGravity (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void SetSoDvGravity(struct P *p, qword q)
{
    if (p->pe) {
        struct O *po = p->po;
        if (po->n90 == 0 && po->k != 2) func_002303A0(p, po, 1);
        p->pe->q = q;
        func_001F5078(p);
    }
}
