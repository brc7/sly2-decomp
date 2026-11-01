typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct X { char pad[0x14]; int n14; float g18; };
struct E { struct O *po; int pad; float g8; struct X *px; int k; int pad2[3]; qword q; };
struct S { char pad[0x58]; int c58; char pad2[0xA0 - 0x5C]; float gA0; float gA4; };
qword func_00173A18(struct S *p);
/** @sly2-annot addr=0x173928
 * Purpose: Ends a spawned-guard slot: kind 0 removes the guard (vt9 RemoveLo), kind 1 sets a respawn delay;
 *   decrements count
 * Info: proposed name ReleaseCid119Entry (T2); class CID119; TU tu_173158
 * Notes: Cid77->Cid119 (CID 0x77=119, agent-default(hex))
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_173158-0x173158-0x174898
 * @end-sly2-annot */
void func_00173928(struct S *p, struct E *pe, int k, int f) {
    pe->k = k;
    switch (k) {
    case 0:
        { struct O *o = pe->po; struct VTE *e; pe->g8 = D_002DDED0.t; e = &o->vt[9]; e->pfn((char *)o + e->d); }
        if ((f ^ 1) & 1) pe->px->g18 = D_002DDED0.t;
        break;
    case 1:
        pe->g8 = D_002DDED0.t + p->gA0;
        pe->q = func_00173A18(p);
        if ((f ^ 1) & 1) pe->px->g18 = D_002DDED0.t + p->gA4;
        break;
    }
    pe->po = 0;
    pe->px->n14 = 0;
    pe->px = 0;
    p->c58--;
}
