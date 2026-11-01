struct CLOCK { char pad[0x3C]; float g3C; };
extern struct CLOCK D_002DDED0;
struct Q { char pad[0x10B4]; int n; };
extern struct Q D_002DFC00;
extern int D_003DA0B8;
struct N { int *an; int c; int pad[2]; int i; int pad2; struct N *next; };
struct P { struct N *head; int pad[2]; float g; };
/** @sly2-annot addr=0x14BF48
 * Purpose: Only for g_joy (D_002DFC00), on a new non-zero press (joy+0x10B4), and only while D_003DA0B8 is
 *   set: for each unfinished code, advance ijbcCur if the press equals ajbc[ijbcCur], else reset to 0. Then
 *   tCheck = g_clock.tUnpaused + 1.0
 * Info: proposed name _MatchCodes (T1); TU cheat
 * Notes: names_v2 v1 proposed FreeGarbage__10CSplotheap (0.013)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources: semantic_tu_docs/cheat
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#cheat-0x14bea8-0x14c548
 * @end-sly2-annot */
void func_0014BF48(struct P *p, struct Q *q, unsigned short k)
{
    struct N *n;
    if (q != &D_002DFC00) return;
    if (k == q->n) return;
    if (!q->n) return;
    if (!D_003DA0B8) return;
    for (n = p->head; n; n = n->next) {
        int fDone = n->i >= n->c;
        if (!fDone) {
            if (n->an[n->i] == q->n) n->i++;
            else n->i = 0;
        }
    }
    p->g = D_002DDED0.g3C + 1.0f;
}
