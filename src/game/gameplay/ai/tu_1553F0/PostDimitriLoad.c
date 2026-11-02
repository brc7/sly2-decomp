struct E { int oidA; void *pA; int oidB; void *pB; int oidC; void *pC; int oidD; void *pD; int n; };
struct S { char pad[0x1010]; int c; struct E *ae; };
extern char D_003DD490[];
void *PloFindSwObject__FP2SWi3OIDP2LO(void *g, int k, int oid, struct S *p);
void PostStepguardLoad(struct S *p);
/** @sly2-annot addr=0x155450
 * Purpose: STEPGUARD PostLoad then resolves the 4 oids of each route record
 * Info: proposed name PostDimitriLoad (T1); class DIMITRI slot15; TU tu_1553F0
 * Notes: names_v2 C proposed UpdateAloHierarchy__FP3ALOf (0.438) | Cid3B->Cid59 (CID 0x3B=59, hex-literal) |
 *   class identity from whole_game WE names/object strings: PostCid59Load -> PostDimitriLoad
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources:
 *   r3b_rest+verdicts_tu_r3r5+whole_game(class identity)
 * See: docs/engine/bosses.md#31-dimitri-m1_boss-l05-nightclub-heist-level,
 *   docs/reference/functions/guards-ai.md#tu_1553f0-0x1553f0-0x1564e8
 * @end-sly2-annot */
void PostDimitriLoad(struct S *p)
{
    int i;
    PostStepguardLoad(p);
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        pe->pA = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, pe->oidA, p);
        pe->pB = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, pe->oidB, p);
        pe->pC = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, pe->oidC, p);
        pe->pD = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, pe->oidD, p);
    }
}
