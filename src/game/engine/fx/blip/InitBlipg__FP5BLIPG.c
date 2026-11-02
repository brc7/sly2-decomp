struct S { char pad[0x18]; int f18; char pad2[0x38 - 0x1c]; float f38; int pad3; int f40; char pad4[0x448 - 0x44]; int f448; };
void InitXfm(struct S *);
void InitDl__FP2DLi(void *, int);
void SetLoBusyk(struct S *, int);
/** @sly2-annot addr=0x1391C8
 * Purpose: BLIPG vt7 Init
 * Info: proposed name InitBlipg (T1); class BLIPG slot7; TU blip
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources:
 *   r2b_rest+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#blip-0x138f08-0x13a9e8
 * @end-sly2-annot */
void InitBlipg__FP5BLIPG(struct S *p) {
    int x;
    InitXfm(p);
    InitDl__FP2DLi(&p->f448, 0x1090);
    p->f18 = -1;
    p->f38 = 1e10f;
    SetLoBusyk(p, 1);
    x = p->f40;
    if ((x ^ 1) & 1) *(int *)x = (*(int *)x & 1) | 0xe;
    else p->f40 = (x & 1) | 0xe;
}
