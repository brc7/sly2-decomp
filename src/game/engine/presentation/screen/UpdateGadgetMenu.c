typedef struct { char pad[0x1C]; int fl; } R;
typedef struct {
    char pad[0x10];
    void *x10;
    char pad14[0x24 - 0x14];
    void *x24;
    char pad28[0x30 - 0x28];
    int x30;
    char pad34[0x40 - 0x34];
    int x40;
    char pad44[4];
    int x48;
} OBJ;
extern struct G D_003DD490;
extern int D_002C3258[];
void func_001DA808(OBJ *);
int GrfHandleJoyBtn(void *, int);
void *PpoHero(struct G *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void SetGadgetMenuState(OBJ *, int);
void UpdateScreenStickDir(OBJ *);
R *func_001DAE20(OBJ *, int);
void func_001DA950(OBJ *, int, int);
void *PsndStartSound(int);
void SetSndSourceLo(void *, int);
void SetStepplayerGadget(void *, int, int);
/** @sly2-annot addr=0x1DAA90
 * Purpose: Gadget menu per-frame (slot 7): circle closes, info button shows a message, the 3 slot buttons
 *   (D_002C3258 = 4,1,2) assign the selected power-up to that slot (SetStepplayerGadget) when owned, else
 *   error message 0x32/0x33
 * Info: proposed name UpdateGadgetMenu (T1); TU screen
 * Notes: names_v2 C hint not used: CalculateTnCrv__FP2TNP6VECTORN21
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void UpdateGadgetMenu(OBJ *p)
{
    void *pv;
    R *r;
    int i;
    func_001DA808(p);
    if (p->x24 == 0)
        return;
    if (p->x40 == -1)
        return;
    if (p->x10 == 0)
        return;
    if (p->x30 && GrfHandleJoyBtn(p->x10, 0x10)) {
        SetGadgetMenuState(p, -1);
        return;
    }
    pv = PpoHero(&D_003DD490);
    if (pv == 0 || !FIsBasicDerivedFrom__FP5BASIC3CID(pv, 0x34)) {
        SetGadgetMenuState(p, -1);
        return;
    }
    UpdateScreenStickDir(p);
    if (GrfHandleJoyBtn(p->x10, 0x7E8)) {
        r = func_001DAE20(p, p->x48);
        if (r == 0 || !(r->fl & 1))
            func_001DA950(p, 1, 0xE2);
        else
            func_001DA950(p, 1, 0x31);
    }
    for (i = 0; i < 3; i++) {
        if (!GrfHandleJoyBtn(p->x10, D_002C3258[i]))
            continue;
        r = func_001DAE20(p, p->x48);
        if (r == 0 || !(r->fl & 4)) {
            func_001DA950(p, 1, 0x32);
        } else if (!(r->fl & 1)) {
            func_001DA950(p, 1, 0x33);
        } else {
            SetSndSourceLo(PsndStartSound(0x34), 0);
            SetStepplayerGadget(pv, i, p->x48);
        }
    }
}
