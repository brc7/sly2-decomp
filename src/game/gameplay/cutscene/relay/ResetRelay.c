struct S { char p0[0x50]; int n50; char p1[0x7C-0x54]; int n7C; };
void StopRelay(struct S *p);
void AddRelayBlockBegin(struct S *p, int n);
/** @sly2-annot addr=0x1DFFE0
 * Purpose: StopRelay, drop all commands (crelaycmd=0), re-add BlockBegin(-1), fAppendEnd=1
 * Info: proposed name ResetRelay (T1); TU relay
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void ResetRelay(struct S *p) {
    StopRelay(p);
    p->n50 = 0;
    AddRelayBlockBegin(p, -1);
    p->n7C = 1;
}
