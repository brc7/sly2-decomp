// (Claude agent) matched: count-down search as do { if (--j < 0) break; if (...) break; } while (1); separate counter k in the last loop.
struct L { char pad[0x1C]; float g1C; char pad2[0x2C - 0x20]; struct L *pnext; };
struct E { int x0; int x4; int x8; };
struct K { float t; int x4; int x8; int xC; };
struct P { char pad[0x50]; struct L *pl; char pad1[0x74 - 0x54]; float g74; char pad2[0xA0 - 0x78]; int cA0; struct K *aA4;
    char pad3[0xC0 - 0xA8]; int cbC0; int cC4; char *pC8; int cCC; struct E *aD0; int xD4; struct P *pD8; };
void SetAsegblDominantBl(struct P *p, int i);
void func_001380F0(struct P *p, int a, int b, int c, float *pg);
/** @sly2-annot addr=0x1384C0
 * Purpose: Normalise asega local times by tMax, recompute blend MRSG tables (CalculateBlendAmrsg per mrsgc),
 *   rescale times; mirror times from a linked asegbl (+0xD8)
 * Info: proposed name ReblendAsegbl (T1); TU blend
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest+verdicts_A1+verdicts_followups
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/reference/functions/animation.md#blend-0x137a30-0x138f08
 * @end-sly2-annot */
void func_001384C0(struct P *p)
{
    int unused[4];
    int i;
    struct L *pl;
    {
        int j;
        j = p->cC4;
        do {
            if (--j < 0)
                break;
            if (0.9999f < *(float *)(p->pC8 + j * p->cbC0))
                break;
        } while (1);
        SetAsegblDominantBl(p, j);
    }
    for (pl = p->pl; pl; pl = pl->pnext)
        pl->g1C = pl->g1C / p->g74;
    for (i = 0; i < p->cCC; i++) {
        struct E *pe = &p->aD0[i];
        func_001380F0(p, pe->x0, pe->x4, pe->x8, &p->g74);
    }
    if (p->cCC == 0)
        func_001380F0(p, 0, 0, 0, &p->g74);
    for (pl = p->pl; pl; pl = pl->pnext)
        pl->g1C = pl->g1C * p->g74;
    if (p->pD8) {
        float r = p->g74 / p->pD8->g74;
        int k;
        for (k = 0; k < p->cA0; k++)
            p->aA4[k].t = r * p->pD8->aA4[k].t;
    }
}
