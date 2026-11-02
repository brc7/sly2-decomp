struct S { char pad[0xF4]; int a; int b; int c; char pad2[0x10]; int d; };
void InitXfm(struct S *p);
/** @sly2-annot addr=0x203568
 * Purpose: CID 0x2E Init: InitXfm, ids -1/-2
 * Info: proposed name InitReconTarget (T1); class CID46 slot7; TU tu_201D78
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
void InitReconTarget(struct S *p) { InitXfm(p);
p->a = -1; //P
p->d = -1; //P
p->c = -1; //P
p->b = -2; //P
}
