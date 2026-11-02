struct X { short a; signed char b; char c; unsigned short h; };
struct T { struct X *px; int n; };
extern int D_002C2D90[];
void FFindSaveVar(void *a, void *b, struct T *pt);
/** @sly2-annot addr=0x1D7E80
 * Purpose: Address of a save var in a GS blob: pb + ib + index*size[kind]
 * Info: proposed name PvFromSaveVar (T1); TU save
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
char *PvFromSaveVar(void *a, void *b, char *pb) {
    struct T t;
    FFindSaveVar(a, b, &t);
    return pb + (t.px->h + t.n * D_002C2D90[t.px->b]);
}
