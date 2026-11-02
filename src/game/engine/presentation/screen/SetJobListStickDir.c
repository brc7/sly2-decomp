extern struct CLOCK { float a[16]; } D_002DDED0;
typedef struct {
    char pad[0x28];
    int state;
    float t;
    char pad30[0x48 - 0x30];
    int c;
    char pad4c[0x118 - 0x4C];
    int i;
} OBJ;
void SetJobListPage(OBJ *, int);
void SelectJobListItem(OBJ *, int);
void *PsndStartSound(int);
void SetSndSourceLo(void *, int);
/** @sly2-annot addr=0x1DD558
 * Purpose: Job list slot 14: 0 -> tasks page, 4 -> jobs page, up/down rows
 * Info: proposed name SetJobListStickDir (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SetJobListStickDir(OBJ *p, unsigned int n)
{
    if (n == p->state)
        return;
    p->state = n;
    p->t = D_002DDED0.a[15];
    switch (n) {
    case 0:
        if (p->c)
            SetJobListPage(p, 1);
        break;
    case 4:
        SetJobListPage(p, 0);
        break;
    case 1:
    case 2:
    case 3:
        if (p->i > 0) {
            SelectJobListItem(p, p->i - 1);
            SetSndSourceLo(PsndStartSound(0x35), 0);
        }
        break;
    case 5:
    case 6:
    case 7:
        if (p->i < p->c - 1) {
            SelectJobListItem(p, p->i + 1);
            SetSndSourceLo(PsndStartSound(0x35), 0);
        }
        break;
    }
}
