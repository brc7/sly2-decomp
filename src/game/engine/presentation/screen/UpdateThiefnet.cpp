struct CRef {
    int m_tagk;
    int m_tag;
    CRef() { m_tagk = -1; }
    CRef(int n) { m_tagk = 0x15; m_tag = n; }
    CRef(const CRef &);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &);
    void Decref();
};
struct REC { int a, b, x8; int pad[5]; };
struct REC12 { int x0, x4, x8; };
struct OBJ {
    char pad0[0x10];
    void *x10;
    char pad14[0x30 - 0x14];
    int x30;
    char pad34[0x40 - 0x34];
    int x40;
    char pad44[0x254 - 0x44];
    int x254;
    int x258;
};
extern "C" {
extern REC D_002BCD00[];
extern REC12 D_002C3288[];
extern char D_002DEB30[];
extern char D_003DD490[];
extern char D_002A79A0[], D_002A79D0[], D_002A79D8[];
void func_001DA808(OBJ *);
int GrfHandleJoyBtn(void *, int);
void SetThiefnetTab(OBJ *, int);
int func_00161798(void *);
void func_001617E0(void *, int);
void func_001DA950(OBJ *, int, int);
void SetPowerupAcquired(int, int);
void RefreshThiefnetItems(OBJ *);
void *PsndStartSound(int);
void SetSndSourceLo(void *, int);
void func_0019F0A8(void *);
void sprintf(char *, const char *, ...);
CRef RefGetSaveVarV(const char *, ...);
CRef RefSetSaveVarV(const CRef &, const char *, ...);
void UpdateScreenStickDir(OBJ *);
void UpdateThiefnetWidgets(OBJ *);
}
/** @sly2-annot addr=0x1DB870
 * Purpose: ThiefNet per-frame (slot 7): tab 0 buy the selected power-up (cost check against coins, confirm,
 *   deduct, SetPowerupAcquired, notify players); tab 1 sell one loot item (loot.mplootkc[i]-1, add its price
 *   to coins); tab switch and close
 * Info: proposed name UpdateThiefnet (T1); TU screen
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/engine/hud-ui.md#2-screens-and-menus-screencpp-0x1d93d8-menu-0x1ae928,
 *   docs/engine/minigames.md#8-safehouse-and-thiefnet-ccode,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void UpdateThiefnet(OBJ *p)
{
    func_001DA808(p);
    if (p->x40 == -1)
        return;
    if (!p->x10)
        return;
    if (p->x30 && GrfHandleJoyBtn(p->x10, 0x10)) {
        SetThiefnetTab(p, -1);
        return;
    }
    switch (p->x40) {
    case 0:
        if (GrfHandleJoyBtn(p->x10, 0x40)) {
            if (p->x254 != -1) {
                int cost = D_002BCD00[p->x254].x8;
                int have = func_00161798(D_002DEB30);
                if (!p->x30) {
                    func_001DA950(p, -1, -1);
                    func_001617E0(D_002DEB30, have - cost);
                    SetPowerupAcquired(p->x254, 1);
                    RefreshThiefnetItems(p);
                    SetSndSourceLo(PsndStartSound(0x29), 0);
                    func_0019F0A8(D_003DD490);
                } else if (have >= cost) {
                    func_001DA950(p, 0, 0x34);
                } else {
                    func_001DA950(p, 1, 0x35);
                }
            } else {
                SetSndSourceLo(PsndStartSound(0x36), 0);
            }
        }
        break;
    case 1:
        if (GrfHandleJoyBtn(p->x10, 0x40)) {
            if (p->x258 == -1) {
                SetSndSourceLo(PsndStartSound(0x36), 0);
            } else {
                CRef r;
                char buf[32];
                sprintf(buf, D_002A79A0, p->x258);
                r = RefGetSaveVarV(D_002A79D0, D_002A79D8, buf, 0);
                if (r.m_tag > 0) {
                    RefSetSaveVarV(CRef(r.m_tag - 1), D_002A79D0, D_002A79D8, buf, 0);
                    func_001617E0(D_002DEB30, func_00161798(D_002DEB30) + D_002C3288[p->x258].x0);
                    RefreshThiefnetItems(p);
                    SetSndSourceLo(PsndStartSound(0x29), 0);
                }
            }
        }
        break;
    }
    if (!p->x30 && GrfHandleJoyBtn(p->x10, 0x10))
        func_001DA950(p, -1, -1);
    if (GrfHandleJoyBtn(p->x10, 0x80)) {
        func_001DA950(p, -1, -1);
        SetThiefnetTab(p, p->x40 == 0);
    }
    if (p->x30)
        UpdateScreenStickDir(p);
    UpdateThiefnetWidgets(p);
}
