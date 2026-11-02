typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x14]; short s14; short pad2; int n18; };
struct H { struct O *po; int pad; void *p8; void *pC; qword q10; qword q20; qword q30; };
struct N { struct H h[2]; char pad[0xAC - 0x80]; struct N *next; };
struct E { char pad[0x6C]; struct N *pn; };
struct P { char pad[0xEC]; struct E *pe; };
struct M { void *p0; void *p4; void *p8; int nC; int n10; int pad[3]; qword q20; qword q30; char pad2[0x60 - 0x40]; };
struct G;
extern struct G D_003DD490;
void *func_001F7530(void *, int);
void func_002333B4(void *, int, int);
int func_001F7598(struct M *);
void FreeSwXpList__FP2SWP2XP(struct G *, void *);
/** @sly2-annot addr=0x1F7410
 * Purpose: SO vt74: for each contact, if other part has zap kinds, build a zap record and InflictZap; free
 *   contact list
 * Info: proposed name InflictSoContactZaps (T2); class SO slot74; TU so
 * Notes: names_v2 v1 proposed UpdateGeomWorld__FP4GEOMT0G9VU_VECTORP7MATRIX3 (0.200)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7410(struct P *p)
{
    struct N *pn;
    for (pn = p->pe->pn; pn; pn = pn->next) {
        int i = pn->h[0].p8 != p;
        struct H *ph = (struct H *)((char *)pn + (i << 6));
        if (ph->po->n18) {
            struct H *pot = (struct H *)((char *)pn + ((1 - i) << 6));
            void *r = func_001F7530(pot->pC, ph->po->n18);
            if (r) {
                struct M m;
                func_002333B4(&m, 0, 0x60);
                m.p0 = p;
                m.p4 = ph->po;
                m.p8 = r;
                m.nC = ph->po->n18;
                m.n10 = ph->po->s14;
                m.q20 = pot->q10;
                m.q30 = ph->q30;
                func_001F7598(&m);
                if (!p->pe) break;
            }
        }
    }
    if (p->pe) {
        FreeSwXpList__FP2SWP2XP(&D_003DD490, p->pe->pn);
        p->pe->pn = 0;
    }
}
