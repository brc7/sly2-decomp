struct CRef { int m_tagk; int m_tag; };
struct G { char pad[0x3B54]; int x3B54; };
struct H { char pad[0x4C]; int x4C; };
struct S { char pad[0xFD50]; void *xFD50; };
extern struct G D_003DD490;
extern struct H D_002E6550;
extern int D_002DEB4C;
extern char D_003D4A60[], D_002E65A0[];
int func_0020E040(struct G *g);
int NWorldCur(void);
void RefGetGlobalSaveVar(struct CRef *ret, int a, void *b);
void Decref__4CRef(struct CRef *);
void SetMcDialogState(struct H *h, int n);
void func_001ABCE8(struct S *s);
void func_001ADE38(void *p);
/** @sly2-annot addr=0x1ABD98
 * Purpose: If a slot is current and not in transition: eval Splice flag; if card missing show UI state 0x22
 *   else start save job
 * Info: proposed name AutoSaveGame (T2); TU memcard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001ABD98(struct S *s, int force)
{
    struct G *g;
    struct CRef ref;
    int t;
    if (!s->xFD50)
        return;
    g = &D_003DD490;
    if (func_0020E040(g))
        return;
    if (D_002DEB4C)
        return;
    if (!NWorldCur() && !force)
        return;
    RefGetGlobalSaveVar(&ref, g->x3B54, D_003D4A60);
    t = ref.m_tag;
    Decref__4CRef(&ref);
    if (!t) {
        D_002E6550.x4C = 3;
        SetMcDialogState(&D_002E6550, 0x22);
    } else {
        func_001ABCE8(s);
        func_001ADE38(D_002E65A0);
    }
}
