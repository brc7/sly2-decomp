struct DL { void *a, *b; int ib; };
struct S { char pad[0x58]; int n58; struct DL dl; };
void InitLo(struct S *p);
void InitDl__FP2DLi(struct DL *pdl, int ib);
/** @sly2-annot addr=0x1DFAD8
 * Purpose: RELAY::Init (vt7): InitLo; crelaycmdMax(+0x58)=-1; InitDl(&dlJsg +0x5C, ibDle 0xE4)
 * Info: proposed name InitRelay (T1); class RELAY slot7; TU relay
 * Notes: superseded r6_1D5150 proposal: InitSgg? (0.35)
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6b_rest+verdicts_followups
 * See: docs/engine/cutscenes.md#4-source-files, docs/engine/cutscenes.md#5-key-functions,
 *   docs/engine/gameplay-objects.md#source-files
 * @end-sly2-annot */
void func_001DFAD8(struct S *p) {
    InitLo(p);
    p->n58 = -1;
    InitDl__FP2DLi(&p->dl, 0xE4);
}
