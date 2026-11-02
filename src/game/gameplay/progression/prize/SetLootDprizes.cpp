struct VTE { short d; short pad; void (*pf)(); };
extern "C" {
void Decref__4CRef(void *p);
void func_00129658(void *p, const void *q);
float GRandInRange__Fff(float a, float b);
void sprintf(char *buf, const char *fmt, ...);
void SetDprizeDprizes(void *p, int n);
}
struct CRef {
    int m_tagk; int m_tag;
    CRef() { m_tagk = -1; }
    CRef(int k, int n) { m_tagk = k; m_tag = n; }
    ~CRef() { Decref__4CRef(this); }
    CRef &operator=(const CRef &r) { func_00129658(this, &r); return *this; }
};
struct ALS { int x0; int x4; };
struct LO { char pad[0xC]; VTE *vt; };
struct S {
    char pad[0xC]; VTE *vt;
    char p0[0x150 - 0x10]; int x150;
    char p1[0x210 - 0x154]; LO *x210;
    char p2[0x230 - 0x214]; float x230; float x234; float x238;
    char p3[0x244 - 0x23C]; ALS *x244;
};
struct M { int n; S *p; int pad[14]; };
extern char D_002A7768[];
extern char D_002A7770[];
extern char D_002A7778[];
CRef RefGetSaveVarV(char *a, char *b, char *c, int n);
CRef RefSetSaveVarV(const CRef &r, char *a, char *b, char *c, int n);
extern "C" void SetLootDprizes(S *p, int n);
/** @sly2-annot addr=0x1C77E8
 * Purpose: CID 94 setter: on collect format id and set save-game key 'mplootkc' (CRef tag 0x15)
 * Info: proposed name SetLootDprizes (T1); class LOOT slot76; TU prize
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void SetLootDprizes(S *p, int n)
{
    if (p->x150 == n) return;
    switch (n) {
    case 0:
        p->x238 = GRandInRange__Fff(p->x230, p->x234);
        break;
    case 1: {
        CRef ref;
        char buf[0x10];
        sprintf(buf, D_002A7768, p->x244->x4);
        ref = RefGetSaveVarV(D_002A7770, D_002A7778, buf, 0);
        RefSetSaveVarV(CRef(0x15, ref.m_tag + 1), D_002A7770, D_002A7778, buf, 0);
        LO *plo = p->x210;
        if (plo) {
            M m;
            m.n = n;
            m.p = p;
            ((void (*)(void *, M *))plo->vt[51].pf)((char *)plo + plo->vt[51].d, &m);
        }
        n = 2;
        ((void (*)(void *, int, int, int, int, int))p->vt[3].pf)((char *)p + p->vt[3].d, 5, 0, 0, 0, 0);
        break;
    }
    }
    SetDprizeDprizes(p, n);
}
