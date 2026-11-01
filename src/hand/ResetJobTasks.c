struct X { char pad[0x64]; int f64; char pad2[0x84 - 0x68]; struct X *pNext; };
struct S { char pad[0x7C]; int f7c; struct X *pFirst; };
void UpdateTaskSaveVars(struct X *p);
void UpdateJobSaveVars(struct S *p);
/** @sly2-annot addr=0x162940
 * Purpose: Clears each child task's start count (+0x64) and refreshes its save vars, zeroes the job's count,
 *   refreshes job save vars
 * Info: proposed name ResetJobTasks (T1); TU game
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void ResetJobTasks(struct S *p)
{
    struct X *px;
    for (px = p->pFirst; px; px = px->pNext) {
        px->f64 = 0;
        UpdateTaskSaveVars(px);
    }
    p->f7c = 0;
    UpdateJobSaveVars(p);
}
