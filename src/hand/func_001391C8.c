struct S { char pad[0x18]; int f18; char pad2[0x38 - 0x1c]; float f38; int pad3; int f40; char pad4[0x448 - 0x44]; int f448; };
void func_00231100(struct S *);
void func_0011A1F8(void *, int);
void func_001A6CA8(struct S *, int);
/** @sly2-annot addr=0x1391C8
 * Purpose: BLIPG vt7 Init
 * Info: proposed name InitBlipg (T1); class BLIPG slot7; TU blip
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources:
 *   r2b_rest+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#blip-0x138f08-0x13a9e8
 * @end-sly2-annot */
void func_001391C8(struct S *p) {
    int x;
    func_00231100(p);
    func_0011A1F8(&p->f448, 0x1090);
    p->f18 = -1;
    p->f38 = 1e10f;
    func_001A6CA8(p, 1);
    x = p->f40;
    if ((x ^ 1) & 1) *(int *)x = (*(int *)x & 1) | 0xe;
    else p->f40 = (x & 1) | 0xe;
}
