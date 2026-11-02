typedef struct CRef { int m_tagk; int m_tag; } CRef;
struct S { char pad[0x54]; int n; char pad2[0xD8 - 0x58]; int fd8; };
extern char D_003D4A60[];
void RefGetGlobalSaveVar(CRef *pref, void *a, void *b);
void func_00129EC8(CRef *pref);
/** @sly2-annot addr=0x164030
 * Purpose: State >= 2, or for an unset task (-1) the value of its save var +0xD8
 * Info: proposed name FTaskComplete (T1); TU game
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
int FTaskComplete(struct S *p)
{
    if (p->n == -1 && p->fd8) {
        CRef ref;
        int n;
        RefGetGlobalSaveVar(&ref, &p->fd8, D_003D4A60);
        n = ref.m_tag;
        func_00129EC8(&ref);
        return n;
    }
    return p->n >= 2;
}
