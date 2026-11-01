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
void func_001DC260(OBJ *, int);
int func_00161798(void *);
void func_001617E0(void *, int);
void func_001DA950(OBJ *, int, int);
void func_001DAF38(int, int);
void func_001DBD10(OBJ *);
void *func_001FBB90(int);
void func_001FB2F8(void *, int);
void func_0019F0A8(void *);
void func_0021ECD8(char *, const char *, ...);
CRef func_001D8A38(const char *, ...);
CRef func_001D8B20(const CRef &, const char *, ...);
void func_001DA858(OBJ *);
void func_001DBEF0(OBJ *);
}
extern "C" void func_001DB870(OBJ *p)
{
    func_001DA808(p);
    if (p->x40 == -1)
        return;
    if (!p->x10)
        return;
    if (p->x30 && GrfHandleJoyBtn(p->x10, 0x10)) {
        func_001DC260(p, -1);
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
                    func_001DAF38(p->x254, 1);
                    func_001DBD10(p);
                    func_001FB2F8(func_001FBB90(0x29), 0);
                    func_0019F0A8(D_003DD490);
                } else if (have >= cost) {
                    func_001DA950(p, 0, 0x34);
                } else {
                    func_001DA950(p, 1, 0x35);
                }
            } else {
                func_001FB2F8(func_001FBB90(0x36), 0);
            }
        }
        break;
    case 1:
        if (GrfHandleJoyBtn(p->x10, 0x40)) {
            if (p->x258 == -1) {
                func_001FB2F8(func_001FBB90(0x36), 0);
            } else {
                CRef r;
                char buf[32];
                func_0021ECD8(buf, D_002A79A0, p->x258);
                r = func_001D8A38(D_002A79D0, D_002A79D8, buf, 0);
                if (r.m_tag > 0) {
                    func_001D8B20(CRef(r.m_tag - 1), D_002A79D0, D_002A79D8, buf, 0);
                    func_001617E0(D_002DEB30, func_00161798(D_002DEB30) + D_002C3288[p->x258].x0);
                    func_001DBD10(p);
                    func_001FB2F8(func_001FBB90(0x29), 0);
                }
            }
        }
        break;
    }
    if (!p->x30 && GrfHandleJoyBtn(p->x10, 0x10))
        func_001DA950(p, -1, -1);
    if (GrfHandleJoyBtn(p->x10, 0x80)) {
        func_001DA950(p, -1, -1);
        func_001DC260(p, p->x40 == 0);
    }
    if (p->x30)
        func_001DA858(p);
    func_001DBEF0(p);
}
