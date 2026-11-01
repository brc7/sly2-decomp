struct S { char pad[0x64]; void *f64; };
struct T { char pad[0x7C]; struct S *ps; };
extern struct T *D_003E0FA0;
extern char D_002DEB30[];
void func_00162940(struct S *p);
void func_00164DF0(void *p, int n);
void func_00163CE0(struct T *p);
void func_001619A0(void *p, int n, struct S *ps);
void func_00162908(struct S *p);
/** @sly2-annot addr=0x162EA0
 * Purpose: Resets the job's tasks, sets its start task back to available, ends the current task if it
 *   belongs, event 0x1F, EndCurrentJob
 * Info: proposed name AbortJob (T1); TU game
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00162EA0(struct S *p)
{
    func_00162940(p);
    func_00164DF0(p->f64, 1);
    if (D_003E0FA0 && D_003E0FA0->ps == p)
        func_00163CE0(D_003E0FA0);
    func_001619A0(D_002DEB30, 0x1F, p);
    func_00162908(p);
}
