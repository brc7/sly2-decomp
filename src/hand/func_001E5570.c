struct O { char pad[0x26C]; int a; };
struct P { char pad[0x24]; struct O *po; int *pnMin; int *pnMax; int *pn; };
int func_00132B10(struct O *po, int cid);
void func_001EECD0(void *p);
/** @sly2-annot addr=0x1E5570
 * Purpose: If the current command is FollowPath (0x27) or WalkTo (0x28) and the actor is a STEPPLAYER,
 *   ClearBhvstk on it
 * Info: proposed name AbortJsgMove (T2); TU relay
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E5570(struct P *p)
{
    if (p->pnMin < p->pn && p->pn < p->pnMax) {
        switch (*p->pn) {
        case 0x27:
            if (p->po && func_00132B10(p->po, 0x34)) func_001EECD0(&p->po->a);
            break;
        case 0x28:
            if (p->po && func_00132B10(p->po, 0x34)) func_001EECD0(&p->po->a);
            break;
        }
    }
}
