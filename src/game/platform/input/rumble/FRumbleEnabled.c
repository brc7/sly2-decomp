typedef struct CRef { int tagk; int tag; } CRef;
extern int D_002C2D8C;
extern void *D_003E0FD8;
extern char D_003D4A60[];
void RefGetGlobalSaveVar(CRef *pref, void *a, char *b);
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x1D55B8
 * Purpose: Mode 2 -> 1; 0/3 -> 0; else !save var D_003E0FD8
 * Info: proposed name FRumbleEnabled (T1); TU rumble
 * Notes: names_v2 C proposed RumbleUnknown4__FP2GS (0.638)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#rumble-0x1d5150-0x1d57a8
 * @end-sly2-annot */
int FRumbleEnabled(void) {
    switch (D_002C2D8C) {
    case 2:
        return 1;
    case 0:
    case 3:
        return 0;
    case 1:
    default: {
        CRef ref;
        int n;
        RefGetGlobalSaveVar(&ref, D_003E0FD8, D_003D4A60);
        n = ref.tag;
        Decref__4CRef(&ref);
        return n ^ 1;
    }
    }
}
