struct O { char pad[0x150]; void *p150; };
struct E { struct O *po; int n4; int n8; float gC; int pad10; int n14; int n18; int n1C; int n20; };
struct Q { float g0; int pad4[7]; int n20; int pad24[3]; };
struct S {
    char pad0[0x58]; float g58; char pad5C[0x60 - 0x5C]; int n60; int pad64[3]; struct Q a70[6];
    char pad190[0x1C0 - 0x190]; int c1C0; struct E a1C4[3];
    int n230; char pad234[0x298 - 0x234]; float g298; float g29C; float g2A0;
};
extern char D_003DD040[];
extern char D_002DEB30[];
int GrfPressedWithin__5POCTLUif(void *, int, float);
int POCTL_FConsumePressed(void *, int);
float func_001FE3F8(void *);
void func_00152390(struct O *, int, int, int);
void BroadcastGameEvent(void *, int, struct S *);
void AddRhythmScore(struct S *, float);
float GRhythmTimingScore(struct S *, struct E *);
float func_00153638(struct S *, float);
int func_0011D4D0(float, float, float);
void func_001533B0(struct S *p, struct O *po)
{
    void *pv = po->p150;
    int i, j;
    struct E *pe;
    p->n230 = GrfPressedWithin__5POCTLUif(pv, 0xF0, 0.0f);
    if (!p->n230)
        return;
    for (i = 0; i < p->c1C0; i++) {
        pe = &p->a1C4[i];
        if (pe->po != po)
            continue;
        if (pe->n20)
            continue;
        BroadcastGameEvent(D_002DEB30, 0x35, p);
        if (func_001FE3F8(D_003DD040) < pe->gC - p->g58) {
            func_00152390(po, -1, -1, 1);
            AddRhythmScore(p, p->g298);
            return;
        }
        if (pe->n1C) {
            func_00152390(po, -1, -1, 1);
            AddRhythmScore(p, p->g2A0);
            return;
        }
        pe->n1C = 1;
        if (POCTL_FConsumePressed(pv, pe->n4)) {
            func_00152390(po, pe->n4, pe->n8, 0);
            AddRhythmScore(p, func_00153638(p, GRhythmTimingScore(p, pe)));
            for (j = 0; j < p->n60; j++) {
                struct Q *pq = &p->a70[j];
                if (func_0011D4D0(pq->g0, pe->gC, 0.1f)) {
                    pq->n20 = 1;
                    break;
                }
            }
        } else {
            func_00152390(po, -1, -1, 1);
            AddRhythmScore(p, p->g29C);
        }
        return;
    }
}
