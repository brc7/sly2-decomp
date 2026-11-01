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
