struct CRef {
    int m_tagk;
    int m_tag;
    CRef() { m_tagk = -1; }
    CRef(const CRef &);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &);
    void Decref();
};
struct ENT { int n; int a; };
struct OBJ {
    char pad[0x40];
    int x40;
    int pad44;
    unsigned int c;
    ENT ae[0x40];
    int i;
};
extern char D_002A79A0[];
extern char D_002A79D0[];
extern char D_002A79D8[];
extern "C" int sprintf(char *, const char *, ...);
extern "C" CRef RefGetSaveVarV(const char *, ...);
extern "C" int func_001DAE58(int);
extern "C" int FPowerupAcquired(int);
extern "C" void SelectThiefnetItem(OBJ *, int);
/** @sly2-annot addr=0x1DBD10
 * Purpose: Rebuild the list: tab 0 power-ups 7..0x2A unlocked by episode and not yet acquired; tab 1 loot
 *   kinds with count > 0
 * Info: proposed name RefreshThiefnetItems (T1); TU screen
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/engine/hud-ui.md#2-screens-and-menus-screencpp-0x1d93d8-menu-0x1ae928,
 *   docs/engine/minigames.md#8-safehouse-and-thiefnet-ccode,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void RefreshThiefnetItems(OBJ *p)
{
    {
        int j;
        for (j = 0; j < (int)p->c; j++)
            p->ae[j].n = -1;
    }
    p->c = 0;
    switch (p->x40) {
    case 0: {
        int i;
        for (i = 7; i < 0x2B; i++) {
            if (!func_001DAE58(i))
                continue;
            if (FPowerupAcquired(i))
                continue;
            if (p->c > 0x40)
                break;
            p->ae[p->c++].n = i;
        }
        break;
    }
    case 1: {
        int i;
        for (i = 0; i < 0x40; i++) {
            CRef ref;
            char ach[0x20];
            sprintf(ach, D_002A79A0, i);
            ref = RefGetSaveVarV(D_002A79D0, D_002A79D8, ach, 0);
            if (ref.m_tag == 0)
                continue;
            if (p->c > 0x40)
                break;
            p->ae[p->c++].n = i;
        }
        break;
    }
    }
    SelectThiefnetItem(p, p->i);
}
