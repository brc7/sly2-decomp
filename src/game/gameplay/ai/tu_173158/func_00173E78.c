struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002BE5E0;
struct E { int n; float g4; float g8; int pad[9]; };
struct S { char pad[0x5C]; int c; struct E *ae; };
/** @sly2-annot addr=0x173E78
 * Purpose: Picks the oldest free slot (or the oldest occupied one past the timeout)
 * Info: proposed name PentryCid119Free (T2); class CID119; TU tu_173158
 * Notes: Cid77->Cid119 (CID 0x77=119, agent-default(hex))
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_173158-0x173158-0x174898
 * @end-sly2-annot */
struct E *func_00173E78(struct S *p, int k) {
    float t4 = D_002DDED0.t - D_002BE5E0;
    float t8 = D_002DDED0.t;
    struct E *peBest = 0;
    struct E *pe;
    int i;
    for (i = 0; i < p->c; i++) {
        pe = &p->ae[i];
        if (pe->n != 0) {
            if (pe->n == k) continue;
            if (peBest && peBest->n == 0) continue;
            if (t4 <= pe->g4) continue;
            t4 = pe->g4;
        } else {
            if (t8 <= pe->g8) continue;
            t8 = pe->g8;
        }
        peBest = pe;
    }
    return peBest;
}
