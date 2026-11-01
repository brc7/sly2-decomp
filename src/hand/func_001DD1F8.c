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
void func_00213DB0(W *, int, int *);
int func_00163400(void *);
int func_00164030(void *);
/** @sly2-annot addr=0x1DD1F8
 * Purpose: Row highlight anims (0x4FD+i selected / 0x4E9+i) and completed checkmarks (0x507+i) via
 *   FJobComplete/FTaskComplete
 * Info: proposed name UpdateJobListWidgets (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DD1F8(OBJ *p)
{
    int i;
    if (p->pw->x58 != 2)
        return;
    for (i = 0; i < p->c; i++) {
        int f;
        ENT *pe = &p->ae[i];
        func_00213DB0(p->pw, i == p->sel ? i + 0x4FD : i + 0x4E9, &pe->a);
        f = 0;
        switch (p->kind) {
        case 0:
            f = func_00163400(pe->p);
            break;
        case 1:
            f = func_00164030(pe->p);
            break;
        }
        func_00213DB0(p->pw, f ? i + 0x507 : -1, &pe->b);
    }
}
