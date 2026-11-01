// func_0020E3C0: ported from Lilly's personal Sly 2 decomp (src/P2/sw.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002DEB30 (g_game), D_002E1E54 (g_nLang), D_003D4A60 (g_savedata), D_002A39E8 (g_apchzWorld)
// 0x0020E3C0 ResolveSwSaveVars__FP2SW  (T1037)
// Once the level file has named its save variables (LoadSaveVarsFromBrx), looks up the paths the game itself reads and writes by name (SW 0x3B1C..:
// the language and sound settings in "flags", the job and world of this level, each world's "dt" and "percent_complete") and stores the current
// language, job and world; "episodeBest" is raised to the current world when that is a new best. Then tells the game (event 3).
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/sw.h"
struct SLOT
{
      SLOT *pslotNext;
};
extern GAME D_002DEB30;
struct GRABSPOTDATA
{
      TN *ptn;
      undefined4 unk_0x04;
      float sUnk08;
      undefined4 unk_0x0c;
      int nPriority;
      int nUnk14;
      int nGroup;
      uint grfUnk1c;
};
struct GRABSPOT : public GRABSPOTDATA
{
};
struct CELL
{
      qword plane;
      CELL *pcellNeg;
      CELL *pcellPos;
      int ileafNeg;
      int ileafPos;
};
struct LANGSTR
{
      int id;
      char *pchz;
};
struct SVN
{
      int nHash;
      char *pchzName;
      int id;
      SVN *psvnNext;
};
struct SVPATH
{
      int cid;
      short *aid;
};
struct SAVEDATA
{
      byte ab[0x5400];
      uint nVersion;
      int nSum;
};
struct LOUPD
{
      int grf;
      LO *plo;
};
int GetCurrentJob();
void OnGameEvent(GAME *pgame, int nEvent, LO *plo);
union TAG
{
    int m_n;
    float m_g;
    int m_bool;
    uint m_symid;
    BIFK m_bifk;
    const VECTOR4* m_pvec4;
    CPair* m_ppair;
    CProc* m_pproc;
    VECTOR* m_pvector;
    MATRIX4* m_pmatrix;
    CLQ* m_pclq;
    LM* m_plm;
    SMP* m_psmp;
    BASIC* m_pbasic;
    CMethod* m_pmethod;
};
class CRef
{
public:
    TAGK m_tagk;
    TAG m_tag;
    CRef() { m_tagk = TAGK_Nil; }
    CRef(TAGK tagk) { SetTag(tagk); }
    CRef(TAGK tagk, int n) { m_tagk = tagk; m_tag.m_n = n; }
    CRef(TAGK tagk, float g) { m_tagk = tagk; m_tag.m_g = g; }
    CRef(const CRef &ref);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &ref);
    bool operator==(const CRef &ref);
    void Decref();
    void SetTag(TAGK tagk);
};
extern int D_002E1E54;
extern SAVEDATA D_003D4A60;
extern const char *D_002A39E8[9];
SVPATH *PsvpMake(const char *pchz, ...);
CRef SetSaveVar(SVPATH *psvp, const CRef &ref, SAVEDATA *psave);
inline void SetSaveVarRef(SVPATH *psvp, CRef ref) { SetSaveVar(psvp, ref, &D_003D4A60); }
inline void SetSaveVarInt(SVPATH *psvp, int n)
{
    CRef ref((TAGK)0x15, n);
    SetSaveVarRef(psvp, ref);
}
CRef SetSaveVarByName(const CRef &ref, const char *pchz, ...);
extern "C" int func_00210AE0();
int NGetEpisodeBest();
void ResolveSwSaveVars(SW *psw) __asm__("func_0020E3C0");
void ResolveSwSaveVars(SW *psw)
{
    psw->psvpDt = PsvpMake("dt", 0);
    psw->psvpWidCurrent = PsvpMake("widCurrent", 0);
    psw->psvpEpisodeCurrent = PsvpMake("episodeCurrent", 0);
    psw->psvpCcoin = PsvpMake("loot", "ccoin", 0);
    psw->psvpLang = PsvpMake("flags", "lang", 0);
    psw->psvpMixerMode = PsvpMake("flags", "nMixerMode", 0);
    psw->psvpMusicVolume = PsvpMake("flags", "uMusicVolume", 0);
    psw->psvpEffectsVolume = PsvpMake("flags", "uEffectsVolume", 0);
    psw->psvpUseHeadset = PsvpMake("flags", "fUseHeadset", 0);
    psw->psvpScreenCenterX = PsvpMake("flags", "dxScreenCenter", 0);
    psw->psvpScreenCenterY = PsvpMake("flags", "dyScreenCenter", 0);
    psw->psvpNoVibration = PsvpMake("flags", "fNoVibration", 0);
    psw->psvpReverseLookAround = PsvpMake("flags", "fReverseLookAround", 0);
    psw->psvpReverseAim = PsvpMake("flags", "fReverseAim", 0);
    psw->psvpAutosaveWarned = PsvpMake("flags", "fAutosaveWarned", 0);
    for (int iworld = 0; iworld < 9; iworld++)
    {
        psw->apsvpEpisodeDt[iworld] = PsvpMake("episode", D_002A39E8[iworld], "dt", 0);
        psw->apsvpEpisodePercent[iworld] = PsvpMake("episode", D_002A39E8[iworld], "percent_complete", 0);
    }
    SetSaveVarInt(psw->psvpLang, D_002E1E54);
    SetSaveVarInt(psw->psvpWidCurrent, GetCurrentJob());
    SetSaveVarInt(psw->psvpEpisodeCurrent, func_00210AE0());
    if (func_00210AE0() > NGetEpisodeBest())
        SetSaveVarByName(CRef((TAGK)0x15, func_00210AE0()), "episodeBest", 0);
    OnGameEvent(&D_002DEB30, 3, 0);
}
