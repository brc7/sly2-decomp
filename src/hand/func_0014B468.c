struct MSG { int kind; int pad[3]; int val; char pad2[0x70 - 0x14]; };
void *memset(void *p, int c, unsigned int n);
void *func_00205458(void *p, int n);
void func_00176750(void *p, struct MSG *m);
/** @sly2-annot addr=0x14B468
 * Purpose: Splice: send a value message to its behaviour 0x9C
 * Info: proposed name SetCarmelitaBhv9C (T2); class CARMELITA; TU tu_14A008
 * Notes: Cid3F->Cid63 (CID 0x3F=63, hex-literal) | verifier PLAUSIBLE: Placeholder (Cid3F/Bhv9C, master
 *   already 'Cid63'). Body: PbhvFindStep(p,0x9C); val!=0 -> msg kind 0xE carrying val; val==0 -> msg kind 1
 *   only if bhv 0xB6 exists. Purpose omits the val==0 / 0xB6 branch | class identity from whole_game WE
 *   names/object strings: SetCid63Bhv9C -> SetCarmelitaBhv9C | T2 forced: verifier: placeholder name
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources:
 *   r2b_rest+verdicts_followups+whole_game(class identity)
 * See: docs/reference/functions/guards-ai.md#tu_14a008-0x14a008-0x14b660
 * @end-sly2-annot */
void func_0014B468(void *p, int val)
{
    struct MSG m;
    void *o;
    memset(&m, 0, sizeof(m));
    o = func_00205458(p, 0x9C);
    if (val) {
        m.val = val;
        m.kind = 0xE;
        func_00176750(o, &m);
    } else if (func_00205458(p, 0xB6)) {
        m.kind = 1;
        func_00176750(o, &m);
    }
}
