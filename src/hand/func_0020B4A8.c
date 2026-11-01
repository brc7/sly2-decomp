struct S { char pad[0x40]; int fl; };
void InitLo(struct S *p);
void SetLoBusyk(struct S *p, int n);
/** @sly2-annot addr=0x20B4A8
 * Purpose: cid 0x7D vt7: InitLo, flag 2, register for update
 * Info: proposed name InitCid125GuardMgr (T2); class CID125 slot7; TU stepguard
 * Notes: Cid7D->Cid125 (CID 0x7D=125, hex-literal) | T2 forced: verifier: coined class word without
 *   descriptor
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020B4A8(struct S *p) {
    int v;
    InitLo(p);
    v = p->fl;
    if ((v ^ 1) & 1) *(int *)v |= 2; else p->fl = v | 2;
    SetLoBusyk(p, 1);
}
