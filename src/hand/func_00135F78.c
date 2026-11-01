struct S { char pad[0x4c0]; int f4c0; char pad2[0x13d0 - 0x4c4]; int st; };
int func_00194640(void *, int);
int func_00140ED8(struct S *, int);
/** @sly2-annot addr=0x135F78
 * Purpose: BENTLEY vt76: pipe state and zpk 0x4000 or jtgrab check, else STEPPLAYER::FInvulnerable
 * Info: proposed name BENTLEY_FInvulnerable (T1); class BENTLEY slot76; TU bentley
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
int func_00135F78(struct S *p, int msg) {
    switch (p->st) { case 13: if (msg == 0x4000) return 1; break; case 3: if (func_00194640(&p->f4c0, msg)) return 1; break; }
    return func_00140ED8(p, msg);
}
