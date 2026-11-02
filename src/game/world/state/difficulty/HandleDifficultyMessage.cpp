struct CRef {
    int m_tagk;
    int m_tag;
    CRef() { m_tagk = -1; }
    CRef(float g) { m_tagk = 0x16; *(float *)&m_tag = g; }
    CRef(const CRef &);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &);
    void Decref();
};
struct V;
struct G;
struct H;
struct S { int pad[2]; V *p8; V *pC; V *p10; float *p14; };
struct M { char pad[0x54]; int x54; char pad2[0x10]; int x68; char pad3[0x10]; void *x7C; char pad4[0x28]; float *xA8; };
extern "C" {
extern G D_003D4A60;
extern H D_003E0EA8;
extern void *D_003E0F9C;
extern float D_002BDC00[];
extern char D_002A5060[], D_002A5070[], D_002A5080[], D_002A5090[];
V *PkeyNewSaveKey(void *p, ...);
void ChangeDifficulty(S *p, float g);
}
CRef RefGetSaveVar(H *, V *, G *);
CRef func_00258200(V *, const CRef &, G *);
static inline CRef RefG(float g) { CRef r; r.m_tagk = 0x16; *(float *)&r.m_tag = g; return r; }
static inline void Set(V *k, CRef r) { func_00258200(k, r, &D_003D4A60); }
static inline void SetG(V *k, float g) { CRef r(g); Set(k, r); }
static inline CRef Get(V *k) { G *pg = &D_003D4A60; return RefGetSaveVar(&D_003E0EA8, k, pg); }

/** @sly2-annot addr=0x154A90
 * Purpose: Game-event handler of the difficulty system: event 3 binds three saved sample slots, 34 shifts the
 *   samples (rolling window of 3), 37/16/29 apply difficulty deltas from table D_002BDC00
 * Info: proposed name HandleDifficultyMessage (T1); TU difficulty
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#difficulty-0x154a60-0x1553f0,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void HandleDifficultyMessage(S *p, int msgk, M *pm)
{
    switch (msgk) {
    case 3:
        p->p14 = D_002BDC00;
        p->p8 = PkeyNewSaveKey(D_002A5060, D_002A5070, 0);
        p->pC = PkeyNewSaveKey(D_002A5060, D_002A5080, 0);
        p->p10 = PkeyNewSaveKey(D_002A5060, D_002A5090, 0);
        break;
    case 34:
        Set(p->p8, Get(p->pC));
        Set(p->pC, Get(p->p10));
        SetG(p->p10, *pm->xA8);
        break;
    case 37:
        if (D_003E0F9C == pm->x7C && pm->x54 == 2 && pm->x68 == 1)
            ChangeDifficulty(p, p->p14[3]);
        break;
    case 16:
    case 29:
        ChangeDifficulty(p, p->p14[2]);
        break;
    }
}
