typedef struct { char pad[0x58]; int x58; } W;
typedef struct { void *p; int a; int b; } ENT;
typedef struct {
    char pad[0x24];
    W *pw;
    char pad28[0x40 - 0x28];
    int kind;
    char pad44[4];
    int c;
    ENT ae[17];
    int sel;
} OBJ;
void SwitchWidgetAnim(W *, int, int *);
int FJobComplete(void *);
int FTaskComplete(void *);
/** @sly2-annot addr=0x1DD1F8
 * Purpose: Row highlight anims (0x4FD+i selected / 0x4E9+i) and completed checkmarks (0x507+i) via
 *   FJobComplete/FTaskComplete
 * Info: proposed name UpdateJobListWidgets (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void UpdateJobListWidgets(OBJ *p)
{
    int i;
    if (p->pw->x58 != 2)
        return;
    for (i = 0; i < p->c; i++) {
        int f;
        ENT *pe = &p->ae[i];
        SwitchWidgetAnim(p->pw, i == p->sel ? i + 0x4FD : i + 0x4E9, &pe->a);
        f = 0;
        switch (p->kind) {
        case 0:
            f = FJobComplete(pe->p);
            break;
        case 1:
            f = FTaskComplete(pe->p);
            break;
        }
        SwitchWidgetAnim(p->pw, f ? i + 0x507 : -1, &pe->b);
    }
}
