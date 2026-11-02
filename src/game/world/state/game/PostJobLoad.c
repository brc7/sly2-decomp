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
void RefGetGlobalSaveVar(CRef *pref, void *a, void *b);
void func_00129EC8(CRef *pref);
void *PvFromSaveVar(void *p, void *q, void *r);
void *PloFindSwObject__FP2SWi3OIDP2LO(void *pg, int kind, int oid, void *owner);
/** @sly2-annot addr=0x162780
 * Purpose: JOB PostLoad: if not current and live apply 'inactive' (state 5) visibility/RSMGs; restore start
 *   count and elapsed time from save vars; resolve a save var; find its completion widget (+0xAC) and the
 *   shared widget oid 0x379
 * Info: proposed name PostJobLoad (T1); class JOB slot15; TU game
 * Notes: superseded r3_1539A8 proposal: PostCid84Load (0.65)
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
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
        RefGetGlobalSaveVar(&ref, pn, D_003D4A60);
        n = ref.m_tag;
        func_00129EC8(&ref);
        p->f7c = n;
    }
    pn = &p->fe0;
    if (*pn) {
        float g;
        RefGetGlobalSaveVar(&ref, pn, D_003D4A60);
        g = *(float *)&ref.m_tag;
        func_00129EC8(&ref);
        p->fa4 = g;
    }
    p->fa8 = PvFromSaveVar(D_003DD490.m3a18, &p->fe8, D_003D4A60);
    if (p->fac != -1)
        p->fb8 = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 0x405, p->fac, 0);
    p->fbc = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 0x405, 0x379, 0);
}
