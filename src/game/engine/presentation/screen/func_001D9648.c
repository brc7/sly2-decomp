extern struct CLOCK { float t; float dt; float g; } D_002DDED0;
typedef struct { char pad[0x100C]; int x100C; } PAD;
extern int D_002DFBC0;
extern int D_002DFBC4;
extern PAD D_002DFC00;
extern PAD D_002E0D00;
extern char D_003D9ED0[], D_003DA150[], D_003DA0F0[], D_002DDEC0[], D_00443740[], D_003D9E80[];
extern char D_003DA0C0[], D_003DA3C0[], D_002E6550[], D_002E65A0[], D_003DA4F0[], D_003DA530[];
void RemoveGrfusr__Fi(int);
int GrfHandleJoyBtn(PAD *, int);
void func_00187E08(PAD *, int);
int func_001B0290(int);
int FIsDlEmpty__FP2DL(void *);
void UpdatePauseMenu(void *);
void UpdateThiefnet(void *);
void UpdateGadgetMenu(void *);
void UpdateCodes(void *);
void UpdateWipe(void *);
void UpdateHudMeter(void *);
void UpdateNotice(void *);
void UpdateJobList(void *);
void CMemcardDialog_Update(void *);
void func_001ADEE8(void *);
void func_001DA808(void *);
void UpdateTitleScreen(void *);
/** @sly2-annot addr=0x1D9648
 * Purpose: Per-frame: handle the pause/menu buttons (joy D_002DFBC4 flags) and run Update (slot 7) of the HUD
 *   screens
 * Info: proposed name UpdateScreens (T2); TU screen
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/hud-ui.md#2-screens-and-menus-screencpp-0x1d93d8-menu-0x1ae928,
 *   docs/engine/hud-ui.md#5-key-functions, docs/engine/overview.md#2-the-frame-loop-exact-order
 * @end-sly2-annot */
void func_001D9648(void *pdl)
{
    PAD *pad;
    if (D_002DFBC4 & 8) {
        if (D_002DDED0.g == 0.0f || 0.1f < D_002DDED0.t / D_002DDED0.g)
            RemoveGrfusr__Fi(8);
    }
    if (D_002DFBC0 & 2)
        pad = &D_002DFC00;
    else
        pad = &D_002E0D00;
    if (GrfHandleJoyBtn(pad, 0x800)) {
        if (!func_001B0290(0))
            func_00187E08(pad, 0x800);
    } else if (D_002DFBC0 & 2) {
        int r = FIsDlEmpty__FP2DL(pdl);
        int f = 0;
        if (pad->x100C == 1 && 5.0f <= D_002DDED0.t)
            f = 1;
        if (r && f)
            func_001B0290(0);
    }
    UpdatePauseMenu(D_003D9ED0);
    UpdateThiefnet(D_003DA150);
    UpdateGadgetMenu(D_003DA0F0);
    UpdateCodes(D_002DDEC0);
    UpdateWipe(D_00443740);
    UpdateHudMeter(D_003D9E80);
    UpdateNotice(D_003DA0C0);
    UpdateJobList(D_003DA3C0);
    CMemcardDialog_Update(D_002E6550);
    func_001ADEE8(D_002E65A0);
    func_001DA808(D_003DA4F0);
    UpdateTitleScreen(D_003DA530);
}
