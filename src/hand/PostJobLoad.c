typedef struct CRef { int m_tagk; int m_tag; } CRef;
struct S {
    char pad[0x7C]; int f7c;
    char pad1[0x8C - 0x80]; int f8c, f90, f94, f98;
    char pad2[0xA4 - 0x9C]; float fa4; void *fa8; int fac;
    char pad3[0xB8 - 0xB0]; void *fb8; void *fbc;
    char pad4[0xD0 - 0xC0]; int fd0;
    char pad5[0xE0 - 0xD4]; int fe0;
    char pad6[0xE8 - 0xE4]; int fe8;
};
extern char D_003D4A60[];
struct G { char pad[0x3A18]; char m3a18[0x10]; };
extern struct G D_003DD490;
void PostLoLoad(struct S *p);
int FIsCurrentJob(struct S *p);
int FJobLive(struct S *p);
void ApplyStateLoVisibility(struct S *p, int n, int c, int a);
void TriggerStateRsmgs(struct S *p, int n, int c, int a, int b);
void func_002581C0(CRef *pref, void *a, void *b);
void func_00129EC8(CRef *pref);
void *func_001D7E80(void *p, void *q, void *r);
void *PloFindSwObject__FP2SWi3OIDP2LO(void *pg, int kind, int oid, void *owner);
void PostJobLoad(struct S *p)
{
    CRef ref;
    int *pn;
    PostLoLoad(p);
    if (!FIsCurrentJob(p) && FJobLive(p)) {
        ApplyStateLoVisibility(p, 5, p->f8c, p->f90);
        TriggerStateRsmgs(p, 5, p->f94, p->f98, 1);
    }
    pn = &p->fd0;
    if (*pn) {
        int n;
        func_002581C0(&ref, pn, D_003D4A60);
        n = ref.m_tag;
        func_00129EC8(&ref);
        p->f7c = n;
    }
    pn = &p->fe0;
    if (*pn) {
        float g;
        func_002581C0(&ref, pn, D_003D4A60);
        g = *(float *)&ref.m_tag;
        func_00129EC8(&ref);
        p->fa4 = g;
    }
    p->fa8 = func_001D7E80(D_003DD490.m3a18, &p->fe8, D_003D4A60);
    if (p->fac != -1)
        p->fb8 = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 0x405, p->fac, 0);
    p->fbc = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 0x405, 0x379, 0);
}
