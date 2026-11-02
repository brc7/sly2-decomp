struct R { int tagk; int tag; };
struct G1 { char pad[0x4C]; int x4C; };
struct G2 { char pad[0xFD50]; int xFD50; };
struct S { char pad[0x1C]; int f; };
extern char D_002DEB30[], D_003D4A60[];
extern struct G1 D_002E6550;
extern struct G2 D_002E65E0;
extern int D_003E0FE4;
void BroadcastGameEvent(void *p, int a, int b);
void RefGetGlobalSaveVar(struct R *r, int a, void *b);
void Decref__4CRef(struct R *r);
void SetMcDialogState(struct G1 *p, int n);
void func_001ABD40(struct G2 *p);
void func_001660F8(void);
/** @sly2-annot addr=0x1B0878
 * Purpose: Game events 0x2D/0x2E; save via memcard dialog states 0x22/0x1A (StartSaveJobB) or ResetGameSeq
 * Info: proposed name PauseItemSaveQuit (T2); TU menu
 * Notes: names_v2 v1 proposed CloneStepPhys__FP4STEPP2SOi (0.025)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001B0878(struct S *p)
{
    BroadcastGameEvent(D_002DEB30, 0x2D, 0);
    BroadcastGameEvent(D_002DEB30, 0x2E, 0);
    if (D_002E65E0.xFD50) {
        struct R r;
        int v;
        D_002E6550.x4C = 1;
        RefGetGlobalSaveVar(&r, D_003E0FE4, D_003D4A60);
        v = r.tag;
        Decref__4CRef(&r);
        if (v == 0) {
            SetMcDialogState(&D_002E6550, 0x22);
        } else {
            SetMcDialogState(&D_002E6550, 0x1A);
            func_001ABD40(&D_002E65E0);
        }
    } else {
        func_001660F8();
    }
    if (p) p->f |= 2;
}
