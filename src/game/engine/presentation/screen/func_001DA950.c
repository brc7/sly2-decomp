extern struct CLOCK { float a[16]; } D_002DDED0;
extern float D_002A7984, D_002C3254;
struct S { char p0[0x30]; int n30; float g34; int n38; };
void *PsndStartSound(int n);
void SetSndSourceLo(void *p, int n);
/** @sly2-annot addr=0x1DA950
 * Purpose: Set mode n (+0x30) and arg (+0x38); mode 0 plays sound 0x29 and sets the timeout
 * Info: proposed name SetScreenMode (T2); TU screen
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DA950(struct S *p, int n, int m) {
    p->n38 = m;
    p->n30 = n;
    switch (n) {
    case -1:
        p->g34 = -10.0f;
        break;
    case 0:
        p->g34 = D_002A7984;
        SetSndSourceLo(PsndStartSound(0x29), 0);
        break;
    case 1:
    default:
        p->g34 = D_002DDED0.a[15] + D_002C3254;
        SetSndSourceLo(PsndStartSound(0x36), 0);
        break;
    }
}
