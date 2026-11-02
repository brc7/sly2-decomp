struct CRef { int m_tagk; int m_tag; };
struct G { char pad[0xFD50]; int xFD50; int xFD54; int xFD58; };
struct H { char pad[0x4C]; int x4C; };
extern struct G D_002E65E0;
extern struct H D_002E6550;
extern int D_003E0FE4;
extern char D_003D4A60[];
void RefGetGlobalSaveVar(struct CRef *ret, int a, void *b);
void func_00129EC8(struct CRef *);
void SetMcDialogState(struct H *h, int n);
void func_001ABD40(struct G *g);
/** @sly2-annot addr=0x1AD3E0
 * Purpose: Eval slot, set dialog state, start save job (0x1ABD40)
 * Info: proposed name SaveToSlot (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001AD3E0(void)
{
    struct G *g = &D_002E65E0;
    struct H *h = &D_002E6550;
    struct CRef ref;
    int t;
    g->xFD50 = g->xFD58;
    h->x4C = 2;
    RefGetGlobalSaveVar(&ref, D_003E0FE4, D_003D4A60);
    t = ref.m_tag;
    func_00129EC8(&ref);
    if (!t) {
        SetMcDialogState(h, 0x22);
    } else {
        SetMcDialogState(h, 0x1A);
        func_001ABD40(g);
    }
}
