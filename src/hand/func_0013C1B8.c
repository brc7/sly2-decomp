struct S { char pad[0x184]; int st; int f188; int f18c; };
void func_00133590(struct S *, int, int);
void func_0013C560(struct S *, int);
/** @sly2-annot addr=0x13C1B8
 * Purpose: cid 0x67 vt5: base; msg 4 while dropped -> state 0; msg 8 from its container -> state 4 (from 2)
 *   or 1 (from 3)
 * Info: proposed name HandleBootyMessage (T1); class BOOTY slot5; TU booty
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void func_0013C1B8(struct S *p, int msg, int a) {
    func_00133590(p, msg, a);
    switch (msg) {
    case 4:
        if (p->st == 5) func_0013C560(p, 0);
        break;
    case 8:
        if (a == p->f18c) {
            switch (p->st) {
            case 2: func_0013C560(p, 4); break;
            case 3: func_0013C560(p, 1); break;
            }
        }
        break;
    }
}
