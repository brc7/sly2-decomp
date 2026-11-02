struct CRef {
    int m_tagk;
    union { int m_tag; float m_g; };
    CRef() { m_tagk = -1; }
    CRef(const CRef &);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &);
    void Decref();
};
struct KEY;
struct GS { char pad[0x3B2C]; KEY *k[7]; };
struct DISP1 { unsigned long dx : 12; unsigned long dy : 11; unsigned long rest : 41; };
struct DISP2 { unsigned int dx : 12; unsigned int dy : 11; unsigned long rest : 41; };
struct GSR { char pad[0x18]; DISP1 d1; char pad20[0x40 - 0x20]; DISP2 d2; };
extern "C" {
extern GS D_003DD490;
extern char D_003DD040[];
extern int D_003DD050;
extern GSR D_002DEB80;
CRef RefGetGlobalSaveVar(KEY *, char *);
int CLanguagesRegion(void);
void SetLanguage(int);
void SetSndmgrPlaybackMode(void *, int);
void SetSndmgrChannelVolume(void *, int, int, float);
unsigned int func_0023E1C8(float);
}
static inline int GetInt(KEY *k, char *pb) { return RefGetGlobalSaveVar(k, pb).m_tag; }
static inline float GetFloat(KEY *k, char *pb) { return RefGetGlobalSaveVar(k, pb).m_g; }
/** @sly2-annot addr=0x1D8FF8
 * Purpose: Apply option vars from GS: vibration(?), sound playback mode, music/sfx volumes (1-g), a flag at
 *   D_003DD050, screen X/Y offset (GS DISPLAY DX=g+636, DY=g+50)
 * Info: proposed name ApplyGsSettings (T1); TU save
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78,
 *   docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
extern "C" void ApplyGsSettings(char *pb)
{
    if (D_003DD490.k[0] && CLanguagesRegion() >= 2)
        SetLanguage(GetInt(D_003DD490.k[0], pb));
    if (D_003DD490.k[1])
        SetSndmgrPlaybackMode(D_003DD040, GetInt(D_003DD490.k[1], pb));
    if (D_003DD490.k[2])
        SetSndmgrChannelVolume(D_003DD040, 1, 1, 1.0f - GetFloat(D_003DD490.k[2], pb));
    if (D_003DD490.k[3]) {
        SetSndmgrChannelVolume(D_003DD040, 0, 1, 1.0f - GetFloat(D_003DD490.k[3], pb));
        SetSndmgrChannelVolume(D_003DD040, 3, 1, 1.0f - GetFloat(D_003DD490.k[3], pb));
    }
    if (D_003DD490.k[4])
        D_003DD050 = GetInt(D_003DD490.k[4], pb);
    if (D_003DD490.k[5]) {
        float f = GetFloat(D_003DD490.k[5], pb);
        GSR *p = &D_002DEB80;
        p->d1.dx = p->d2.dx = func_0023E1C8(f + 636.0f);
    }
    if (D_003DD490.k[6]) {
        float f = GetFloat(D_003DD490.k[6], pb);
        GSR *p = &D_002DEB80;
        p->d1.dy = p->d2.dy = func_0023E1C8(f + 50.0f) & ~1;
    }
}
