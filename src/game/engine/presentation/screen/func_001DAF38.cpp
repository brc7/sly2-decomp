struct CRef {
    int m_tagk;
    int m_tag;
    CRef() { m_tagk = -1; }
    CRef(bool f) { m_tagk = 0x17; m_tag = f; }
    CRef(const CRef &);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &);
    void Decref();
};
struct G { char pad[0xC]; int n; };
extern G D_002DEB30;
extern char D_002A79A0[];
extern char D_002A79A8[];
extern char D_002A79B8[];
int func_0021ECD8(char *, const char *, ...);
CRef func_001D8B20(const CRef &, const char *, ...);
void BroadcastGameEvent(G *, int, int);
/** @sly2-annot addr=0x1DAF38
 * Purpose: Set powerUps.mppukfAcquired[n]; when set broadcast game event 0x3B with n
 * Info: proposed name SetPowerupAcquired (T1); TU screen
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/engine/hud-ui.md#5-key-functions, docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void func_001DAF38(int n, int f)
{
    CRef ref;
    char buf[0x20];
    func_0021ECD8(buf, D_002A79A0, n);
    ref = func_001D8B20(CRef(f != 0), D_002A79A8, D_002A79B8, buf, 0);
    if (f) {
        D_002DEB30.n = n;
        BroadcastGameEvent(&D_002DEB30, 0x3B, 0);
    }
}
