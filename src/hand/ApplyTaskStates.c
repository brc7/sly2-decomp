struct E { int oid; int n; };
extern char D_003DD490[];
void *PloFindSwObject__FP2SWi3OIDP2LO(void *pg, int kind, int oid, void *owner);
void RequestTaskState(void *p, int n, void *owner);
/** @sly2-annot addr=0x161F48
 * Purpose: For each (oid,state) pair finds the TASK (PloFindSwObject grffso 0x105) and requests that state
 *   (RequestTaskState)
 * Info: proposed name ApplyTaskStates (T1); TU game
 * Notes: names_v2 C proposed SetStepguardTargetHeading__FP9STEPGUARDff (0.438)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270,
 *   docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
void ApplyTaskStates(void *pv, int c, struct E *ae, void *owner)
{
    int i;
    for (i = 0; i < c; i++) {
        void *p = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 0x105, ae[i].oid, owner);
        if (p)
            RequestTaskState(p, ae[i].n, owner);
    }
}
