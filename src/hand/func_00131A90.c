struct X { char pad[0x98]; short f98; char pad2[0xe]; float fa8; };
struct N { int f0; int f4; struct X *px; char pad[0x1c]; struct N *pnext; };
struct S { char pad[0x34]; struct N *pfirst; };
int func_0012CEE0(struct N *);
void func_002303A0(int, struct X *, int);
/** @sly2-annot addr=0x131A90
 * Purpose: For channels of kind 7 that are live: kick blend state 9 and store g
 * Info: proposed name SetAsegaChannelWeight (T2); TU asega
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources:
 *   r1_116918+verdicts_tu_r1r2+verdicts_A1
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
void func_00131A90(struct S *p, float g) {
    struct N *pn;
    for (pn = p->pfirst; pn; pn = pn->pnext) {
        if (func_0012CEE0(pn) && pn->px->f98 == 7) {
            func_002303A0(pn->f4, pn->px, 9);
            pn->px->fa8 = g;
        }
    }
}
